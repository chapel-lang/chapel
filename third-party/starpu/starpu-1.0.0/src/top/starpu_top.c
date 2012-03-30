/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2011 William Braik, Yann Courtois, Jean-Marie Couteyen, Anthony Roy
 * Copyright (C) 2011, 2012 Centre National de la Recherche Scientifique
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


#include <starpu_top.h>
#include <top/starpu_top_message_queue.h>
#include <top/starpu_top_connection.h>
#include <top/starpu_top_core.h>
#include <profiling/profiling.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <common/timing.h>
#include <common/utils.h>

extern struct _starpu_top_message_queue*  _starpu_top_mt;
int starpu_top = 0;
int starpu_top_debug_on = 0;
unsigned int starpu_top_data_cpt = 0;
unsigned int starpu_top_param_cpt = 0;
struct starpu_top_data* starpu_top_first_data = NULL;
struct starpu_top_param* starpu_top_first_param = NULL;
struct starpu_top_data** starpu_top_datas;
struct starpu_top_param** starpu_top_params;

sem_t starpu_top_wait_for_go;
pthread_mutex_t starpu_top_wait_for_continue_mutex;
pthread_cond_t starpu_top_wait_for_continue_cond = PTHREAD_COND_INITIALIZER;

int _starpu_top_status_get()
{
	return starpu_top;
}

static
unsigned long long current_timestamp();

/*********************************************
*****************INIT FUNC********************
**********************************************/

static
char *message_for_topdata_init(struct starpu_top_data* data);

static
char *message_for_topparam_init(struct starpu_top_param* param);

/*
 * we store data and param in a tab to offer a O(1) access when the program  is
 * running
 */
static
void copy_data_and_param()
{
	printf("%s:%d trace\n", __FILE__, __LINE__);
	//copying datas
	starpu_top_datas = (struct starpu_top_data **) malloc(starpu_top_data_cpt*sizeof(struct starpu_top_data*));
	struct starpu_top_data* cur = starpu_top_first_data;
	unsigned int i = 0;
	for(i = 0; i < starpu_top_data_cpt; i++)
	{
		starpu_top_datas[i] = cur;
		cur = cur->next;
	}
	//copying params
	starpu_top_params = (struct starpu_top_param **) malloc(starpu_top_param_cpt*sizeof(struct starpu_top_param*));
	struct starpu_top_param* cur2 = starpu_top_first_param;
	for(i = 0; i < starpu_top_param_cpt; i++)
	{
		starpu_top_params[i] = cur2;
		cur2 = cur2->next;
	}
}

static void starpu_top_get_device_type(int id, char* type)
{
	enum starpu_archtype device_type=starpu_worker_get_type(id);
	switch (device_type)
	{
	case STARPU_CPU_WORKER:
		strncpy(type, "CPU",9);
		break;
	case STARPU_CUDA_WORKER:
		strncpy(type, "CUDA",9);
		break;
	case STARPU_OPENCL_WORKER:
		strncpy(type, "OPENCL",9);
		break;
	case STARPU_GORDON_WORKER:
		strncpy(type, "GORDON",9);
		break;
	}
}

static void starpu_top_send_devices_info()
{
	char* message=(char *)malloc(5*sizeof(char));
	snprintf(message,5,"DEV\n");
	_starpu_top_message_add(_starpu_top_mt,message);

	unsigned int i;
	for(i=0;i<starpu_worker_get_count();i++)
	{
		message=(char *)malloc(sizeof(char)*128);
		char dev_type[10];
		char dev_name[64];
		starpu_top_get_device_type(i,dev_type);
		starpu_worker_get_name(i, dev_name,64);
		snprintf(message, 128, "%u;%s;%s\n", i, dev_type, dev_name);
		_starpu_top_message_add(_starpu_top_mt,message);
	}

	message=(char*)malloc(6*sizeof(char));
	snprintf(message,6,"/DEV\n");
	_starpu_top_message_add(_starpu_top_mt,message);
}


