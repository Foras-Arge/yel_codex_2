#include "honeywell.h"
#include "driver/i2c_master.h"
#include "esp_log.h"
#include "math.h"
#include "esp_system.h"
#include <string.h>

#define FLOW_I2C_NUM I2C_NUM_0     // Akış için I2C portu
#define PRESSURE_I2C_NUM I2C_NUM_1 // Basınç için I2C portu
#define I2C_MASTER_FREQ_HZ 100000  // I2C hızı
#define I2C_ADDRESS 0x38           // I2C adresi (8-bit formatında)
#define REPEAT_CALCULATION 5       // Ölçüm ortalaması için tekrar sayısı

static const char *TAG = "Pressure-Flow Sensors";
#define FP_SENSORS_DEBUG 1

#define PRESSURE_CONVERSION_FACTOR 0.095321 // Basınç dönüşüm katsayısı
#define PRESSURE_ALPHA 0.05                 // Basınç için düşük geçiren filtre alfa değeri
#define FLOW_ALPHA 0.1                      // Akış için düşük geçiren filtre alfa değeri

sensor_data_t sensor_data;
i2c_master_dev_handle_t flow_dev_handle;
i2c_master_dev_handle_t pressure_dev_handle;
i2c_master_bus_handle_t flow_bus_handle;
i2c_master_bus_handle_t pressure_bus_handle;

double pressure_filtered = 0;
double flow_filtered = 0;

int convert_pressure(int raw_pressure)
{
    return (int)(PRESSURE_CONVERSION_FACTOR * raw_pressure);
}

int convert_flow(int raw_flow)
{
    /*
     * Polynomial coefficients obtained from flow_analysis.m. The original
     * polynomials included constant offsets which caused the converted flow to
     * report a non‑zero value when there was no differential pressure.  To make
     * the zero‑flow condition map to 0 L/min we remove those offsets here and
     * explicitly subtract them after applying the polynomial.
     */
    double p1[] = {2.5419e-06, -0.0017567, 0.4532, 2.5134};
    double p2[] = {4.1106e-07, -0.0005016, 0.24597, 5.7692};
    double p3[] = {-9.0825e-06, 0.063717, 25.757};
    double flow;
    int sign;

    // Handle sign and convert to absolute value
    if (raw_flow < 0)
    {
        sign = -1;
        raw_flow = -raw_flow; // Simplified absolute value calculation
    }
    else
    {
        sign = 1;
    }

    // Determine which polynomial to use based on flow range. Each polynomial
    // originally contained a constant term representing the sensor's zero-flow
    // output.  When calibrated_flow is zero we want the result to be exactly
    // zero, so the constant term is subtracted after applying the polynomial.

    double offset; // zero-flow offset in the same units as 'flow'

    if (raw_flow <= 20)
    {
        flow = p1[0] * raw_flow * raw_flow * raw_flow +
               p1[1] * raw_flow * raw_flow +
               p1[2] * raw_flow;
        offset = p1[3];
    }
    else if (raw_flow <= 450)
    {
        flow = p2[0] * raw_flow * raw_flow * raw_flow +
               p2[1] * raw_flow * raw_flow +
               p2[2] * raw_flow;
        offset = p2[3];
    }
    else
    {
        flow = p3[0] * raw_flow * raw_flow +
               p3[1] * raw_flow;
        offset = p3[2];
    }

    // Remove the offset then apply sign and scaling
    flow = (flow - offset) * 11 * sign;
    return (int)flow;
}

/**
 * @brief Dönüştürülmüş basınç değerine düşük geçiren filtre uygular.
 *
 * @param conv_pressure Dönüştürülmüş basınç değeri
 * @return int Filtrelenmiş basınç değeri
 */
int filter_pressure(int conv_pressure)
{
    pressure_filtered = PRESSURE_ALPHA * conv_pressure + (1.0 - PRESSURE_ALPHA) * pressure_filtered;
    return (int)pressure_filtered;
}

