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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/utsname.h>
#include <sys/resource.h>
//#include <sys/loadavg.h> // KBW: commented out for OSX
#include "bots_common.h"
#include "bots_main.h"
#include "bots.h"
#include "libxomp.h" 

void bots_error(int error,char *message)
{
  if (message == ((0L))) {
    switch(error){
      case 0:
{
        fprintf(stderr,"Error (%d): %s\n",error,"Unspecified error.");
        break; 
      }
      case 1:
{
        fprintf(stderr,"Error (%d): %s\n",error,"Not enough memory.");
        break; 
      }
      case 2:
{
        fprintf(stderr,"Error (%d): %s\n",error,"Unrecognized parameter.");
        bots_print_usage();
        break; 
      }
      default:
{
        fprintf(stderr,"Error (%d): %s\n",error,"Invalid error code.");
        break; 
      }
    }
  }
  else 
    fprintf(stderr,"Error (%d): %s\n",error,message);
  exit((100 + error));
}

void bots_warning(int warning,char *message)
{
  if (message == ((0L))) {
    switch(warning){
      case 0:
{
        fprintf(stderr,"Warning (%d): %s\n",warning,"Unspecified warning.");
        break; 
      }
      default:
{
        fprintf(stderr,"Warning (%d): %s\n",warning,"Invalid warning code.");
        break; 
      }
    }
  }
  else 
    fprintf(stderr,"Warning (%d): %s\n",warning,message);
}

long bots_usecs()
{
  struct timeval t;
  gettimeofday(&t,((0L)));
  return (t.tv_sec * (1000000)) + t.tv_usec;
}

void bots_get_date(char *str)
{
  time_t now;
  time(&now);
  strftime(str,(32),"%Y/%m/%d;%H:%M",((gmtime(((&now))))));
}
#if defined (__linux)
/* ****************************************************************** */

void bots_get_architecture(char *str)
{
  int ncpus = (sysconf((_SC_NPROCESSORS_CONF)));
  struct utsname architecture;
  uname(&architecture);
  sprintf(str,"%s-%s;%d",architecture.utsname::sysname,architecture.utsname::machine,ncpus);
}

void bots_get_load_average(char *str)
{
  double loadavg[3UL];
  getloadavg(loadavg,3);
  sprintf(str,"%.2f;%.2f;%.2f",loadavg[0],loadavg[1],loadavg[2]);
}
#else
/* ****************************************************************** */
void bots_get_architecture(char *str)
{
  int ncpus = (sysconf((_SC_NPROCESSORS_CONF)));
  sprintf(str,"SunOS-sparc;%d",ncpus);
}

void bots_get_load_average(char *str)
{
  double loadavg[3UL];
  getloadavg(loadavg,3);
  sprintf(str,"%.2f;%.2f;%.2f",loadavg[0],loadavg[1],loadavg[2]);
}
#endif

