#include "u_math.h"
#include "string.h"

double abs_double(double x)
{
    if (x < 0)
        return -x;
    else
        return x;
}

void parse_string(const char *input, uint8_t *action_type, char *data_str)
{
    if (input == NULL || action_type == NULL || data_str == NULL)
    {
        return;
    }

    // İlk karakteri action type olarak al
    *action_type = (uint8_t)(input[1] - '0'); // ~0,548 -> '0' karakteri uint8_t olarak alınır

    // Virgülden sonraki kısmı string olarak kopyala
    strcpy(data_str, &input[3]); // '548' kısmını string olarak kopyalar
}

float pressure_to_speed(float pressure)
{
    // Define the pressure-speed mapping table
    struct
    {
        float pressure;
        float speed;
    } mapping[] = {
        {0, 0.1}, {1, 8.1}, {2, 9.6}, {3, 9.8}, {4, 10.4}, {5, 11.2}, {6, 11.8}, {7, 12.5}, {8, 13.3}, {9, 13.9}, {10, 14.4}, {20, 19.7}, {30, 24.4}, {40, 27.7}, {50, 35.1}, {100, 50.7}, {200, 70.0}, {300, 61.5}, {400, 98.7}};

    int table_size = sizeof(mapping) / sizeof(mapping[0]);

    // Basınç tablodaki en küçük değerden küçükse minimum hızı döndür
    if (pressure <= mapping[0].pressure)
    {
        return mapping[0].speed;
    }

    // Basınç tablodaki en büyük değerden büyükse maksimum hızı döndür
    if (pressure >= mapping[table_size - 1].pressure)
    {
        return mapping[table_size - 1].speed;
    }

    // Lineer interpolasyon ile hız hesaplama
    for (int i = 0; i < table_size - 1; i++)
    {
        if (pressure >= mapping[i].pressure && pressure <= mapping[i + 1].pressure)
        {
            float p1 = mapping[i].pressure, p2 = mapping[i + 1].pressure;
            float s1 = mapping[i].speed, s2 = mapping[i + 1].speed;
            return s1 + (pressure - p1) * (s2 - s1) / (p2 - p1);
        }
    }

    return mapping[table_size - 1].speed;
}

void u_shift_and_insert_float(float arr[], int size, float new_value)
{
    memmove(&arr[1], &arr[0], (size - 1) * sizeof(float));
    arr[0] = new_value; // Yeni elemanı başa ekle
}

float u_average_float(float arr[], uint64_t count)
{
    float sum = 0;
    for (int i = 0; i < count; i++)
    {
        sum += arr[i];
    }
    return sum / count;
}

float u_sum_float(float arr[], uint64_t count)
{
    float sum = 0;
    for (int i = 0; i < count; i++)
    {
        sum += arr[i];
    }
    return sum;
}

void u_shift_and_insert_double(double arr[], int size, double new_value)
{
    memmove(&arr[1], &arr[0], (size - 1) * sizeof(double));
    arr[0] = new_value; // Yeni elemanı başa ekle
}

double u_average_double(double arr[], uint64_t count)
{
    double sum = 0;
    for (int i = 0; i < count; i++)
    {
        sum += arr[i];
    }
    return sum / count;
}

double u_sum_double(double arr[], uint64_t count)
{
    double sum = 0;
    for (int i = 0; i < count; i++)
    {
        sum += arr[i];
    }
    return sum;
}

void u_shift_and_insert_uint16(uint16_t arr[], int size, uint16_t new_value)
{
    memmove(&arr[1], &arr[0], (size - 1) * sizeof(uint16_t));
    arr[0] = new_value; // Yeni elemanı başa ekle
}

uint16_t u_sum_uint16(uint16_t arr[], uint64_t count)
{
    uint16_t sum = 0;
    for (int i = 0; i < count; i++)
    {
        sum += arr[i];
    }
    return sum;
}

uint16_t u_average_uint16(uint16_t arr[], uint64_t count)
{
    uint16_t sum = 0;
    for (int i = 0; i < count; i++)
    {
        sum += arr[i];
    }
    return sum / count;
}

void u_shift_and_insert_int16(int16_t arr[], int size, int16_t new_value)
{
    memmove(&arr[1], &arr[0], (size - 1) * sizeof(int16_t));
    arr[0] = new_value; // Yeni elemanı başa ekle
}

int16_t u_sum_int16(int16_t arr[], uint64_t count)
{
    int16_t sum = 0;
    for (int i = 0; i < count; i++)
    {
        sum += arr[i];
    }
    return sum;
}

int16_t u_average_int16(int16_t arr[], uint64_t count)
{
    int16_t sum = 0;
    for (int i = 0; i < count; i++)
    {
        sum += arr[i];
    }
    return sum / count;
}

void u_shift_and_insert_uint64(uint64_t arr[], int size, uint64_t new_value)
{
    memmove(&arr[1], &arr[0], (size - 1) * sizeof(uint64_t));
    arr[0] = new_value; // Yeni elemanı başa ekle
}

uint64_t u_sum_uint64(uint64_t arr[], uint64_t count)
{
    uint64_t sum = 0;
    for (int i = 0; i < count; i++)
    {
        sum += arr[i];
    }
    return sum;
}

uint64_t u_average_uint64(uint64_t arr[], uint64_t count)
{
    uint64_t sum = 0;
    for (int i = 0; i < count; i++)
    {
        sum += arr[i];
    }
    return sum / count;
}

void u_shift_and_insert_int64(int64_t arr[], int size, int64_t new_value)
{
    memmove(&arr[1], &arr[0], (size - 1) * sizeof(int64_t));
    arr[0] = new_value; // Yeni elemanı başa ekle
}

int64_t u_sum_int64(int64_t arr[], uint64_t count)
{
    int64_t sum = 0;
    for (int i = 0; i < count; i++)
    {
        sum += arr[i];
    }
    return sum;
}

int64_t u_average_int64(int64_t arr[], uint64_t count)
{
    int64_t sum = 0;
    for (int i = 0; i < count; i++)
    {
        sum += arr[i];
    }
    return sum / count;
}

static uint8_t hex_char_to_val(char c)
{
    if ('0' <= c && c <= '9')
        return c - '0';
    if ('A' <= c && c <= 'F')
        return c - 'A' + 10;
    if ('a' <= c && c <= 'f')
        return c - 'a' + 10;
    return 0xFF; // Geçersiz karakter
}

uint8_t *u_hex_to_bytes(const char *hex, size_t *out_len)
{
    if (hex == NULL || out_len == NULL)
    {
        return NULL;
    }
    size_t hex_len = strlen(hex);
    if (hex_len % 2 != 0)
    {
        return NULL;
    }
    *out_len = hex_len / 2;
    uint8_t *bytes = (uint8_t *)malloc(*out_len);
    if (!bytes)
    {
        return NULL;
    }

    for (size_t i = 0; i < *out_len; i++)
    {
        uint8_t high = hex_char_to_val(hex[i * 2]);
        uint8_t low = hex_char_to_val(hex[i * 2 + 1]);

        if (high == 0xFF || low == 0xFF)
        {
            free(bytes);
            return NULL; // Geçersiz karakter varsa
        }

        bytes[i] = (high << 4) | low;
    }
    return bytes;
}
