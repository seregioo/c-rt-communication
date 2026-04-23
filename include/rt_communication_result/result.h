/**
 * @file rt_communication_result/result.h
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Header file for program results
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */

#ifndef RT_COMMUNICATION_RESULT_H
#define RT_COMMUNICATION_RESULT_H

typedef struct rt_communication_result_e rt_communication_result_t;

typedef const char *(
    rt_communication_result_callback_string_function_t)(rt_communication_result_t
                                                            *);

typedef enum rt_communication_result_type_e {
  RT_COMMUNICATION_RESULT_TYPE_FAILURE =
      -2, // errors are recoverable, failures not
  RT_COMMUNICATION_RESULT_TYPE_ERROR,
  RT_COMMUNICATION_RESULT_TYPE_SUCCESS,
  RT_COMMUNICATION_RESULT_TYPE_MAX_VALUE,
} rt_communication_result_type_t;

/**
 * @brief Creates the RealTime communication result
 *
 * @param result_type The type for the result
 * @param sub_type The specific type for the result
 * @param result_value A pointer to the resulted value
 * @param rt_result_callback_funct A pointer to the resulted value
 * @return The created result
 */
rt_communication_result_t *rt_communication_result_create(
    rt_communication_result_type_t result_type, int sub_type,
    void *result_value,
    rt_communication_result_callback_string_function_t
        rt_result_callback_funct);
/**
 * @brief Destroys the result, but not its data content
 *
 * @param rt_result pointer to the result
 */
void rt_communication_result_destroy(rt_communication_result_t *rt_result);

/**
 * @brief Returns the type of the RealTime result
 *
 * @param rt_result The result
 * @return the type
 */
rt_communication_result_type_t
rt_communication_result_get_type(rt_communication_result_t *rt_result);

/**
 * @brief Returns the subtype of the RealTime result
 *
 * @param rt_result The result
 * @return the numeric value of the subtype
 */
int rt_communication_result_get_subtype(rt_communication_result_t *rt_result);

/**
 * @brief Gets the value asociated with that result, if there is one
 *
 * @param rt_result The result
 * @return a pointer to the value, or NULL
 */
void *rt_communication_result_get_value(rt_communication_result_t *rt_result);

/**
 * @brief Gets the string asociated with that result
 *
 * @param rt_result The result
 * @return a string with the message
 */
const char *
rt_communication_result_get_string(rt_communication_result_t *rt_result);

#endif // RT_COMMUNICATION_RESULT_H
