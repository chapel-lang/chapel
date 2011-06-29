/*************************************************************************************/
/*      Copyright 2009 Barcelona Supercomputing Center                               */
/*                                                                                   */
/*      This file is part of the NANOS++ library.                                    */
/*                                                                                   */
/*      NANOS++ is free software: you can redistribute it and/or modify              */
/*      it under the terms of the GNU Lesser General Public License as published by  */
/*      the Free Software Foundation, either version 3 of the License, or            */
/*      (at your option) any later version.                                          */
/*                                                                                   */
/*      NANOS++ is distributed in the hope that it will be useful,                   */
/*      but WITHOUT ANY WARRANTY; without even the implied warranty of               */
/*      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                */
/*      GNU Lesser General Public License for more details.                          */
/*                                                                                   */
/*      You should have received a copy of the GNU Lesser General Public License     */
/*      along with NANOS++.  If not, see <http://www.gnu.org/licenses/>.             */
/*************************************************************************************/

/*
<testinfo>
test_generator=gens/api-generator
</testinfo>
*/

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <nanos.h>

typedef struct {
   int* p_i;
} my_args;

typedef struct {
   int* p_i;
   int  index;
} my_args2;

typedef struct {
   int* p_i;
   int* p_result;
   int  index;
} my_args3;


void first(void *ptr)
{
   int *i = ((my_args *) ptr)->p_i;
   *i = 0;
#ifdef VERBOSE
   printf("first task: %x set to %d\n",(unsigned int)i,*i);
   fflush(stdout);
#endif
}

void second(void *ptr)
{
   int *i = ((my_args *) ptr)->p_i;
   (*i)++;
#ifdef VERBOSE
   printf("successor: %x set to %d\n",(unsigned int)i,*i);
   fflush(stdout);
#endif
}

void third(void *ptr)
{
   int j;
   for ( j = 0; j < 100; j++ ) {
      int *i = ((my_args *) ptr)[j].p_i;
      (*i)++;
   }
}

void fourth(void *ptr)
{
   int *i = ((my_args *) ptr)[0].p_i;
   int *j = ((my_args *) ptr)[1].p_i;
   (*j) = *i;
}

void fifth(void *ptr)
{
#ifdef VERBOSE
   printf("fifth\n");
   fflush(stdout);
#endif
   int *red_array = ((my_args2 *) ptr)->p_i;
   int index = ((my_args2 *) ptr)->index;
   red_array[index]++;
}

void sixth(void *ptr)
{
#ifdef VERBOSE
   printf("sixth\n");
   fflush(stdout);
#endif
   int i;
   int *red_array = ((my_args2 *) ptr)->p_i;
   int size = ((my_args2 *) ptr)->index;
   for ( i = 0; i < size; i++ )
   {
      red_array[i]++;
   }
}

void seventh(void *ptr)
{
#ifdef VERBOSE
   printf("seventh\n");
   fflush(stdout);
#endif
   int *array = ((my_args3 *) ptr)->p_i;
   int *result = ((my_args3 *) ptr)->p_result;
   int index = ((my_args3 *) ptr)->index;
   if ( array[index] != 2 ) {
      *result = -1;
   }
}

void eighth(void *ptr)
{
#ifdef VERBOSE
   printf("eighth\n");
   fflush(stdout);
#endif
}




nanos_smp_args_t test_device_arg_1 = { first };
nanos_smp_args_t test_device_arg_2 = { second };
nanos_smp_args_t test_device_arg_3 = { third };
nanos_smp_args_t test_device_arg_4 = { fourth };
nanos_smp_args_t test_device_arg_5 = { fifth };
nanos_smp_args_t test_device_arg_6 = { sixth };
nanos_smp_args_t test_device_arg_7 = { seventh };
nanos_smp_args_t test_device_arg_8 = { eighth };

