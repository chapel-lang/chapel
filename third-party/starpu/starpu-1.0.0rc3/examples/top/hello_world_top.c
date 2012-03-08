/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010  Université de Bordeaux 1
 * Copyright (C) 2010, 2011, 2012  Centre National de la Recherche Scientifique
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

/*
 * This examples demonstrates how to construct and submit a task to StarPU and
 * more precisely:
 *  - how to allocate a new task structure (starpu_task_create)
 *  - how to describe a multi-versionned computational kernel (ie. a codelet)
 *  - how to pass an argument to the codelet (task->cl_arg)
 *  - how to declare a callback function that is called once the task has been
 *    executed
 *  - how to specify if starpu_task_submit is a blocking or non-blocking
 *    operation (task->synchronous)
 */

#include <stdio.h>
#include <stdint.h>
#include <starpu.h>
#include <starpu_top.h>
#include <stdlib.h>
#include <time.h>

/* Example of enum param */
char* names[] = {"Paul", "Jean", "Jaques", "Alain", "Brian"};
int names_len = 5;
int name_selected=2; //must be between 0 and names_len-1
/* Exemple of int param */
int number_of_addition = 30;
/* Exemple of bool param */
int stop_after_5_task = 0;

/* When the task is done, task->callback_func(task->callback_arg) is called. Any
 * callback function must have the prototype void (*)(void *).
 * NB: Callback are NOT allowed to perform potentially blocking operations */
void callback_func(void *callback_arg)
{
	printf("Callback function got argument %p\n", callback_arg);
}

/* Every implementation of a codelet must have this prototype, the first
 * argument (buffers) describes the buffers/streams that are managed by the
 * DSM; the second arguments references read-only data that is passed as an
 * argument of the codelet (task->cl_arg). Here, "buffers" is unused as there
 * are no data input/output managed by the DSM (cl.nbuffers = 0) */
struct params
{
	int i;
	float f;
};

void cpu_func(void *buffers[], void *cl_arg)
{
	struct params *params = (struct params *) cl_arg;

	//loosing time for top example...
	int sum = 0;
	int i = 0;
	while(i<number_of_addition*1000000)
	{
		sum+=rand();
		i++;
	}

	printf("Hello %s (params = {%i, %f} ) sum=%d\n",
			names[name_selected],
			params->i,
			params->f,
			sum);
}

void callback_name_changed(struct starpu_top_param* param)
{
	char* message = (char *) malloc(256);
	sprintf(message, "Name have been changed to %s", names[name_selected]);
	starpu_top_debug_log(message);
}

void callback_number_addition_changed(struct starpu_top_param* param)
{
	char* message = (char *) malloc(256);
	sprintf(message, "Number of addition is now %d", number_of_addition);

	starpu_top_debug_log(message);
}

struct starpu_codelet cl =
{
	/* this codelet may only be executed on a CPU, and its cpu
 	 * implementation is function "cpu_func" */
	.where = STARPU_CPU,
	.cpu_funcs = {cpu_func, NULL},
	/* the codelet does not manipulate any data that is managed
	 * by our DSM */
	.nbuffers = 0
};

int main(int argc, char **argv)
{
	int ret;

	srand ( time(NULL) );
	/* initialize StarPU : passing a NULL argument means that we use
 	* default configuration for the scheduling policies and the number of
	* processors/accelerators */
	ret = starpu_init(NULL);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	/*init starpu_top*/
	struct starpu_top_data * loop_count =
		starpu_top_add_data_integer("Loop count", 0,124,1);
	struct starpu_top_data * remain_count =
		starpu_top_add_data_integer("Remaining loop", 0,124,1);
	struct starpu_top_data * midle_reach =
		starpu_top_add_data_boolean("Midle reached", 1);
	struct starpu_top_param* name =
		starpu_top_register_parameter_enum("Your name : ",
						   &name_selected,
						   names,
						   names_len,
						   callback_name_changed);
	struct starpu_top_param * number_of_addition_param =
		starpu_top_register_parameter_integer("Number of Millions of addition",
						      &number_of_addition,
						      0,
						      50,
						      callback_number_addition_changed);
	STARPU_ASSERT(number_of_addition_param != NULL);

	struct starpu_top_param * stop5_param =
		starpu_top_register_parameter_boolean("Stop after 5 task ?",
						      &stop_after_5_task,
						      NULL);
	STARPU_ASSERT(stop5_param != NULL);



	//all parameters are initialized, we can connect to UI
	starpu_top_init_and_wait("Serveur de test HelloWorld");

	//set "default value"
	starpu_top_update_data_boolean(midle_reach, 0);


	/* create a new task that is non-blocking by default : the task is not
	 * submitted to the scheduler until the starpu_task_submit function is
	 * called */

	/*
	 * For this simple example, we make 124 iter
	*/
	struct starpu_task *task[124];
	int i;
	for(i=0; i<124; i++)
	{
		starpu_top_update_data_integer(loop_count, i);
		starpu_top_update_data_integer(remain_count, 124-i);
		if(i==62)
		{
			starpu_top_update_data_boolean(midle_reach, 1);
		}
		if(i==25)
		{
			//changing name
			name_selected = 1;
			starpu_top_update_parameter(name);
		}
		if(i>4 && stop_after_5_task)
		{
			break;
		}

		task[i]=starpu_task_create();

		/* the task uses codelet "cl" */
		task[i]->cl = &cl;

		/* It is possible to pass buffers that are not managed by the DSM to the
		 * kernels: the second argument of the "cpu_func" function is a pointer to a
		 * buffer that contains information for the codelet (cl_arg stands for
		 * codelet argument). In the case of accelerators, it is possible that
		 * the codelet is given a pointer to a copy of that buffer: this buffer
		 * is read-only so that any modification is not passed to other copies
		 * of the buffer.  For this reason, a buffer passed as a codelet
		 * argument (cl_arg) is NOT a valid synchronization medium! */
		struct params params = { i, 2.0f };
		task[i]->cl_arg = &params;
		task[i]->cl_arg_size = sizeof(params);

		/* once the task has been executed, callback_func(0x42)
		 * will be called on a CPU */
		task[i]->callback_func = callback_func;
		task[i]->callback_arg = (void*) (uintptr_t) 0x42;

		/* starpu_task_submit will be a blocking call */
		task[i]->synchronous = 1;

		/* submit the task to StarPU */
		if(number_of_addition==42)
			starpu_top_debug_lock("debug stop point because of 42 !");

		ret = starpu_task_submit(task[i]);
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
	}
	/* terminate StarPU: statistics and other debug outputs are not
	 * guaranteed to be generated unless this function is called. Once it
	 * is called, it is not possible to submit tasks anymore, and the user
	 * is responsible for making sure all tasks have already been executed:
	 * calling starpu_shutdown() before the termination of all the tasks
	 * results in an undefined behaviour */

	starpu_shutdown();

	return 0;
}
