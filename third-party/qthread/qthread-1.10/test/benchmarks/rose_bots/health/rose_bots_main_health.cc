/**********************************************************************************************/
/*  This program is part of the Barcelona OpenMP Tasks Suite                                  */
/*  Copyright (C) 2009 Barcelona Supercomputing Center - Centro Nacional de Supercomputacion  */
/*  Copyright (C) 2009 Universitat Politecnica de Catalunya                                   */
/*                                                                                            */
/*  This program is free software; you can redistribute it and/or modify                      */
/*  it under the terms of the GNU General Public License as published by                      */
/*  the Free Software Foundation; either version 2 of the License, or                         */
/*  (at your option) any later version.                                                       */
/*                                                                                            */
/*  This program is distributed in the hope that it will be useful,                           */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of                            */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                             */
/*  GNU General Public License for more details.                                              */
/*                                                                                            */
/*  You should have received a copy of the GNU General Public License                         */
/*  along with this program; if not, write to the Free Software                               */
/*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA            */
/**********************************************************************************************/
/***********************************************************************
 * main function & common behaviour of the benchmark.
 **********************************************************************/
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <math.h>
#include <stddef.h>
#include <memory.h>
#include <sys/time.h>
#include <libgen.h>
#include "bots_common.h"
#include "bots_main.h"
#include "bots.h"
#include "app-desc.h"
/***********************************************************************
 * BENCHMARK HEADERS 
 *********************************************************************/
#include "libxomp.h" 
extern void bots_initialize();
extern void bots_finalize();
extern void bots_sequential_ini();
extern long bots_sequential();
extern void bots_sequential_fini();
extern int bots_check_result();
extern void bots_print_usage_specific();
extern void bots_get_params_specific(int argc,char **argv);
extern void bots_set_info();
/***********************************************************************
 * DEFAULT VALUES 
 *********************************************************************/
/* common flags */
int bots_sequential_flag = 0;
int bots_check_flag = 0;
bots_verbose_mode_t bots_verbose_mode = BOTS_VERBOSE_DEFAULT;
int bots_result = 3;
int bots_output_format = 1;
int bots_print_header = 0;
/* common variables */
char bots_name[128UL];
char bots_execname[128UL];
char bots_parameters[128UL];
char bots_model[128UL];
char bots_resources[128UL];
/* compile and execution information */
char bots_exec_date[128UL];
char bots_exec_message[128UL];
char bots_comp_date[128UL];
char bots_comp_message[128UL];
char bots_cc[512UL];
char bots_cflags[128UL];
char bots_ld[512UL];
char bots_ldflags[128UL];
char bots_cutoff[128UL];
/* time variables */
double bots_time_program = 0.0;
double bots_time_sequential = 0.0;
int bots_number_of_tasks = 0;
/*
 * Application dependent info
 */