bool single_dependency()
{
   int my_value;
   int * dep_addr = &my_value;
   my_args *args1=0;
   nanos_dependence_t deps1 = {(void **)&dep_addr,0, {0,1,0,0}, 0};
   nanos_wd_props_t props = {
     .mandatory_creation = true,
     .tied = false,
     .tie_to = false,
   };
   nanos_wd_t wd1=0;
   nanos_device_t test_devices_1[1] = { NANOS_SMP_DESC( test_device_arg_1) };
   NANOS_SAFE( nanos_create_wd ( &wd1, 1,test_devices_1, sizeof(my_args), __alignof__(my_args), (void**)&args1, nanos_current_wd(), &props, 0, NULL) );
   args1->p_i = dep_addr;
   NANOS_SAFE( nanos_submit( wd1,1,&deps1,0 ) );

   my_args *args2=0;
   nanos_dependence_t deps2 = {(void **)&dep_addr,0, {1,1,0,0}, 0};
   nanos_wd_t wd2 = 0;
   nanos_device_t test_devices_2[1] = { NANOS_SMP_DESC( test_device_arg_2 ) };
   NANOS_SAFE( nanos_create_wd ( &wd2, 1,test_devices_2, sizeof(my_args), __alignof__(my_args), (void**)&args2, nanos_current_wd(), &props, 0, NULL) );
   args2->p_i = dep_addr;
   NANOS_SAFE( nanos_submit( wd2,1,&deps2,0 ) );

   NANOS_SAFE( nanos_wg_wait_completion( nanos_current_wd() ) );
   
   return (my_value == 1);
}

bool single_inout_chain()
{
   int i;
   int my_value;
   int * dep_addr = &my_value;
   my_args *args1=0;
   nanos_dependence_t deps1 = {(void **)&dep_addr,0, {0,1,0,0}, 0};
   nanos_wd_props_t props = {
     .mandatory_creation = true,
     .tied = false,
     .tie_to = false,
   };
   nanos_wd_t wd1=0;
   nanos_device_t test_devices_1[1] = { NANOS_SMP_DESC( test_device_arg_1) };
   NANOS_SAFE( nanos_create_wd ( &wd1, 1,test_devices_1, sizeof(my_args), __alignof__(my_args), (void**)&args1, nanos_current_wd(), &props, 0, NULL) );
   args1->p_i = dep_addr;
   NANOS_SAFE( nanos_submit( wd1,1,&deps1,0 ) );

   for ( i = 0; i < 100; i++ ) {
      my_args *args2=0;
      nanos_dependence_t deps2 = {(void **)&dep_addr,0, {1,1,0,0}, 0};
      nanos_wd_t wd2 = 0;
      nanos_device_t test_devices_2[1] = { NANOS_SMP_DESC( test_device_arg_2 ) };
      NANOS_SAFE( nanos_create_wd ( &wd2, 1,test_devices_2, sizeof(my_args), __alignof__(my_args), (void**)&args2, nanos_current_wd(), &props, 0, NULL) );
      args2->p_i = dep_addr;
      NANOS_SAFE( nanos_submit( wd2,1,&deps2,0 ) );
   }

   NANOS_SAFE( nanos_wg_wait_completion( nanos_current_wd() ) );
   
   return (my_value == 100);
}

bool multiple_inout_chains()
{
   int i, j;
   int size = 10;
   int my_value[size];

   for ( i = 0; i < size; i++ ) {
      int * dep_addr = &my_value[i];
      my_args *args1=0;
      nanos_dependence_t deps1 = {(void **)&dep_addr,0, {0,1,0,0}, 0};
      nanos_wd_props_t props = {
        .mandatory_creation = true,
        .tied = false,
        .tie_to = false,
      };
      nanos_wd_t wd1=0;
      nanos_device_t test_devices_1[1] = { NANOS_SMP_DESC( test_device_arg_1) };
      NANOS_SAFE( nanos_create_wd ( &wd1, 1,test_devices_1, sizeof(my_args), __alignof__(my_args), (void**)&args1, nanos_current_wd(), &props, 0, NULL) );
      args1->p_i = dep_addr;
      NANOS_SAFE( nanos_submit( wd1,1,&deps1,0 ) );

      for ( j = 0; j < size; j++ ) {
         my_args *args2=0;
         nanos_dependence_t deps2 = {(void **)&dep_addr,0, {1,1,0,0}, 0};
         nanos_wd_t wd2 = 0;
         nanos_device_t test_devices_2[1] = { NANOS_SMP_DESC( test_device_arg_2 ) };
         NANOS_SAFE( nanos_create_wd ( &wd2, 1,test_devices_2, sizeof(my_args), __alignof__(my_args), (void**)&args2, nanos_current_wd(), &props, 0, NULL) );
         args2->p_i = dep_addr;
         NANOS_SAFE( nanos_submit( wd2,1,&deps2,0 ) );
      }
   }

   NANOS_SAFE( nanos_wg_wait_completion( nanos_current_wd() ) );

   for ( i = 0; i < size; i++ ) {
      if ( my_value[i] != size ) return false;
   }
   return true;
}