void starpu_top_init_and_wait(const char* server_name)
{
	starpu_top=1;
	sem_init(&starpu_top_wait_for_go,0,0);

	_STARPU_PTHREAD_MUTEX_INIT(&starpu_top_wait_for_continue_mutex, NULL);

	//profiling activation
	starpu_profiling_status_set(STARPU_PROFILING_ENABLE);

	//init locked queue before adding the first message
	_starpu_top_mt = _starpu_top_message_queue_new();
	STARPU_ASSERT(_starpu_top_mt);

	//waiting for UI to connect
	printf("%s:%d launching network threads\n", __FILE__, __LINE__);
	_starpu_top_communications_threads_launcher();

	//sending server information (report to protocol)
	char* message = (char *) malloc(strlen("SERVERINFO\n")+1);
	sprintf(message, "%s", "SERVERINFO\n");
	_starpu_top_message_add(_starpu_top_mt,message);
	message = (char *) malloc(strlen(server_name)+2);
	sprintf(message, "%s\n", server_name);
	_starpu_top_message_add(_starpu_top_mt,message);
	message = (char *) malloc(25);
	sprintf(message, "%lld\n", current_timestamp());
	_starpu_top_message_add(_starpu_top_mt,message);
	message = (char *) malloc(strlen("/SERVERINFO\n")+1);
	sprintf(message,"%s", "/SERVERINFO\n");
	_starpu_top_message_add(_starpu_top_mt,message);


	//sending data list
	message = (char *) malloc(strlen("DATA\n")+1);
	sprintf(message, "%s", "DATA\n");
	_starpu_top_message_add(_starpu_top_mt,message);
	struct starpu_top_data * cur_data = starpu_top_first_data;
	while(cur_data != NULL)
	{
		_starpu_top_message_add(_starpu_top_mt,message_for_topdata_init(cur_data));
		cur_data = cur_data->next;
	}
	message = (char *) malloc(strlen("/DATA\n")+1);
	sprintf(message, "%s", "/DATA\n");
	_starpu_top_message_add(_starpu_top_mt,message);

	//sending parameter list
	message = (char *) malloc(strlen("PARAMS\n")+1);
	sprintf(message, "%s", "PARAMS\n");
	_starpu_top_message_add(_starpu_top_mt,message);
	struct starpu_top_param * cur_param = starpu_top_first_param;
	printf("%s:%d sending parameters\n", __FILE__, __LINE__);
	while(cur_param != NULL)
	{
	  _starpu_top_message_add(_starpu_top_mt,message_for_topparam_init(cur_param));
	  cur_param = cur_param->next;
	}
	printf("%s:%d parameters sended\n", __FILE__, __LINE__);
	message = (char *) malloc(strlen("/PARAMS\n")+1);
	sprintf(message, "%s", "/PARAMS\n");
	_starpu_top_message_add(_starpu_top_mt,message);


	//sending DEVICE list
	printf("%s:%d sending devices info\n", __FILE__, __LINE__);
	starpu_top_send_devices_info();
	printf("%s:%d devices_info sended\n", __FILE__, __LINE__);
	//copying data and params
	copy_data_and_param();

	//sending READY message
	message = (char *) malloc(strlen("READY\n")+1);
	sprintf(message, "%s", "READY\n");
	_starpu_top_message_add(_starpu_top_mt,message);

	//This threads keeps locked while we don't receive an GO message from UI
	printf("%s:%d waiting for GO message\n", __FILE__, __LINE__);
	sem_wait(&starpu_top_wait_for_go);
}

static
void starpu_top_enqueue_data(struct starpu_top_data * data)
{
	if(starpu_top_first_data == NULL)
	{
		starpu_top_first_data = data;
	}
	else
	{
		struct starpu_top_data * cur = starpu_top_first_data;
		while(cur->next != NULL)
			cur = cur->next;
		cur->next = data;
	}
}

