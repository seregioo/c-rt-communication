/**
 * @file rt_result/result.h
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Header file for program results
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */

#ifndef RT_RESULT_H
#define RT_RESULT_H

#include <stdbool.h>
#include <sys/types.h>

#define RT_RESULT_MAX_BUFFER_SIZE 1024

typedef struct rt_result_e rt_result_t;

typedef const char *(rt_result_callback_string_function_t)(rt_result_t *);

typedef enum rt_result_type_e {
  RT_RESULT_TYPE_INVALID = 0,
  RT_RESULT_TYPE_SUCCESS,
  RT_RESULT_TYPE_ERROR,
  RT_RESULT_TYPE_FAILURE, // errors are recoverable, failures not
  RT_RESULT_TYPE_MAX_VALUE,
} rt_result_type_t;

struct rt_result_e {
  rt_result_type_t result_type;
  int sub_type;
  uint invalid_sub_type;
  uint max_sub_type;
  void *result_value;
  rt_result_callback_string_function_t *rt_result_callback_string_function;
  char result_string[RT_RESULT_MAX_BUFFER_SIZE];
};

/**
 * @brief Creates the RT communication result
 *
 * @param result_type The type for the result
 * @param sub_type The specific type for the result
 * @param invalid_sub_type Invalid type for the result, always 0
 * @param max_sub_type Max value for the result
 * @param result_value A pointer to the resulted value
 * @param rt_result_callback_funct A pointer to the resulted value
 * @return The created result
 */
rt_result_t
rt_result_create(rt_result_type_t result_type, int sub_type,
                 uint invalid_sub_type, uint max_sub_type, void *result_value,
                 rt_result_callback_string_function_t rt_result_callback_funct);
/**
 * @brief Destroys the result, but not its data content
 *
 * @param rt_result pointer to the result
 */
void rt_result_destroy(rt_result_t rt_result);

/**
 * @brief Returns the type of the RT result
 *
 * @param rt_result The result
 * @return the type
 */
rt_result_type_t rt_result_get_type(rt_result_t *rt_result);

/**
 * @brief Returns the subtype of the RT result
 *
 * @param rt_result The result
 * @return the numeric value of the subtype
 */
int rt_result_get_subtype(rt_result_t *rt_result);

/**
 * @brief Gets the value asociated with that result, if there is one
 *
 * @param rt_result The result
 * @return a pointer to the value, or NULL
 */
void *rt_result_get_value(rt_result_t *rt_result);

/**
 * @brief Gets if the value succeded
 *
 * @param rt_result The result
 * @return true if succeded, false other wise
 */
bool rt_result_succeded(rt_result_t *rt_result);

/**
 * @brief Gets the string asociated with that result
 *
 * @param rt_result The result
 * @return a string with the message
 */
const char *rt_result_get_string(rt_result_t *rt_result);

#endif // RT_RESULT_H