bool multiple_predecessors()
{
   int j;
   int size=100;
   int my_value[size];
   nanos_wd_props_t props = {
     .mandatory_creation = true,
     .tied = false,
     .tie_to = false,
   };

   for ( j = 0; j < size; j++ ) {
      int * dep_addr1 = &my_value[j];
      my_args *args1=0;
      nanos_dependence_t deps1 = {(void **)&dep_addr1,0, {0,1,0,0}, 0};
      nanos_wd_t wd1 = 0;

      nanos_device_t test_devices_1[1] = { NANOS_SMP_DESC( test_device_arg_1 ) };
      NANOS_SAFE( nanos_create_wd ( &wd1, 1,test_devices_1, sizeof(my_args), __alignof(my_args), (void**)&args1, nanos_current_wd(), &props, 0, NULL) );
      args1->p_i = dep_addr1;
      NANOS_SAFE( nanos_submit( wd1,1,&deps1,0 ) );
   }

   nanos_dependence_t deps2[size];
   int *dep_addr2[size];
   my_args *args2=0;
   for ( j = 0; j < size; j++ ) {
      dep_addr2[j] = &my_value[j];
      deps2[j] = (nanos_dependence_t){(void **) &dep_addr2[j],0, {1,1,0,0},0};
   }

   nanos_wd_t wd2=0;
   nanos_device_t test_devices_3[1] = { NANOS_SMP_DESC( test_device_arg_3) };
   NANOS_SAFE( nanos_create_wd ( &wd2, 1,test_devices_3, sizeof(my_args)*size, __alignof__(my_args), (void**)&args2, nanos_current_wd(), &props, 0, NULL) );
   for ( j = 0; j < size; j++)
      args2[j].p_i = dep_addr2[j];
   NANOS_SAFE( nanos_submit( wd2,size,&deps2[0],0 ) );

   NANOS_SAFE( nanos_wg_wait_completion( nanos_current_wd() ) );
   for ( j = 0; j < size; j++ ) {
      if ( my_value[j] != 1 ) return false;
   }
   return true;
}

bool multiple_antidependencies()
{
   int j;
   int my_value=1500;
   int my_reslt[100];
   nanos_wd_props_t props = {
     .mandatory_creation = true,
     .tied = false,
     .tie_to = false,
   };

   for ( j = 0; j < 100; j++ ) {
      int * dep_addr1 = &my_value;
      int * reslt_addr =&my_reslt[j];
      my_args *args1=0;
      nanos_dependence_t deps1 = {(void **)&dep_addr1,0, {1,0,0,0}, 0};

      nanos_wd_t wd1 = 0;
      nanos_device_t test_devices_4[1] = { NANOS_SMP_DESC( test_device_arg_4 ) };
      NANOS_SAFE( nanos_create_wd ( &wd1, 1,test_devices_4, sizeof(my_args)*2, __alignof__(my_args), (void**)&args1, nanos_current_wd(), &props, 0, NULL) );
      args1[0].p_i = dep_addr1;
      args1[1].p_i = reslt_addr;
      NANOS_SAFE( nanos_submit( wd1,1,&deps1,0 ) );
   }

   int *dep_addr2 = &my_value;
   nanos_dependence_t deps2 = (nanos_dependence_t){(void **) &dep_addr2,0, {1,1,0,0},0};
   my_args *args2=0;

   nanos_wd_t wd2=0;
   nanos_device_t test_devices_2[1] = { NANOS_SMP_DESC( test_device_arg_2) };
   NANOS_SAFE( nanos_create_wd ( &wd2, 1,test_devices_2, sizeof(my_args), __alignof__(my_args), (void**)&args2, nanos_current_wd(), &props, 0, NULL) );
   args2->p_i = dep_addr2;
   NANOS_SAFE( nanos_submit( wd2,1,&deps2,0 ) );

   NANOS_SAFE( nanos_wg_wait_completion( nanos_current_wd() ) );
   for ( j = 0; j < 100; j++ ) {
      if ( my_reslt[j] != 1500 ) return false;
   }
   if (my_value != 1501) return false;
   return true;
}