struct starpu_top_data * starpu_top_add_data_boolean(const char* data_name,
						     int active)
{
	struct starpu_top_data * data = (struct starpu_top_data *) malloc(sizeof(struct starpu_top_data));
	data->id = starpu_top_data_cpt++;
	data->name = data_name;
	data->type = STARPU_TOP_DATA_BOOLEAN;
	data->active = active;
	data->next = NULL;

	starpu_top_enqueue_data(data);

	return data;
}

struct starpu_top_data * starpu_top_add_data_integer(const char* data_name,
						     int minimum_value,
						     int maximum_value,
						     int active)
{
	struct starpu_top_data * data = (struct starpu_top_data *) malloc(sizeof(struct starpu_top_data));
	data->id = starpu_top_data_cpt++;
	data->name = data_name;
	data->type = STARPU_TOP_DATA_INTEGER;
	data->int_min_value = minimum_value;
	data->int_max_value = maximum_value;
	data->active = active;
	data->next = NULL;

	starpu_top_enqueue_data(data);

	return data;
}

struct starpu_top_data* starpu_top_add_data_float(const char* data_name,
						  double minimum_value,
						  double maximum_value,
						  int active)
{
	struct starpu_top_data * data = (struct starpu_top_data *) malloc(sizeof(struct starpu_top_data));
	data->id = starpu_top_data_cpt++;
	data->name = data_name;
	data->type = STARPU_TOP_DATA_FLOAT;
	data->double_min_value = minimum_value;
	data->double_max_value = maximum_value;
	data->active = active;
	data->next = NULL;

	starpu_top_enqueue_data(data);

	return data;
}

char *message_for_topdata_init(struct starpu_top_data* data)
{
	char*message = (char *) malloc(256+strlen(data->name));
	switch(data->type)
	{
		case STARPU_TOP_DATA_BOOLEAN:
			sprintf(message,
					"BOOL;%u;%s;%d\n",
					data->id,
					data->name,
					data->active ? 1 : 0);
			break;
		case STARPU_TOP_DATA_INTEGER:
			sprintf(message,
					"INT;%u;%s;%d;%d;%d\n",
					data->id,
					data->name,
					data->int_min_value,
					data->int_max_value,
					data->active ? 1 : 0);
			break;
		case STARPU_TOP_DATA_FLOAT:
			sprintf(message,
					"FLOAT;%u;%s;%f;%f;%d\n",
					data->id,
					data->name,
					data->double_min_value,
					data->double_max_value,
					data->active ? 1 : 0);
			break;
	}
	return message;
}

char *message_for_topparam_init(struct starpu_top_param* param)
{
	char*message = NULL;
	int i;
	int length=0;
	switch(param->type)
	{
	case STARPU_TOP_PARAM_BOOLEAN:
		message = (char *) malloc(256);
		sprintf(message,
				"BOOL;%u;%s;%d\n",
				param->id,
				param->name,
				(*(int*)(param->value)) ? 1 : 0);
		break;
	case STARPU_TOP_PARAM_INTEGER:
		message = (char *) malloc(256);
		sprintf(message,
				"INT;%u;%s;%d;%d;%d\n",param->id,
				param->name,
				param->int_min_value,
				param->int_max_value,
				*(int*)(param->value));
		break;
	case STARPU_TOP_PARAM_FLOAT:
		message = (char *) malloc(256);
		sprintf(message,
				"FLOAT;%u;%s;%f;%f;%f\n",
				param->id,
				param->name,
				param->double_min_value,
				param->double_max_value,
				*(double*)(param->value));
		break;
	case STARPU_TOP_PARAM_ENUM:
		//compute message lenght
		for(i = 0; i < param->nb_values; i++)
		{
			length += strlen(param->enum_values[i])+1;
		}
		message = (char *) malloc(256+length);
		sprintf(message,
				"ENUM;%u;%s;",
				param->id,
				param->name);

		//compute the begin of enums elements in message
		char* cur = message+strlen(message);
		//add each enum element
		for(i = 0; i < param->nb_values; i++)
		{
			strcpy(cur, param->enum_values[i]);
			cur+=strlen(cur);
			*cur=';';
			cur++;
		}
		sprintf(cur,
				"%d\n",
				*((int*)(param->value)));
		break;
	}
	return message;
}