void bots_print_results()
{
  char str_name[128UL];
  char str_parameters[128UL];
  char str_model[128UL];
  char str_resources[128UL];
  char str_result[15UL];
  char str_time_program[15UL];
  char str_time_sequential[15UL];
  char str_speed_up[15UL];
  char str_number_of_tasks[15UL];
  char str_number_of_tasks_per_second[15UL];
  char str_exec_date[128UL];
  char str_exec_message[128UL];
  char str_architecture[128UL];
  char str_load_avg[128UL];
  char str_comp_date[128UL];
  char str_comp_message[128UL];
  char str_cc[128UL];
  char str_cflags[128UL];
  char str_ld[128UL];
  char str_ldflags[128UL];
  char str_cutoff[128UL];
/* compute output strings */
  sprintf(str_name,"%s",bots_name);
  sprintf(str_parameters,"%s",bots_parameters);
  sprintf(str_model,"%s",bots_model);
  sprintf(str_cutoff,"%s",bots_cutoff);
  sprintf(str_resources,"%s",bots_resources);
  switch(bots_result){
    case 0:
{
      sprintf(str_result,"n/a");
      break; 
    }
    case 1:
{
      sprintf(str_result,"successful");
      break; 
    }
    case 2:
{
      sprintf(str_result,"UNSUCCESSFUL");
      break; 
    }
    case 3:
{
      sprintf(str_result,"Not requested");
      break; 
    }
    default:
{
      sprintf(str_result,"error");
      break; 
    }
  }
  snprintf(str_time_program, 15UL, "%f",bots_time_program);
  if (bots_sequential_flag) 
    sprintf(str_time_sequential,"%f",bots_time_sequential);
  else 
    sprintf(str_time_sequential,"n/a");
  if (bots_sequential_flag) 
    sprintf(str_speed_up,"%3.2f",(bots_time_sequential / bots_time_program));
  else 
    sprintf(str_speed_up,"n/a");
  sprintf(str_number_of_tasks,"%3.2f",(((float )bots_number_of_tasks)));
  sprintf(str_number_of_tasks_per_second,"%3.2f",((((float )bots_number_of_tasks)) / bots_time_program));
  sprintf(str_exec_date,"%s",bots_exec_date);
  sprintf(str_exec_message,"%s",bots_exec_message);
  bots_get_architecture(str_architecture);
  bots_get_load_average(str_load_avg);
  sprintf(str_comp_date,"%s",bots_comp_date);
  sprintf(str_comp_message,"%s",bots_comp_message);
  sprintf(str_cc,"%s",bots_cc);
  sprintf(str_cflags,"%s",bots_cflags);
  sprintf(str_ld,"%s",bots_ld);
  sprintf(str_ldflags,"%s",bots_ldflags);
  if (bots_print_header) {
    switch(bots_output_format){
      case 0:
{
        break; 
      }
      case 1:
{
        break; 
      }
      case 2:
{
        fprintf(stdout,"Benchmark;Parameters;Model;Cutoff;Resources;Result;Time;Sequential;Speed-up;Nodes;Nodes/Sec;Exec Date;Exec Time;Exec Message;Architecture;Processors;Load Avg-1;Load Avg-5;Load Avg-15;Comp Date;Comp Time;Comp Message;CC;CFLAGS;LD;LDFLAGS\n");
        break; 
      }
      case 3:
{
        break; 
      }
      case 4:
{
        fprintf(stdout,"Benchmark;Parameters;Model;Cutoff;Resources;Result;Time;Sequential;Speed-up;Nodes;Nodes/Sec;\n");
        break; 
      }
      default:
{
        break; 
      }
    }
  }
/* print results */
  switch(bots_output_format){
    case 0:
{
      break; 
    }
    case 1:
{
      fprintf(stdout,"\n");
/*fix*/
      fprintf(stdout,"Program             = %s\n",str_name);
/*fix*/
      fprintf(stdout,"Parameters          = %s\n",str_parameters);
      fprintf(stdout,"Model               = %s\n",str_model);
      fprintf(stdout,"Embedded cut-off    = %s\n",str_cutoff);
      fprintf(stdout,"# of Threads        = %s\n",str_resources);
      fprintf(stdout,"Verification        = %s\n",str_result);
      fprintf(stdout,"Time Program        = %s seconds\n",str_time_program);
      if (bots_sequential_flag) {
        fprintf(stdout,"Time Sequential     = %s seconds\n",str_time_sequential);
        fprintf(stdout,"Speed-up            = %s\n",str_speed_up);
      }
      if (bots_number_of_tasks > 0) {
        fprintf(stdout,"Nodes               = %s\n",str_number_of_tasks);
        fprintf(stdout,"Nodes/Sec           = %s\n",str_number_of_tasks_per_second);
      }
      fprintf(stdout,"Execution Date      = %s\n",str_exec_date);
      fprintf(stdout,"Execution Message   = %s\n",str_exec_message);
      fprintf(stdout,"Architecture        = %s\n",str_architecture);
      fprintf(stdout,"Load Avg [1:5:15]   = %s\n",str_load_avg);
      fprintf(stdout,"Compilation Date    = %s\n",str_comp_date);
      fprintf(stdout,"Compilation Message = %s\n",str_comp_message);
      fprintf(stdout,"Compiler            = %s\n",str_cc);
      fprintf(stdout,"Compiler Flags      = %s\n",str_cflags);
      fprintf(stdout,"Linker              = %s\n",str_ld);
      fprintf(stdout,"Linker Flags        = %s\n",str_ldflags);
      fflush(stdout);
      break; 
    }
    case 2:
{
      fprintf(stdout,"%s;%s;%s;%s;%s;%s;",str_name,str_parameters,str_model,str_cutoff,str_resources,str_result);
      fprintf(stdout,"%s;%s;%s;",str_time_program,str_time_sequential,str_speed_up);
      fprintf(stdout,"%s;%s;",str_number_of_tasks,str_number_of_tasks_per_second);
      fprintf(stdout,"%s;%s;",str_exec_date,str_exec_message);
      fprintf(stdout,"%s;%s;",str_architecture,str_load_avg);
      fprintf(stdout,"%s;%s;",str_comp_date,str_comp_message);
      fprintf(stdout,"%s;%s;%s;%s;",str_cc,str_cflags,str_ld,str_ldflags);
      fprintf(stdout,"\n");
      break; 
    }
    case 3:
{
      fprintf(stdout,"\n");
/*fix*/
      fprintf(stdout,"Program             = %s\n",str_name);
/*fix*/
      fprintf(stdout,"Parameters          = %s\n",str_parameters);
      fprintf(stdout,"Model               = %s\n",str_model);
      fprintf(stdout,"Embedded cut-off    = %s\n",str_cutoff);
      fprintf(stdout,"# of Threads        = %s\n",str_resources);
      fprintf(stdout,"Verification        = %s\n",str_result);
      fprintf(stdout,"Time Program        = %s seconds\n",str_time_program);
      if (bots_sequential_flag) {
        fprintf(stdout,"Time Sequential     = %s seconds\n",str_time_sequential);
        fprintf(stdout,"Speed-up            = %s\n",str_speed_up);
      }
      if (bots_number_of_tasks > 0) {
        fprintf(stdout,"Nodes               = %s\n",str_number_of_tasks);
        fprintf(stdout,"Nodes/Sec           = %s\n",str_number_of_tasks_per_second);
      }
      break; 
    }
    case 4:
{
      fprintf(stdout,"%s;%s;%s;%s;%s;%s;",str_name,str_parameters,str_model,str_cutoff,str_resources,str_result);
      fprintf(stdout,"%s;%s;%s;",str_time_program,str_time_sequential,str_speed_up);
      fprintf(stdout,"%s;%s;",str_number_of_tasks,str_number_of_tasks_per_second);
      fprintf(stdout,"\n");
      break; 
    }
    default:
{
      bots_error(0,("No valid output format\n"));
      break; 
    }
  }
}