bool out_dep_chain()
{
   int i;
   int my_value;
   int * dep_addr = &my_value;
   nanos_wd_props_t props = {
     .mandatory_creation = true,
     .tied = false,
     .tie_to = false,
   };

   for ( i = 0; i < 100; i++ ) {
      my_args *args2=0;
      nanos_dependence_t deps2 = {(void **)&dep_addr,0, {0,1,0,0}, 0};
      nanos_wd_t wd2 = 0;
      nanos_device_t test_devices_1[1] = { NANOS_SMP_DESC( test_device_arg_1 ) };
      NANOS_SAFE( nanos_create_wd ( &wd2, 1,test_devices_1, sizeof(my_args), __alignof__(my_args),(void**)&args2, nanos_current_wd(), &props, 0, NULL) );
      args2->p_i = dep_addr;
      NANOS_SAFE( nanos_submit( wd2,1,&deps2,0 ) );
   }

   int input=500;
   int * input_addr = &input;
   nanos_dependence_t deps1 = {(void **)&dep_addr,0, {0,1,0,0}, 0};
   my_args *args1=0;
   nanos_wd_t wd1=0;
   nanos_device_t test_devices_1[1] = { NANOS_SMP_DESC( test_device_arg_4) };
   NANOS_SAFE( nanos_create_wd ( &wd1, 1,test_devices_1, sizeof(my_args)*2, __alignof__(my_args), (void**)&args1, nanos_current_wd(), &props, 0, NULL) );
   args1[0].p_i = input_addr;
   args1[1].p_i = dep_addr;
   NANOS_SAFE( nanos_submit( wd1,1,&deps1,0 ) );

   NANOS_SAFE( nanos_wg_wait_completion( nanos_current_wd() ) );
   
   return (my_value == 500);
}

bool wait_on_test()
{
   int j;
   int size=10;
   int my_value[size];
   nanos_wd_props_t props = {
     .mandatory_creation = true,
     .tied = false,
     .tie_to = false,
   };

   for ( j = 0; j < size; j++ ) {
      my_value[j] = 500;
      int * dep_addr1 = &my_value[j];
      my_args *args1=0;
      nanos_dependence_t deps1 = {(void **)&dep_addr1,0, {0,1,0,0}, 0};
      nanos_wd_t wd1 = 0;

      nanos_device_t test_devices_1[1] = { NANOS_SMP_DESC( test_device_arg_1 ) };
      NANOS_SAFE( nanos_create_wd ( &wd1, 1,test_devices_1, sizeof(my_args), __alignof__(my_args), (void**)&args1, nanos_current_wd(), &props, 0, NULL) );
      args1->p_i = dep_addr1;
      NANOS_SAFE( nanos_submit( wd1,1,&deps1,0 ) );
   }

   nanos_dependence_t deps2[size];
   int *dep_addr2[size];
   for ( j = 0; j < size; j++ ) {
      dep_addr2[j] = &my_value[j];
      deps2[j] = (nanos_dependence_t){(void **) &dep_addr2[j],0, {1,0,0,0},0};
   }
   
   NANOS_SAFE( nanos_wait_on( size, &deps2[0] ));

   for ( j = 0; j < size; j++ ) {
    if ( my_value[j] != 0 ) return false;
   }
   return true;
}