/**
 * @brief Dönüştürülmüş akış değerine düşük geçiren filtre uygular.
 *
 * @param conv_flow Dönüştürülmüş akış değeri
 * @return int Filtrelenmiş akış değeri
 */

int filter_flow(int conv_flow)
{
    flow_filtered = FLOW_ALPHA * conv_flow + (1.0 - FLOW_ALPHA) * flow_filtered;
    return (int)flow_filtered;
}

/**
 * @brief Raw basınç ve akış değerlerini alır, kalibrasyon (offset düşme),
 *        dönüşüm ve filtre uygulayarak son değerleri verir.
 *
 * @param raw_pressure Raw basınç değeri (örneğin I2C'den okunan)
 * @param raw_flow     Raw akış değeri
 * @param final_pressure Pointer üzerinden elde edilecek son basınç değeri
 * @param final_flow     Pointer üzerinden elde edilecek son akış değeri
 */
void process_sensor_data(int raw_pressure, int raw_flow, int *final_pressure, int *final_flow)
{
    // Kalibrasyon: sensör boşta okunan değeri düşüyoruz
    int calibrated_pressure = raw_pressure - sensor_data.pressure_base;
    int calibrated_flow = raw_flow - sensor_data.flow_base;

    // Dönüşüm: fiziksel birimlere çeviriyoruz
    int conv_pressure = convert_pressure(calibrated_pressure);
    int conv_flow = convert_flow(calibrated_flow);

    // Filtreleme: düşük geçiren filtre uyguluyoruz
    *final_pressure = filter_pressure(conv_pressure);
    *final_flow = filter_flow(conv_flow);
}

//*****************************************************
// I2C Başlatma ve Ölçüm Fonksiyonları
//*****************************************************

esp_err_t init_flow()
{
    i2c_master_bus_config_t i2c_bus_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = FLOW_I2C_NUM,
        .scl_io_num = FP_FLOW_SCL_IO,
        .sda_io_num = FP_FLOW_SDA_IO,
        .glitch_ignore_cnt = 7,
        .intr_priority = 0,
        .trans_queue_depth = 0,
        .flags.enable_internal_pullup = true,
    };

    esp_err_t ret = i2c_new_master_bus(&i2c_bus_config, &flow_bus_handle);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to create flow I2C bus: %s", esp_err_to_name(ret));
        return ret;
    }

    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = I2C_ADDRESS,
        .scl_speed_hz = I2C_MASTER_FREQ_HZ,
    };

    ret = i2c_master_bus_add_device(flow_bus_handle, &dev_config, &flow_dev_handle);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to add flow I2C device: %s", esp_err_to_name(ret));
        i2c_del_master_bus(flow_bus_handle);
        flow_bus_handle = NULL;
        return ret;
    }

    return ESP_OK;
}

esp_err_t init_pressure()
{
    i2c_master_bus_config_t i2c_bus_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = PRESSURE_I2C_NUM,
        .scl_io_num = FP_PRESS_SCL_IO,
        .sda_io_num = FP_PRESS_SDA_IO,
        .glitch_ignore_cnt = 7,
        .intr_priority = 0,
        .trans_queue_depth = 0,
        .flags.enable_internal_pullup = true,
    };

    esp_err_t ret = i2c_new_master_bus(&i2c_bus_config, &pressure_bus_handle);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to create pressure I2C bus: %s", esp_err_to_name(ret));
        return ret;
    }

    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = I2C_ADDRESS,
        .scl_speed_hz = I2C_MASTER_FREQ_HZ,
    };

    ret = i2c_master_bus_add_device(pressure_bus_handle, &dev_config, &pressure_dev_handle);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to add pressure I2C device: %s", esp_err_to_name(ret));
        i2c_del_master_bus(pressure_bus_handle);
        pressure_bus_handle = NULL;
        return ret;
    }

    return ESP_OK;
}