static
void starpu_top_enqueue_param(struct starpu_top_param* param)
{
	if(starpu_top_first_param == NULL)
	{
		starpu_top_first_param = param;
	}
	else
	{
		struct starpu_top_param * cur = starpu_top_first_param;
		while(cur->next != NULL)
			cur = cur->next;
		cur->next = param;
	}
}

struct starpu_top_param* starpu_top_register_parameter_boolean(const char* param_name,
							       int* parameter_field,
							       void (*callback)(struct starpu_top_param*))
{
	STARPU_ASSERT(!_starpu_top_status_get());
	struct starpu_top_param * param = (struct starpu_top_param *) malloc(sizeof(struct starpu_top_param));
	param->callback = callback;
	param->name = param_name;
	param->id = starpu_top_param_cpt++;
	param->type = STARPU_TOP_PARAM_BOOLEAN;
	param->value = (void*)parameter_field;
	param->next = NULL;

	starpu_top_enqueue_param(param);

	return param;
}

struct starpu_top_param* starpu_top_register_parameter_integer(const char* param_name,
							       int* parameter_field,
							       int minimum_value,
							       int maximum_value,
							       void (*callback)(struct starpu_top_param*))
{
	STARPU_ASSERT(!_starpu_top_status_get());
	struct starpu_top_param * param = (struct starpu_top_param *) malloc(sizeof(struct starpu_top_param));
	param->callback = callback;
	param->name = param_name;
	param->id = starpu_top_param_cpt++;
	param->type = STARPU_TOP_PARAM_INTEGER;
	param->value = (void*)parameter_field;
	param->int_min_value = minimum_value;
	param->int_max_value = maximum_value;
	param->next = NULL;

	starpu_top_enqueue_param(param);

	return param;
}

struct starpu_top_param* starpu_top_register_parameter_float(const char* param_name,
							     double* parameter_field,
							     double minimum_value,
							     double maximum_value,
							     void (*callback)(struct starpu_top_param*))
{
	STARPU_ASSERT(!_starpu_top_status_get());
	struct starpu_top_param * param = (struct starpu_top_param *) malloc(sizeof(struct starpu_top_param));
	param->callback = callback;
	param->name = param_name;
	param->id = starpu_top_param_cpt++;
	param->type = STARPU_TOP_PARAM_FLOAT;
	param->value = (void*)parameter_field;
	param->double_min_value = minimum_value;
	param->double_max_value = maximum_value;
	param->next = NULL;

	starpu_top_enqueue_param(param);

	return param;
}

struct starpu_top_param* starpu_top_register_parameter_enum(const char* param_name,
							    int* parameter_field,
							    char** values,
							    int nb_values,
							    void (*callback)(struct starpu_top_param*))
{
	STARPU_ASSERT(!_starpu_top_status_get());
	struct starpu_top_param * param = (struct starpu_top_param *) malloc(sizeof(struct starpu_top_param));
	param->callback = callback;
	param->name = param_name;
	param->id = starpu_top_param_cpt++;
	param->type = STARPU_TOP_PARAM_ENUM;
	param->value = (void*)parameter_field;
	param->enum_values = values;
	param->nb_values = nb_values;
	param->next = NULL;

	starpu_top_enqueue_param(param);

	return param;
}
/*********************************************
*****************UPDATE FUNC******************
**********************************************/

void starpu_top_update_data_boolean(const struct starpu_top_data* data, int value)
{
	if (!_starpu_top_status_get())
		return;
	if(data->active)
	{
		char*message = (char *) malloc(256+strlen(data->name));
		sprintf(message,
				"U;%u;%d;%lld\n",
				data->id,
				(value?1:0),
				current_timestamp());
		_starpu_top_message_add(_starpu_top_mt,message);
	}
}