bool create_and_run_test()
{
   int j;
   int my_value[100];
   int other_value=0;
   nanos_wd_props_t props = {
     .mandatory_creation = true,
     .tied = false,
     .tie_to = false,
   };

   for ( j = 0; j < 100; j++ ) {
      my_value[j] = 500;
      int * dep_addr1 = &my_value[j];
      my_args *args1=0;
      nanos_dependence_t deps1 = {(void **)&dep_addr1,0, {0,1,0,0}, 0};
      nanos_wd_t wd1 = 0;

      nanos_device_t test_devices_1[1] = { NANOS_SMP_DESC( test_device_arg_1 ) };
      NANOS_SAFE( nanos_create_wd ( &wd1, 1,test_devices_1, sizeof(my_args), __alignof__(my_args), (void**)&args1, nanos_current_wd(), &props, 0, NULL) );
      args1->p_i = dep_addr1;
      NANOS_SAFE( nanos_submit( wd1,1,&deps1,0 ) );
   }

   nanos_dependence_t deps2[100];
   int *dep_addr2[100];
   for ( j = 0; j < 100; j++ ) {
      dep_addr2[j] = &my_value[j];
      deps2[j] = (nanos_dependence_t){(void **) &dep_addr2[j],0, {1,0,0,0},0};
   }

   my_args arg;
   arg.p_i = &other_value;
   nanos_device_t test_devices_2[1] = { NANOS_SMP_DESC( test_device_arg_1 ) };

   NANOS_SAFE( nanos_create_wd_and_run( 1, test_devices_2, sizeof(my_args), __alignof__(my_args),  (void *)&arg, 100, &deps2[0], &props , 0, NULL, NULL ) );

   for ( j = 0; j < 100; j++ ) {
    if ( my_value[j] != 0 ) return false;
   }
   return true;
}

// Test commutative tasks, this test creates a task with an inout dependency on an array an then
// a bunch of commutative (reduction) tasks that update it. Finally it waits for them all to finish and
// checks the result
bool commutative_task_1()
{
   int i, j;
   int size = 100;
   int my_value[size];
   int *value_ref = (int *)&my_value;

   for ( i = 0; i < size; i++ ) {
      my_value[i] = 0;
   }

   nanos_wd_props_t props = {
     .mandatory_creation = true,
     .tied = false,
     .tie_to = false,
   };

   my_args2 *args1=0;
   nanos_dependence_t deps1 = {(void **)&value_ref,0, {0,1,0,0}, 0};
   nanos_wd_t wd1 = 0;

   nanos_device_t test_devices_1[1] = { NANOS_SMP_DESC( test_device_arg_6 ) };
   NANOS_SAFE( nanos_create_wd ( &wd1, 1,test_devices_1, sizeof(my_args2), __alignof__(my_args2), (void**)&args1, nanos_current_wd(), &props, 0, NULL) );
   args1->p_i = my_value;
   args1->index = size;
   NANOS_SAFE( nanos_submit( wd1,1,&deps1,0 ) );

   for ( j = 0; j < size; j++ ) {
      my_args2 *args1=0;
      nanos_dependence_t deps1 = {(void **)&value_ref,0, {1,1,0,1}, 0};
      nanos_wd_t wd1 = 0;

      nanos_device_t test_devices_1[1] = { NANOS_SMP_DESC( test_device_arg_5 ) };
      NANOS_SAFE( nanos_create_wd ( &wd1, 1,test_devices_1, sizeof(my_args2), __alignof__(my_args2), (void**)&args1, nanos_current_wd(), &props, 0, NULL) );
      args1->p_i = my_value;
      args1->index = j;
      NANOS_SAFE( nanos_submit( wd1,1,&deps1,0 ) );
   }

   NANOS_SAFE( nanos_wg_wait_completion( nanos_current_wd() ) );
   for ( j = 0; j < 100; j++ ) {
      if ( my_value[j] != 2 ) return false;
   }
   return true;
}

