/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2011 William Braik, Yann Courtois, Jean-Marie Couteyen, Anthony
 * Roy
 *
 * StarPU is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 *
 * StarPU is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU Lesser General Public License in COPYING.LGPL for more details.
 */

#ifndef __STARPU_TOP_H__
#define __STARPU_TOP_H__

#include <starpu.h>
#include <stdlib.h>
#include <time.h>

#ifdef __cplusplus
extern "C"
{
#endif

enum starpu_top_data_type
{
	STARPU_TOP_DATA_BOOLEAN,
	STARPU_TOP_DATA_INTEGER,
	STARPU_TOP_DATA_FLOAT
};

struct starpu_top_data
{
	unsigned int id;
	const char* name;
	int int_min_value;
	int int_max_value;
	double double_min_value;
	double double_max_value;
	int active;
	enum starpu_top_data_type type;
	struct starpu_top_data * next;
};

enum starpu_top_param_type
{
	STARPU_TOP_PARAM_BOOLEAN,
	STARPU_TOP_PARAM_INTEGER,
	STARPU_TOP_PARAM_FLOAT,
	STARPU_TOP_PARAM_ENUM
};

struct starpu_top_param
{
	unsigned int id;
	const char* name;
	enum starpu_top_param_type type;
	void* value;
	char** enum_values; /* only for enum type can be NULL */
	int nb_values;
	void (*callback)(struct starpu_top_param*);
	int int_min_value; /* only for integer type */
	int int_max_value;
	double double_min_value; /*only for double type */
	double double_max_value;
	struct starpu_top_param * next;
};

enum starpu_top_message_type
{
	TOP_TYPE_GO,
	TOP_TYPE_SET,
	TOP_TYPE_CONTINUE,
	TOP_TYPE_ENABLE,
	TOP_TYPE_DISABLE,
	TOP_TYPE_DEBUG,
	TOP_TYPE_UNKNOW
};

/*****************************************************
****   Functions to call BEFORE initialisation   *****
*****************************************************/
/*
 * This fonction register a data named data_name of type boolean
 * If active=0, the value will NOT be displayed to user by default.
 * Any other value will make the value displayed by default.
*/
struct starpu_top_data *starpu_top_add_data_boolean(const char* data_name,
						    int active);
/*
 * This fonction register a data named data_name of type integer
 * The minimum and maximum value will be usefull to define the scale in UI
 * If active=0, the value will NOT be displayed to user by default.
 * Any other value will make the value displayed by default.
*/
struct starpu_top_data * starpu_top_add_data_integer(const char* data_name,
						     int minimum_value,
						     int maximum_value,
						     int active);
/*
 * This fonction register a data named data_name of type float
 * The minimum and maximum value will be usefull to define the scale in UI
 * If active=0, the value will NOT be displayed to user by default.
 * Any other value will make the value displayed by default.
*/
struct starpu_top_data* starpu_top_add_data_float(const char* data_name,
						  double minimum_value,
						  double maximum_value,
						  int active);

/*
 * This fonction register a parameter named parameter_name, of type boolean.
 * The callback fonction will be called when the parameter is modified by UI,
 * and can be null.
*/
struct starpu_top_param* starpu_top_register_parameter_boolean(const char* param_name,
							       int* parameter_field,
							       void (*callback)(struct starpu_top_param*));
/*
 * This fonction register a parameter named param_name, of type integer.
 * Minimum and maximum value will be used to prevent user seting incorrect
 * value.
 * The callback fonction will be called when the parameter is modified by UI,
 * and can be null.
*/
struct starpu_top_param* starpu_top_register_parameter_integer(const char* param_name,
							       int* parameter_field,
							       int minimum_value,
							       int maximum_value,
							       void (*callback)(struct starpu_top_param*));
/*
 * This fonction register a parameter named param_name, of type float.
 * Minimum and maximum value will be used to prevent user seting incorrect
 * value.
 * The callback fonction will be called when the parameter is modified by UI,
 * and can be null.
*/
struct starpu_top_param* starpu_top_register_parameter_float(const char* param_name,
							     double* parameter_field,
							     double minimum_value,
							     double maximum_value,
							     void (*callback)(struct starpu_top_param*));

/*
 * This fonction register a parameter named param_name, of type enum.
 * Minimum and maximum value will be used to prevent user seting incorrect
 * value.
 * The callback fonction will be called when the parameter is modified by UI,
 * and can be null.
*/
struct starpu_top_param* starpu_top_register_parameter_enum(const char* param_name,
							    int* parameter_field,
							    char** values,
							    int nb_values,
							    void (*callback)(struct starpu_top_param*));




/****************************************************
******************* Initialisation ******************
*****************************************************/
/*
 * This function must be called when all parameters and
 * data have been registered AND initialised (for parameters).
 * This function will wait for a TOP to connect, send initialisation
 * sentences, and wait for the GO message.
 */
void starpu_top_init_and_wait(const char* server_name);

/****************************************************
************ To call after initialisation************
*****************************************************/

/*
 * This function should be called after every modification
 * of a parameter from something other than starpu_top.
 * This fonction notice UI that the configuration changed
 */
void starpu_top_update_parameter(const struct starpu_top_param* param);

/*
 * This functions update the value of the starpu_top_data on UI
 */
void starpu_top_update_data_boolean(const struct starpu_top_data* data,
				    int value);
void starpu_top_update_data_integer(const struct starpu_top_data* data,
				    int value);
void starpu_top_update_data_float(const struct starpu_top_data* data,
				  double value);

/*
 * This functions are usefull in debug mode. The starpu developper doesn't need
 * to check if the debug mode is active.
 * This is checked by starpu_top itsefl.
 *
 * top_debug_log just send a message to display by UI
 * top_debug_lock send a message and wait for a continue message from UI
 * to return
 *
 * The lock (wich create a stop-point) should be called only by the main thread.
 * Calling it from more than one thread is not supported.
 */
void starpu_top_debug_log(const char* message);
void starpu_top_debug_lock(const char* message);


#ifdef __cplusplus
}
#endif

#endif /* __STARPU_TOP_H__ */