#ifndef BOTS_APP_NAME
#error "Application name must be defined (#define BOTS_APP_NAME)"
#endif
#ifndef BOTS_APP_PARAMETERS_DESC
#define BOTS_APP_PARAMETERS_DESC ""
#endif
#ifndef BOTS_APP_PARAMETERS_LIST
#define BOTS_APP_PARAMETERS_LIST
#endif
#ifndef BOTS_APP_INIT
#define BOTS_APP_INIT
#endif
#ifndef BOTS_APP_FINI
#define BOTS_APP_FINI
#endif
#ifndef KERNEL_CALL
#error "Initial kernell call must be specified (#define KERNEL_CALL)"
#endif
#ifndef KERNEL_INIT
#define KERNEL_INIT
#endif
#ifndef KERNEL_FINI
#define KERNEL_FINI
#endif
#ifndef KERNEL_SEQ_INIT
#define KERNEL_SEQ_INIT
#endif
#ifndef KERNEL_SEQ_FINI
#define KERNEL_SEQ_FINI
#endif
#ifndef BOTS_MODEL_DESC
#define BOTS_MODEL_DESC "Unknown"
#endif
#ifdef BOTS_APP_USES_ARG_SIZE
#ifndef BOTS_APP_DEF_ARG_SIZE
#error "Default vaule for argument size must be specified (#define BOTS_APP_DEF_ARG_SIZE)"
#endif
#ifndef BOTS_APP_DESC_ARG_SIZE
#error "Help description for argument size must be specified (#define BOTS_APP_DESC_ARG_SIZE)"
#endif
#endif
#ifdef BOTS_APP_USES_ARG_SIZE_1
#ifndef BOTS_APP_DEF_ARG_SIZE_1
#error "Default vaule for argument size must be specified (#define BOTS_APP_DEF_ARG_SIZE_1)"
#endif
#ifndef BOTS_APP_DESC_ARG_SIZE_1
#error "Help description for argument size must be specified (#define BOTS_APP_DESC_ARG_SIZE_1)"
#endif
#endif
#ifdef BOTS_APP_USES_ARG_SIZE_2
#ifndef BOTS_APP_DEF_ARG_SIZE_2
#error "Default vaule for argument size must be specified (#define BOTS_APP_DEF_ARG_SIZE_2)"
#endif
#ifndef BOTS_APP_DESC_ARG_SIZE_2
#error "Help description for argument size must be specified (#define BOTS_APP_DESC_ARG_SIZE_2)"
#endif
#endif
#ifdef BOTS_APP_USES_ARG_REPETITIONS
#ifndef BOTS_APP_DEF_ARG_REPETITIONS
#error "Default vaule for argument repetitions must be specified (#define BOTS_APP_DEF_ARG_REPETITIONS)"
#endif
#ifndef BOTS_APP_DESC_ARG_REPETITIONS
#error "Help description for argument repetitions must be specified (#define BOTS_APP_DESC_ARG_REPETITIONS)"
#endif
#endif
#ifdef BOTS_APP_USES_ARG_FILE
#ifndef BOTS_APP_DESC_ARG_FILE
#error "Help description for argument file must be specified (#define BOTS_APP_DESC_ARG_FILE)"
#endif
char bots_arg_file[255UL] = "";
#endif
#ifdef BOTS_APP_USES_ARG_BLOCK
#ifndef BOTS_APP_DEF_ARG_BLOCK
#error "Default value for argument block must be specified (#define BOTS_APP_DEF_ARG_BLOCK)"
#endif
#ifndef BOTS_APP_DESC_ARG_BLOCK
#error "Help description for argument block must be specified (#define BOTS_APP_DESC_ARG_BLOCK)"
#endif
#endif
#ifdef BOTS_APP_USES_ARG_CUTOFF
#ifndef BOTS_APP_DEF_ARG_CUTOFF
#error "Default value for argument cutoff  must be specified (#define BOTS_APP_DEF_ARG_CUTOFF)"
#endif
#ifndef BOTS_APP_DESC_ARG_CUTOFF
#error "Help description for argument cutoff must be specified (#define BOTS_APP_DESC_ARG_CUTOFF)"
#endif
#endif
#ifdef BOTS_APP_USES_ARG_CUTOFF_1
#ifndef BOTS_APP_DEF_ARG_CUTOFF_1
#error "Default value for argument cutoff  must be specified (#define BOTS_APP_DEF_ARG_CUTOFF_1)"
#endif
#ifndef BOTS_APP_DESC_ARG_CUTOFF_1
#error "Help description for argument cutoff must be specified (#define BOTS_APP_DESC_ARG_CUTOFF_1)"
#endif
#endif
#ifdef BOTS_APP_USES_ARG_CUTOFF_2
#ifndef BOTS_APP_DEF_ARG_CUTOFF_2
#error "Default value for argument cutoff  must be specified (#define BOTS_APP_DEF_ARG_CUTOFF_2)"
#endif
#ifndef BOTS_APP_DESC_ARG_CUTOFF_2
#error "Help description for argument cutoff must be specified (#define BOTS_APP_DESC_ARG_CUTOFF_2)"
#endif
#endif
#if defined(MANUAL_CUTOFF) || defined(IF_CUTOFF) || defined(FINAL_CUTOFF)
int bots_cutoff_value = 2;
#endif
/***********************************************************************
 * print_usage: 
 **********************************************************************/