/**
 * @brief I2C üzerinden sensörden veriyi alır ve belirli sayıda örnek ortalamasını hesaplar.
 *
 * @param i2c_num I2C portu (örneğin, PRESSURE_I2C_NUM veya FLOW_I2C_NUM)
 * @param times   Alınacak örnek sayısı
 * @return int    Ortalama raw ölçüm değeri (hata durumunda -1 döner)
 */
int measure_i2c(i2c_port_t i2c_num, uint8_t times)
{
    int retval = 0;
    uint8_t out[4];
    esp_err_t ret;
    i2c_master_dev_handle_t dev_handle;
    const int max_retries = 3;
    int retry_count = 0;

    // Select the correct device handle based on I2C port
    if (i2c_num == FLOW_I2C_NUM)
    {
        dev_handle = flow_dev_handle;
    }
    else
    {
        dev_handle = pressure_dev_handle;
    }

    for (int i = 0; i < times; i++)
    {
        retry_count = 0;
        while (retry_count < max_retries)
        {
            ret = i2c_master_receive(dev_handle, out, sizeof(out), 5 / portTICK_PERIOD_MS);
            if (ret == ESP_OK)
            {
                break; // Success, exit retry loop
            }

            // Handle I2C NACK error silently
            if (ret == ESP_ERR_TIMEOUT || ret == ESP_ERR_INVALID_STATE)
            {
                retry_count++;
                vTaskDelay(pdMS_TO_TICKS(5)); // Small delay between retries
                continue;
            }

            // For other errors, return error without logging
            return -1;
        }

        if (retry_count >= max_retries)
        {
            continue; // Skip this measurement but continue with others
        }

        // Ölçüm: İlk 6 bitten maske uygulayarak, 14-bitlik ölçüm değeri elde ediyoruz
        int measurement = ((out[0] & 0x3F) << 8) | out[1];
        retval += measurement;
    }

    if (retval == 0)
    {
        return -1;
    }

    retval = retval / times;
    return retval;
}

//*****************************************************
// Sensör Okuma ve İşleme Task'ı
//*****************************************************

void fp_sensors_task(void *pvParameters)
{
    while (1)
    {

        sensor_data.raw_pressure = measure_i2c(PRESSURE_I2C_NUM, REPEAT_CALCULATION);
        sensor_data.raw_flow = measure_i2c(FLOW_I2C_NUM, REPEAT_CALCULATION);
        // #if FP_SENSORS_DEBUG
        //         ESP_LOGI(TAG, "Raw Pressure: %d , Raw Flow: %d ", sensor_data.raw_pressure, sensor_data.raw_flow);
        // #endif

        process_sensor_data(sensor_data.raw_pressure, sensor_data.raw_flow,
                            &sensor_data.pressure, &sensor_data.flow);
    }
}

void honeywell_init()
{
    esp_err_t ret = init_pressure();
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Pressure sensor initialization failed");
        esp_restart();
    }
    ret = init_flow();
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Flow sensor initialization failed");
        esp_restart();
    }
    ESP_LOGI("FP_SENSORS", "Sensors initialized");

    sensor_data.pressure_base = measure_i2c(PRESSURE_I2C_NUM, 50);
    sensor_data.flow_base = measure_i2c(FLOW_I2C_NUM, 50);

    if (sensor_data.pressure_base < 8250 && sensor_data.pressure_base > 8100 && sensor_data.flow_base < 8250 && sensor_data.flow_base > 8100)
    {
        ESP_LOGI(TAG, "Pressure base: %d , Flow base: %d ", sensor_data.pressure_base, sensor_data.flow_base);
    }
    else
    {
        ESP_LOGE(TAG, "Pressure base: %d , Flow base: %d ", sensor_data.pressure_base, sensor_data.flow_base);
        esp_restart();
    }
    xTaskCreate(fp_sensors_task, "fp_sensors_task", 8192, NULL, 5, NULL);
}