void starpu_top_update_data_integer(const struct starpu_top_data* data, int value)
{
	if (!_starpu_top_status_get())
		return;
	if(data->active)
	{
		char*message = (char *) malloc(256+strlen(data->name));
		sprintf(message,
				"U;%u;%d;%lld\n",
				data->id,
				value,
				current_timestamp());
		_starpu_top_message_add(_starpu_top_mt,message);
	}
}

void starpu_top_update_data_float(const struct starpu_top_data* data, double value)
{
	if (!_starpu_top_status_get())
		return;
	if(data->active)
	{
		char*message = (char *) malloc(256+strlen(data->name));
		sprintf(message,
				"U;%u;%f;%lld\n",
				data->id, value,
				current_timestamp());
		_starpu_top_message_add(_starpu_top_mt,message);
	}
}

void starpu_top_update_parameter(const struct starpu_top_param* param)
{
	if (!_starpu_top_status_get())
		return;
	char*message = (char *) malloc(50);

	switch(param->type)
	{
		case STARPU_TOP_PARAM_BOOLEAN:
		case STARPU_TOP_PARAM_INTEGER:
		case STARPU_TOP_PARAM_ENUM:
			sprintf(message,
					"SET;%u;%d;%lld\n",
					param->id,
					*((int*)param->value),
					current_timestamp());
			break;

		case STARPU_TOP_PARAM_FLOAT:
			sprintf(message,
					"SET;%u;%f;%lld\n",
					param->id,
					*((double*)param->value),
					current_timestamp());
			break;
	}

	_starpu_top_message_add(_starpu_top_mt,message);
}

/*********************************************
*****************DEBUG FUNC******************
**********************************************/

void starpu_top_debug_log(const char* debug_message)
{
	if(starpu_top_debug_on)
	{
		//length can be up to strlen*2, if message contains only unwanted chars
		char * message = (char *) malloc(strlen(debug_message)*2+16);
		sprintf(message,"MESSAGE;");

		//escape unwanted char : ; and \n
		char* cur = message+8;
		while(*debug_message!='\0')
		{
			if(*debug_message=='\n' || *debug_message==';')
			{
				*cur='\\';
				cur++;
			}
			*cur = *debug_message;
			cur++;
			debug_message++;
		}
		*cur='\n';
		cur++;
		*cur='\0';

		_starpu_top_message_add(_starpu_top_mt,message);
	}
}

void starpu_top_debug_lock(const char* debug_message)
{
	if(starpu_top_debug_on)
	{
		char * message = (char *) malloc(strlen(debug_message)*2+16);
		sprintf(message,"LOCK;");
		char* cur = message+5;
		while(*debug_message!='\0')
		{
			if(*debug_message=='\n' || *debug_message==';')
			{
				*cur='\\';
				cur++;
			}
			*cur = *debug_message;
			cur++;
			debug_message++;
		}
		*cur='\n';
		*(cur+1)='\0';

		_starpu_top_message_add(_starpu_top_mt,message);

		//This threads keeps locked while we don't receive an STEP message
		_STARPU_PTHREAD_MUTEX_LOCK(&starpu_top_wait_for_continue_mutex);
		_STARPU_PTHREAD_COND_WAIT(&starpu_top_wait_for_continue_cond,
					  &starpu_top_wait_for_continue_mutex);
		_STARPU_PTHREAD_MUTEX_UNLOCK(&starpu_top_wait_for_continue_mutex);
	}
}



/********************************************
 **************TIME FUNCTION****************
 *******************************************/

unsigned long long current_timestamp()
{
	struct timespec now;
	_starpu_clock_gettime(&now);
	return _starpu_top_timing_timespec_to_ms(&now);
}

unsigned long long _starpu_top_timing_timespec_to_ms(const struct timespec *ts)
{
	return (1000.0*ts->tv_sec) + (0.000001*ts->tv_nsec);
}

/********************************************
 **************INPUT PROCESSING**************
 *******************************************/