void bots_print_usage()
{
  fprintf(stderr,"\n");
  fprintf(stderr,"Usage: %s -[options]\n",bots_execname);
  fprintf(stderr,"\n");
  fprintf(stderr,"Where options are:\n");
#ifdef BOTS_APP_USES_REPETITIONS
#endif
#ifdef BOTS_APP_USES_ARG_SIZE
#endif
#ifdef BOTS_APP_USES_ARG_SIZE_1
#endif
#ifdef BOTS_APP_USES_ARG_SIZE_2
#endif
#ifdef BOTS_APP_USES_ARG_FILE
  fprintf(stderr,"  -f <file>  : Health input file (mandatory)\n");
#endif
#if defined(MANUAL_CUTOFF) || defined(IF_CUTOFF) || defined(FINAL_CUTOFF)
  fprintf(stderr,"  -x <value> : OpenMP tasks cut-off value (default=%d)\n",2);
#endif
#ifdef BOTS_APP_USES_ARG_CUTOFF
#endif
#ifdef BOTS_APP_USES_ARG_CUTOFF_1
#endif
#ifdef BOTS_APP_USES_ARG_CUTOFF_2
#endif
  fprintf(stderr,"\n");
  fprintf(stderr,"  -e <str>   : Include \'str\' execution message.\n");
  fprintf(stderr,"  -v <level> : Set verbose level (default = 1).\n");
  fprintf(stderr,"               0 - none.\n");
  fprintf(stderr,"               1 - default.\n");
  fprintf(stderr,"               2 - debug.\n");
  fprintf(stderr,"  -o <value> : Set output format mode (default = 1).\n");
  fprintf(stderr,"               0 - no benchmark output.\n");
  fprintf(stderr,"               1 - detailed list format.\n");
  fprintf(stderr,"               2 - detailed row format.\n");
  fprintf(stderr,"               3 - abridged list format.\n");
  fprintf(stderr,"               4 - abridged row format.\n");
  fprintf(stderr,"  -z         : Print row header (if output format is a row variant).\n");
  fprintf(stderr,"\n");
#ifdef KERNEL_SEQ_CALL
#endif
#ifdef BOTS_APP_CHECK_USES_SEQ_RESULT
#else
  fprintf(stderr,"  -c         : Check mode ON.\n");
#endif
  fprintf(stderr,"\n");
  fprintf(stderr,"  -h         : Print program\'s usage (this help).\n");
  fprintf(stderr,"\n");
}
/***********************************************************************
 * bots_get_params_common: 
 **********************************************************************/

void bots_get_params_common(int argc,char **argv)
{
  int i;
  strcpy(bots_execname,((strrchr(argv[0],'/')+1)));
  bots_get_date(bots_exec_date);
  strcpy(bots_exec_message,"");
  for (i = 1; i < argc; i++) {
    if ((argv[i][0]) == ('-')) {
      switch(argv[i][1]){
#ifdef BOTS_APP_USES_ARG_CUTOFF_1
#endif
#ifdef BOTS_APP_USES_ARG_CUTOFF_2
#endif
/* set/unset check mode */
        case 'c':
{
          argv[i][1] = '*';
//i++;
//if (argc == i) { bots_print_usage(); exit(100); }
//bots_check_flag = atoi(argv[i]);
          bots_check_flag = 1;
          break; 
        }
/* include execution message */
        case 'e':
{
          argv[i][1] = '*';
          i++;
          if (argc == i) {
            bots_print_usage();
            exit(100);
          }
          strcpy(bots_exec_message,(argv[i]));
          break; 
        }
#ifdef BOTS_APP_USES_ARG_FILE
/* read argument file name */
        case 'f':
{
          argv[i][1] = '*';
          i++;
          if (argc == i) {
            bots_print_usage();
            exit(100);
          }
          strcpy(bots_arg_file,(argv[i]));
          break; 
        }
#endif
/* print usage */
        case 'h':
{
          argv[i][1] = '*';
          bots_print_usage();
          exit(100);
        }
#ifdef BOTS_APP_USES_ARG_SIZE_2
/* read argument size 2 */
#endif
#ifdef BOTS_APP_USES_ARG_SIZE_1
/* read argument size 1 */
#endif
#ifdef BOTS_APP_USES_ARG_SIZE
/* read argument size 0 */
#endif
#ifdef BOTS_APP_USES_ARG_BLOCK
/*TODO*/
#endif
/* set output mode */
        case 'o':
{
          argv[i][1] = '*';
          i++;
          if (argc == i) {
            bots_print_usage();
            exit(100);
          }
          bots_output_format = atoi((argv[i]));
          break; 
        }
#ifdef BOTS_APP_USES_REPETITIONS
/* set number of repetitions */
#endif
#ifdef KERNEL_SEQ_CALL
/* set sequential execution */
//i++;
//if (argc == i) { bots_print_usage(); exit(100); }
//bots_sequential_flag = atoi(argv[i]);
#endif
/* set/unset verbose level */
        case 'v':
{
          argv[i][1] = '*';
          i++;
          if (argc == i) {
            bots_print_usage();
            exit(100);
          }
          bots_verbose_mode = ((bots_verbose_mode_t )(atoi((argv[i]))));
#ifndef BOTS_DEBUG
          if ((bots_verbose_mode) > 1) {
            fprintf(stderr,"Error: Configure the suite using \'--debug\' option in order to use a verbose level greather than 1.\n");
            exit(100);
          }
#endif
          break; 
        }
#if defined(MANUAL_CUTOFF) || defined(IF_CUTOFF) || defined(FINAL_CUTOFF)
        case 'x':
{
          argv[i][1] = '*';
          i++;
          if (argc == i) {
            bots_print_usage();
            exit(100);
          }
          bots_cutoff_value = atoi((argv[i]));
          break; 
        }
#endif
#ifdef BOTS_APP_USES_ARG_CUTOFF
#endif
        case 'z':
{
          argv[i][1] = '*';
          bots_print_header = 1;
          break; 
        }
        default:
{
// As at the moment there are only common paramenters
// we launch an error. Otherwise we have to ignore the
// parameter and to check, after specific parameters are
// completely read, if there are unrecognized parameters.
          fprintf(stderr,"Error: Unrecognized parameter.\n");
          bots_print_usage();
          exit(100);
        }
      }
    }
    else {
// As at the moment there are only common paramenters
// we launch an error. Otherwise we have to ignore the
// parameter and to check, after specific parameters are
// completely read, if there are unrecognized parameters.
      fprintf(stderr,"Error: Unrecognized parameter.\n");
      bots_print_usage();
      exit(100);
    }
  }
}
/***********************************************************************
 * bots_get_params_common: 
 **********************************************************************/

