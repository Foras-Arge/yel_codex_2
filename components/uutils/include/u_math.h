#ifndef U_MATH_H
#define U_MATH_H

#include <stdint.h>
#include <stddef.h>

// Temel matematik fonksiyonları
double abs_double(double x);
void parse_string(const char *input, uint8_t *action_type, char *data_str);
float pressure_to_speed(float pressure);

// Float array işlemleri
void u_shift_and_insert_float(float arr[], int size, float new_value);
float u_average_float(float arr[], uint64_t count);
float u_sum_float(float arr[], uint64_t count);

// Uint16 array işlemleri
void u_shift_and_insert_uint16(uint16_t arr[], int size, uint16_t new_value);
uint16_t u_average_uint16(uint16_t arr[], uint64_t count);
uint16_t u_sum_uint16(uint16_t arr[], uint64_t count);

// Int16 array işlemleri
void u_shift_and_insert_int16(int16_t arr[], int size, int16_t new_value);
int16_t u_average_int16(int16_t arr[], uint64_t count);
int16_t u_sum_int16(int16_t arr[], uint64_t count);

// Uint64 array işlemleri
void u_shift_and_insert_uint64(uint64_t arr[], int size, uint64_t new_value);
uint64_t u_sum_uint64(uint64_t arr[], uint64_t count);
uint64_t u_average_uint64(uint64_t arr[], uint64_t count);

// Int64 array işlemleri
void u_shift_and_insert_int64(int64_t arr[], int size, int64_t new_value);
int64_t u_sum_int64(int64_t arr[], uint64_t count);
int64_t u_average_int64(int64_t arr[], uint64_t count);

// Hex string işlemleri
uint8_t *u_hex_to_bytes(const char *hex, size_t *out_len);

#endif