// Test commutative tasks, this test creates a task with an inout dependency on an array an then
// a bunch of commutative (reduction) tasks that update it. Then, another set of tasks are successors
// of the commutative ones. This checks that the commutation task behaves correctly
bool commutative_task_2()
{
   int i, j;
   int size = 100;
   int my_value[size];
   int *value_ref = (int *)&my_value;
   int my_results[size];

   for ( i = 0; i < size; i++ ) {
      my_value[i] = 0;
      my_results[i] = 0;
   }

   nanos_wd_props_t props = {
     .mandatory_creation = true,
     .tied = false,
     .tie_to = false,
   };

   my_args2 *args1=0;
   nanos_dependence_t deps1 = {(void **)&value_ref,0, {0,1,0,0}, 0};
   nanos_wd_t wd1 = 0;

   nanos_device_t test_devices_1[1] = { NANOS_SMP_DESC( test_device_arg_6 ) };
   NANOS_SAFE( nanos_create_wd ( &wd1, 1,test_devices_1, sizeof(my_args2), __alignof__(my_args2), (void**)&args1, nanos_current_wd(), &props, 0, NULL) );
   args1->p_i = my_value;
   args1->index = size;
   NANOS_SAFE( nanos_submit( wd1,1,&deps1,0 ) );

   for ( j = 0; j < size; j++ ) {
      my_args2 *args1=0;
      nanos_dependence_t deps1 = {(void **)&value_ref,0, {1,1,0,1}, 0};
      nanos_wd_t wd1 = 0;

      nanos_device_t test_devices_1[1] = { NANOS_SMP_DESC( test_device_arg_5 ) };
      NANOS_SAFE( nanos_create_wd ( &wd1, 1,test_devices_1, sizeof(my_args2), __alignof__(my_args2), (void**)&args1, nanos_current_wd(), &props, 0, NULL) );
      args1->p_i = my_value;
      args1->index = j;
      NANOS_SAFE( nanos_submit( wd1,1,&deps1,0 ) );
   }

   for ( j = 0; j < size; j++ ) {
      my_args3 *args1=0;
      nanos_dependence_t deps1 = {(void **)&value_ref,0, {1,0,0,0}, 0};
      nanos_wd_t wd1 = 0;

      nanos_device_t test_devices_1[1] = { NANOS_SMP_DESC( test_device_arg_7 ) };
      NANOS_SAFE( nanos_create_wd ( &wd1, 1,test_devices_1, sizeof(my_args3), __alignof__(my_args3), (void**)&args1, nanos_current_wd(), &props, 0, NULL) );
      args1->p_i = my_value;
      args1->p_result = &my_results[j];
      args1->index = j;
      NANOS_SAFE( nanos_submit( wd1,1,&deps1,0 ) );
   }

   NANOS_SAFE( nanos_wg_wait_completion( nanos_current_wd() ) );

   for ( j = 0; j < size; j++ ) {
      if ( my_results[j] < 0 ) return false;
   }
   return true;
}

// Test commutative tasks, this test creates a task with an inout dependency on an array an then
// a bunch of tasks that read the dependency, then, again, a bunch of commutative (reduction) tasks 
// that update it. Then, another set of tasks are successors
// of the commutative ones. This checks that the commutation task behaves correctly
bool commutative_task_3()
{
   int i, j;
   int size = 100;
   int my_value[size];
   int *value_ref = (int *)&my_value;
   int my_results[size];

   for ( i = 0; i < size; i++ ) {
      my_value[i] = 0;
      my_results[i] = 0;
   }

   nanos_wd_props_t props = {
     .mandatory_creation = true,
     .tied = false,
     .tie_to = false,
   };

   my_args2 *args1=0;
   nanos_dependence_t deps1 = {(void **)&value_ref,0, {0,1,0,0}, 0};
   nanos_wd_t wd1 = 0;

   nanos_device_t test_devices_1[1] = { NANOS_SMP_DESC( test_device_arg_6 ) };
   NANOS_SAFE( nanos_create_wd ( &wd1, 1,test_devices_1, sizeof(my_args2), __alignof__(my_args2), (void**)&args1, nanos_current_wd(), &props, 0, NULL) );
   args1->p_i = my_value;
   args1->index = size;
   NANOS_SAFE( nanos_submit( wd1,1,&deps1,0 ) );

   for ( j = 0; j < size; j++ ) {
      my_args2 *args1=0;
      nanos_dependence_t deps1 = {(void **)&value_ref,0, {1,0,0,0}, 0};
      nanos_wd_t wd1 = 0;

      nanos_device_t test_devices_1[1] = { NANOS_SMP_DESC( test_device_arg_8 ) };
      NANOS_SAFE( nanos_create_wd ( &wd1, 1,test_devices_1, sizeof(my_args2), __alignof__(my_args2), (void**)&args1, nanos_current_wd(), &props, 0, NULL) );
      args1->p_i = my_value;
      args1->index = j;
      NANOS_SAFE( nanos_submit( wd1,1,&deps1,0 ) );
   }

   for ( j = 0; j < size; j++ ) {
      my_args2 *args1=0;
      nanos_dependence_t deps1 = {(void **)&value_ref,0, {1,1,0,1}, 0};
      nanos_wd_t wd1 = 0;

      nanos_device_t test_devices_1[1] = { NANOS_SMP_DESC( test_device_arg_5 ) };
      NANOS_SAFE( nanos_create_wd ( &wd1, 1,test_devices_1, sizeof(my_args2), __alignof__(my_args2), (void**)&args1, nanos_current_wd(), &props, 0, NULL) );
      args1->p_i = my_value;
      args1->index = j;
      NANOS_SAFE( nanos_submit( wd1,1,&deps1,0 ) );
   }

   for ( j = 0; j < size; j++ ) {
      my_args3 *args1=0;
      nanos_dependence_t deps1 = {(void **)&value_ref,0, {1,0,0,0}, 0};
      nanos_wd_t wd1 = 0;

      nanos_device_t test_devices_1[1] = { NANOS_SMP_DESC( test_device_arg_7 ) };
      NANOS_SAFE( nanos_create_wd ( &wd1, 1,test_devices_1, sizeof(my_args3), __alignof__(my_args3), (void**)&args1, nanos_current_wd(), &props, 0, NULL) );
      args1->p_i = my_value;
      args1->p_result = &my_results[j];
      args1->index = j;
      NANOS_SAFE( nanos_submit( wd1,1,&deps1,0 ) );
   }

   NANOS_SAFE( nanos_wg_wait_completion( nanos_current_wd() ) );

   for ( j = 0; j < size; j++ ) {
      if ( my_results[j] < 0 ) return false;
   }
   return true;
}