void bots_get_params(int argc,char **argv)
{
  bots_get_params_common(argc,argv);
//   bots_get_params_specific(argc, argv);
}
/***********************************************************************
 * bots_set_info 
 **********************************************************************/

void bots_set_info()
{
/* program specific info */
  sprintf(bots_name,"Health");
  sprintf(bots_parameters,"%s",bots_arg_file);
  sprintf(bots_model,"OpenMP (using tied tasks)");
  sprintf(bots_resources,"%d",omp_get_max_threads());
/* compilation info (do not modify) */
  strcpy(bots_comp_date,"2011/08/11;14:01");
  strcpy(bots_comp_message,"");
  strcpy(bots_cc,"~akp/shared/install/rose/bin/roseCompiler -I/hpc_shared/apps/RHEL-5/x86_64/rose/rose-0.9.5a-10757_gcc-4.4.4/include/hpc_shared/apps/RHEL-5/x86_64/gcc/gcc-4.4.4/bin/gcc_HEADERS/ -I/hpc_shared/apps/RHEL-5/x86_64/rose/rose-0.9.5a-10757_gcc-4.4.4/include -I. -rose:Cxx -rose:OpenMP:lowering -L/hpc_shared/apps/RHEL-5/x86_64/rose/rose-0.9.5a-10757_gcc-4.4.4/lib -L/home/olivier/shared/install/qthread/lib -lqthread -lqthread_xomp -g -fopenmp");
  strcpy(bots_cflags,"-c -I../..//common -O2   -DFORCE_TIED_TASKS -DMANUAL_CUTOFF -I.");
  strcpy(bots_ld,"~akp/shared/install/rose/bin/roseCompiler -I/hpc_shared/apps/RHEL-5/x86_64/rose/rose-0.9.5a-10757_gcc-4.4.4/include/hpc_shared/apps/RHEL-5/x86_64/gcc/gcc-4.4.4/bin/gcc_HEADERS/ -I/hpc_shared/apps/RHEL-5/x86_64/rose/rose-0.9.5a-10757_gcc-4.4.4/include -I. -rose:Cxx -rose:OpenMP:lowering -L/hpc_shared/apps/RHEL-5/x86_64/rose/rose-0.9.5a-10757_gcc-4.4.4/lib -L/home/olivier/shared/install/qthread/lib -lqthread -lqthread_xomp -g -fopenmp");
  strcpy(bots_ldflags,"-O2   -lm");
#if defined(MANUAL_CUTOFF) 
  sprintf(bots_cutoff,"manual (%d)",bots_cutoff_value);
#elif defined(IF_CUTOFF) 
#elif defined(FINAL_CUTOFF)
#else
#endif
}
/***********************************************************************
 * main: 
 **********************************************************************/

int main(int argc,char *argv[])
{
  int status = 0;
  XOMP_init(argc,argv);
#ifndef BOTS_APP_SELF_TIMING
  long bots_t_start;
  long bots_t_end;
#endif
  bots_get_params(argc,argv);
  struct Village *top;
  read_input_data(bots_arg_file);
  bots_set_info();
#ifdef KERNEL_SEQ_CALL
#ifdef BOTS_APP_CHECK_USES_SEQ_RESULT
#else
#endif
#ifdef BOTS_APP_SELF_TIMING
#else
#endif
#endif
  allocate_village(&top,((0L)),((0L)),sim_level,0);
#ifdef BOTS_APP_SELF_TIMING
#else
  bots_t_start = bots_usecs();
  sim_village_main_par(top);
  bots_t_end = bots_usecs();
  bots_time_program = (((double )(bots_t_end - bots_t_start)) / (1000000));
#endif
#ifdef KERNEL_CHECK
  if (bots_check_flag) {
    bots_result = check_village(top);
  }
#endif
  bots_print_results();
  XOMP_terminate(status);
  return 0;
}