static
enum starpu_top_message_type starpu_top_get_message_type(const char* message)
{
	if(!strncmp("GO\n", message,3))
		return TOP_TYPE_GO;
	else if(!strncmp("SET;", message,4))
		return TOP_TYPE_SET;
	else if(!strncmp("STEP\n", message,9))
		return TOP_TYPE_CONTINUE;
	else if(!strncmp("ENABLE;", message,7))
		return TOP_TYPE_ENABLE;
	else if(!strncmp("DISABLE;", message,8))
		return TOP_TYPE_DISABLE;
	else if(!strncmp("DEBUG;", message,6))
		return TOP_TYPE_DEBUG;
	else
		return TOP_TYPE_UNKNOW;
}

static
void starpu_top_unlock_starpu()
{
	sem_post(&starpu_top_wait_for_go);
	printf("%s:%d starpu started\n", __FILE__, __LINE__);
}

static
void starpu_top_change_data_active(char* message, int active)
{
	char* debut = strstr(message, ";")+1;
	char* fin = strstr(debut+1, "\n");
	*fin = '\0';
	int data_id = atoi(debut);
	printf("%s:%d data %d %s\n", __FILE__, __LINE__, data_id, active ? "ENABLED" : "DISABLE");
	starpu_top_datas[data_id]->active = active;
}

static
void starpu_top_change_parameter_value(const char* message)
{
	const char*tmp = strstr(message, ";")+1;
	int param_id = atoi(tmp);
	struct starpu_top_param* param = starpu_top_params[param_id];
	tmp = strstr(tmp+1,";")+1;
	int* val_ptr_int;
	double* val_ptr_double;

	switch(param->type)
	{
		case STARPU_TOP_PARAM_BOOLEAN:
		case STARPU_TOP_PARAM_INTEGER:
			val_ptr_int = (int*)param->value;
			*val_ptr_int = atoi(tmp);
		break;

		case STARPU_TOP_PARAM_FLOAT:
			val_ptr_double = (double*)param->value;
			*val_ptr_double = atof(tmp);
		break;

		case STARPU_TOP_PARAM_ENUM:
			val_ptr_int = (int*)param->value;
			*val_ptr_int = atoi(tmp);
		break;

	}
	if(param->callback != NULL)
		param->callback(param);
}

static
void starpu_top_change_debug_mode(const char*message)
{
	const char* debut = strstr(message, ";")+1;
	if(!strncmp("ON",debut, 2))
	{
		starpu_top_debug_on = 1;
		printf("%s:%d debug is now ON\n", __FILE__, __LINE__);
	}
	else
	{
		starpu_top_debug_on = 0;
		printf("%s:%d debug is now OFF\n", __FILE__, __LINE__);
	}

	char * m = (char *) malloc(strlen(message)+1);
	sprintf(m,"%s",message);
	_starpu_top_message_add(_starpu_top_mt,m);
}

/*
 * Unlock starpu if it was locked in debug state
*/
static
void starpu_top_debug_next_step()
{
	_STARPU_PTHREAD_COND_SIGNAL(&starpu_top_wait_for_continue_cond);
}


void _starpu_top_process_input_message(char *buffer)
{
	enum starpu_top_message_type message_type = starpu_top_get_message_type(buffer);
	switch(message_type)
	{
		case TOP_TYPE_GO:
			starpu_top_unlock_starpu();
		break;
		case TOP_TYPE_ENABLE:
			starpu_top_change_data_active(buffer, 1);
		break;
		case TOP_TYPE_DISABLE:
			starpu_top_change_data_active(buffer, 0);
		break;
		case TOP_TYPE_SET:
			starpu_top_change_parameter_value(buffer);
		break;
		case TOP_TYPE_DEBUG:
			starpu_top_change_debug_mode(buffer);
		break;
		case TOP_TYPE_CONTINUE:
			starpu_top_debug_next_step();
		break;
		default:
			printf("%s:%d unknow message : '%s'\n", __FILE__, __LINE__, buffer);
	}
}