int main ( int argc, char **argv )
{
   printf("Single dependency test... \n");
   fflush(stdout);
   if ( single_dependency() ) {
      printf("PASS\n");
      fflush(stdout);
   } else {
      printf("FAIL\n");
      fflush(stdout);
   }
   
   printf("Single inout chain test... \n");
   fflush(stdout);
   if ( single_inout_chain() ) {
      printf("PASS\n");
      fflush(stdout);
   } else {
      printf("FAIL\n");
      fflush(stdout);
   }

   printf("Multiple inout chains test... \n");
   fflush(stdout);
   if ( multiple_inout_chains() ) {
      printf("PASS\n");
      fflush(stdout);
   } else {
      printf("FAIL\n");
      fflush(stdout);
      return 1;
   }

   printf("task with multiple predecessors... \n");
   fflush(stdout);
   if ( multiple_predecessors() ) {
      printf("PASS\n");
      fflush(stdout);
   } else {
      printf("FAIL\n");
      fflush(stdout);
      return 1;
   }
   printf("task with multiple anti-dependencies... \n");
   fflush(stdout);
   if ( multiple_antidependencies() ) {
      printf("PASS\n");
      fflush(stdout);
   } else {
      printf("FAIL\n");
      fflush(stdout);
      return 1;
   }

   printf("Out dependencies chain... \n");
   fflush(stdout);
   if ( out_dep_chain() ) {
      printf("PASS\n");
      fflush(stdout);
   } else {
      printf("FAIL\n");
      fflush(stdout);
      return 1;
   }

   printf("Wait on test...\n");
   fflush(stdout);
   if ( wait_on_test() ) {
      printf("PASS\n");
      fflush(stdout);
   } else {
      printf("FAIL\n");
      fflush(stdout);
      return 1;
   }

   printf("create and run test...\n");
   fflush(stdout);
   if ( create_and_run_test() ) {
      printf("PASS\n");
      fflush(stdout);
   } else {
      printf("FAIL\n");
      fflush(stdout);
      return 1;
   }

   printf("commutative tasks test...\n");
   fflush(stdout);
   if ( commutative_task_1() ) {
      printf("PASS\n");
      fflush(stdout);
   } else {
      printf("FAIL\n");
      fflush(stdout);
      return 1;
   }

   printf("commutative tasks 2 test...\n");
   fflush(stdout);
   if ( commutative_task_2() ) {
      printf("PASS\n");
      fflush(stdout);
   } else {
      printf("FAIL\n");
      fflush(stdout);
      return 1;
   }

   printf("commutative tasks 3 test...\n");
   fflush(stdout);
   if ( commutative_task_3() ) {
      printf("PASS\n");
      fflush(stdout);
   } else {
      printf("FAIL\n");
      fflush(stdout);
      return 1;
   }

   return 0;
}

