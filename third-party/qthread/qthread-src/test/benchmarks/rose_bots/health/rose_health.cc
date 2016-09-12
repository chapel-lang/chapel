/**********************************************************************************************/
/*  This program is part of the Barcelona OpenMP Tasks Suite                                  */
/*  Copyright (C) 2009 Barcelona Supercomputing Center - Centro Nacional de Supercomputacion  */
/*  Copyright (C) 2009 Universitat Politecnica de Catalunya                                   */
/**********************************************************************************************/
/* OLDEN parallel C for dynamic structures: compiler, runtime system
 * and benchmarks
 *       
 * Copyright (C) 1994-1996 by Anne Rogers (amr@cs.princeton.edu) and
 * Martin Carlisle (mcc@cs.princeton.edu)
 * ALL RIGHTS RESERVED.
 *
 * OLDEN is distributed under the following conditions:
 *
 * You may make copies of OLDEN for your own use and modify those copies.
 *
 * All copies of OLDEN must retain our names and copyright notice.
 *
 * You may not sell OLDEN or distribute OLDEN in conjunction with a
 * commercial product or service without the expressed written consent of
 * Anne Rogers and Martin Carlisle.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE.
 *
 */
/******************************************************************* 
 *  Health.c : Model of the Colombian Health Care System           *
 *******************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "app-desc.h"
#include "bots.h"
#include "health.h"
/* global variables */
#include "libxomp.h" 
int sim_level;
int sim_cities;
int sim_population_ratio;
int sim_time;
int sim_assess_time;
int sim_convalescence_time;
long sim_seed;
float sim_get_sick_p;
float sim_convalescence_p;
float sim_realloc_p;
int sim_pid = 0;
int res_population;
int res_hospitals;
int res_personnel;
int res_checkin;
int res_village;
int res_waiting;
int res_assess;
int res_inside;
float res_avg_stay;
/**********************************************************
 * Handles math routines for health.c                     *
 **********************************************************/

float my_rand(long *seed)
{
  long k;
  float answer;
  long idum =  *seed;
  idum ^= (123459876);
  k = (idum / (127773));
  idum = (((16807) * (idum - (k * (127773)))) - ((2836) * k));
  idum ^= (123459876);
  if (idum < (0)) 
    idum += (2147483647);
  answer = (((float )(1.0 / (2147483647))) * (idum));
   *seed = ((long )(answer * (2147483647)));
  return answer;
}
/********************************************************************
 * Handles lists.                                                   *
 ********************************************************************/

void addList(struct Patient **list,struct Patient *patient)
{
  if ( *list == ((0L))) {
     *list = patient;
    patient -> Patient::back = ((0L));
    patient -> Patient::forward = ((0L));
  }
  else {
    struct Patient *aux =  *list;
    while((aux -> Patient::forward) != ((0L)))
      aux = (aux -> Patient::forward);
    aux -> Patient::forward = patient;
    patient -> Patient::back = aux;
    patient -> Patient::forward = ((0L));
  }
}

void removeList(struct Patient **list,struct Patient *patient)
{
#if 0
// Patient not found
// Removing patient
#else
  if ((patient -> Patient::back) != ((0L))) 
    ( *(patient -> Patient::back)).Patient::forward = (patient -> Patient::forward);
  else 
     *list = (patient -> Patient::forward);
  if ((patient -> Patient::forward) != ((0L))) 
    ( *(patient -> Patient::forward)).Patient::back = (patient -> Patient::back);
#endif
}
/**********************************************************************/

void allocate_village(struct Village **capital,struct Village *back,struct Village *next,int level,int vid)
{
  int i;
  int population;
  int personnel;
  struct Village *current;
  struct Village *inext;
  struct Patient *patient;
  if (level == 0) 
     *capital = ((0L));
  else {
    personnel = ((int )(pow((2),(level))));
    population = (personnel * sim_population_ratio);
/* Allocate Village */
     *capital = ((struct Village *)(malloc(((sizeof(struct Village ))))));
/* Initialize Village */
    ( *( *capital)).Village::back = back;
    ( *( *capital)).Village::next = next;
    ( *( *capital)).Village::level = level;
    ( *( *capital)).Village::id = vid;
    ( *( *capital)).Village::seed = ((vid) * ((127773) + sim_seed));
    ( *( *capital)).Village::population = ((0L));
    for (i = 0; i < population; i++) {
      patient = ((struct Patient *)(malloc(((sizeof(struct Patient ))))));
      patient -> Patient::id = sim_pid++;
      patient -> Patient::seed = ( *( *capital)).Village::seed;
// changes seed for capital:
      my_rand(&( *( *capital)).Village::seed);
      patient -> Patient::hosps_visited = 0;
      patient -> Patient::time = 0;
      patient -> Patient::time_left = 0;
      patient -> Patient::home_village =  *capital;
      addList(&( *( *capital)).Village::population,patient);
    }
/* Initialize Hospital */
    ( *( *capital)).Village::hosp.Hosp::personnel = personnel;
    ( *( *capital)).Village::hosp.Hosp::free_personnel = personnel;
    ( *( *capital)).Village::hosp.Hosp::assess = ((0L));
    ( *( *capital)).Village::hosp.Hosp::waiting = ((0L));
    ( *( *capital)).Village::hosp.Hosp::inside = ((0L));
    ( *( *capital)).Village::hosp.Hosp::realloc = ((0L));
    omp_init_lock(&( *( *capital)).Village::hosp.Hosp::realloc_lock);
// Create Cities (lower level)
    inext = ((0L));
    for (i = sim_cities; i > 0; i--) {
      allocate_village(&current, *capital,inext,(level - 1),((vid * sim_cities) + i));
      inext = current;
    }
    ( *( *capital)).Village::forward = current;
  }
}
/**********************************************************************/

struct Results get_results(struct Village *village)
{
  struct Village *vlist;
  struct Patient *p;
  struct Results t_res;
  struct Results p_res;
  t_res.Results::hosps_number = (0.0);
  t_res.Results::hosps_personnel = (0.0);
  t_res.Results::total_patients = (0.0);
  t_res.Results::total_in_village = (0.0);
  t_res.Results::total_waiting = (0.0);
  t_res.Results::total_assess = (0.0);
  t_res.Results::total_inside = (0.0);
  t_res.Results::total_hosps_v = (0.0);
  t_res.Results::total_time = (0.0);
  if (village == ((0L))) 
    return t_res;
/* Traverse village hierarchy (lower level first)*/
  vlist = (village -> Village::forward);
  while(vlist){
    p_res = (get_results(vlist));
    t_res.Results::hosps_number += p_res.Results::hosps_number;
    t_res.Results::hosps_personnel += p_res.Results::hosps_personnel;
    t_res.Results::total_patients += p_res.Results::total_patients;
    t_res.Results::total_in_village += p_res.Results::total_in_village;
    t_res.Results::total_waiting += p_res.Results::total_waiting;
    t_res.Results::total_assess += p_res.Results::total_assess;
    t_res.Results::total_inside += p_res.Results::total_inside;
    t_res.Results::total_hosps_v += p_res.Results::total_hosps_v;
    t_res.Results::total_time += p_res.Results::total_time;
    vlist = (vlist -> Village::next);
  }
  t_res.Results::hosps_number += 1.0;
  t_res.Results::hosps_personnel += (village -> Village::hosp.Hosp::personnel);
// Patients in the village
  p = (village -> Village::population);
  while(p != ((0L))){
    t_res.Results::total_patients += 1.0;
    t_res.Results::total_in_village += 1.0;
    t_res.Results::total_hosps_v += ((float )(p -> Patient::hosps_visited));
    t_res.Results::total_time += ((float )(p -> Patient::time));
    p = (p -> Patient::forward);
  }
// Patients in hospital: waiting
  p = village -> Village::hosp.Hosp::waiting;
  while(p != ((0L))){
    t_res.Results::total_patients += 1.0;
    t_res.Results::total_waiting += 1.0;
    t_res.Results::total_hosps_v += ((float )(p -> Patient::hosps_visited));
    t_res.Results::total_time += ((float )(p -> Patient::time));
    p = (p -> Patient::forward);
  }
// Patients in hospital: assess
  p = village -> Village::hosp.Hosp::assess;
  while(p != ((0L))){
    t_res.Results::total_patients += 1.0;
    t_res.Results::total_assess += 1.0;
    t_res.Results::total_hosps_v += ((float )(p -> Patient::hosps_visited));
    t_res.Results::total_time += ((float )(p -> Patient::time));
    p = (p -> Patient::forward);
  }
// Patients in hospital: inside
  p = village -> Village::hosp.Hosp::inside;
  while(p != ((0L))){
    t_res.Results::total_patients += 1.0;
    t_res.Results::total_inside += 1.0;
    t_res.Results::total_hosps_v += ((float )(p -> Patient::hosps_visited));
    t_res.Results::total_time += ((float )(p -> Patient::time));
    p = (p -> Patient::forward);
  }
  return t_res;
}
/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

void check_patients_inside(struct Village *village)
{
  struct Patient *list = village -> Village::hosp.Hosp::inside;
  struct Patient *p;
  while(list != ((0L))){
    p = list;
    list = (list -> Patient::forward);
    p -> Patient::time_left--;
    if ((p -> Patient::time_left) == 0) {
      village -> Village::hosp.Hosp::free_personnel++;
      removeList(&village -> Village::hosp.Hosp::inside,p);
      addList(&village -> Village::population,p);
    }
  }
}
/**********************************************************************/

void check_patients_assess_par(struct Village *village)
{
  struct Patient *list = village -> Village::hosp.Hosp::assess;
  float rand;
  struct Patient *p;
  while(list != ((0L))){
    p = list;
    list = (list -> Patient::forward);
    p -> Patient::time_left--;
    if ((p -> Patient::time_left) == 0) {
      rand = my_rand(&p -> Patient::seed);
/* sim_covalescense_p % */
      if (rand < sim_convalescence_p) {
        rand = my_rand(&p -> Patient::seed);
/* !sim_realloc_p % or root hospital */
        if ((rand > sim_realloc_p) || ((village -> Village::level) == sim_level)) {
          removeList(&village -> Village::hosp.Hosp::assess,p);
          addList(&village -> Village::hosp.Hosp::inside,p);
          p -> Patient::time_left = sim_convalescence_time;
          p -> Patient::time += (p -> Patient::time_left);
        }
        else 
/* move to upper level hospital !!! */
{
          village -> Village::hosp.Hosp::free_personnel++;
          removeList(&village -> Village::hosp.Hosp::assess,p);
          omp_set_lock(&village -> Village::hosp.Hosp::realloc_lock);
          addList(&( *(village -> Village::back)).Village::hosp.Hosp::realloc,p);
          omp_unset_lock(&village -> Village::hosp.Hosp::realloc_lock);
        }
      }
      else 
/* move to village */
{
        village -> Village::hosp.Hosp::free_personnel++;
        removeList(&village -> Village::hosp.Hosp::assess,p);
        addList(&village -> Village::population,p);
      }
    }
  }
}
/**********************************************************************/

void check_patients_waiting(struct Village *village)
{
  struct Patient *list = village -> Village::hosp.Hosp::waiting;
  struct Patient *p;
  while(list != ((0L))){
    p = list;
    list = (list -> Patient::forward);
    if (village -> Village::hosp.Hosp::free_personnel > 0) {
      village -> Village::hosp.Hosp::free_personnel--;
      p -> Patient::time_left = sim_assess_time;
      p -> Patient::time += (p -> Patient::time_left);
      removeList(&village -> Village::hosp.Hosp::waiting,p);
      addList(&village -> Village::hosp.Hosp::assess,p);
    }
    else {
      p -> Patient::time++;
    }
  }
}
/**********************************************************************/

void check_patients_realloc(struct Village *village)
{
  struct Patient *p;
  struct Patient *s;
  while(village -> Village::hosp.Hosp::realloc != ((0L))){
    p = (s = village -> Village::hosp.Hosp::realloc);
    while(p != ((0L))){
      if ((p -> Patient::id) < (s -> Patient::id)) 
        s = p;
      p = (p -> Patient::forward);
    }
    removeList(&village -> Village::hosp.Hosp::realloc,s);
    put_in_hosp(&village -> Village::hosp,s);
  }
}
/**********************************************************************/

void check_patients_population(struct Village *village)
{
  struct Patient *list = (village -> Village::population);
  struct Patient *p;
  float rand;
  while(list != ((0L))){
    p = list;
    list = (list -> Patient::forward);
/* randomize in patient */
    rand = my_rand(&p -> Patient::seed);
    if (rand < sim_get_sick_p) {
      removeList(&village -> Village::population,p);
      put_in_hosp(&village -> Village::hosp,p);
    }
  }
}
/**********************************************************************/

void put_in_hosp(struct Hosp *hosp,struct Patient *patient)
{
  patient -> Patient::hosps_visited++;
  if ((hosp -> Hosp::free_personnel) > 0) {
    hosp -> Hosp::free_personnel--;
    addList(&hosp -> Hosp::assess,patient);
    patient -> Patient::time_left = sim_assess_time;
    patient -> Patient::time += (patient -> Patient::time_left);
  }
  else {
    addList(&hosp -> Hosp::waiting,patient);
  }
}
/**********************************************************************/
#if defined (IF_CUTOFF)
// lowest level returns nothing
// only for sim_village first call with village = NULL
// recursive call cannot occurs
/* Traverse village hierarchy (lower level first)*/
/* Uses lists v->hosp->inside, and v->return */
/* Uses lists v->hosp->assess, v->hosp->inside, v->population and (v->back->hosp->realloc) !!! */
/* Uses lists v->hosp->waiting, and v->hosp->assess */
/* Uses lists v->hosp->realloc, v->hosp->asses and v->hosp->waiting */
/* Uses list v->population, v->hosp->asses and v->h->waiting */
#elif defined (MANUAL_CUTOFF)

struct OUT__3__1527___data 
{
  struct Village *vlist;
}
;
static void OUT__3__1527__(void *__out_argv);

void sim_village_par(struct Village *village)
{
  struct Village *vlist;
// lowest level returns nothing
// only for sim_village first call with village = NULL
// recursive call cannot occurs
  if (village == ((0L))) 
    return ;
/* Traverse village hierarchy (lower level first)*/
  vlist = (village -> Village::forward);
  if ((sim_level - (village -> Village::level)) < bots_cutoff_value) {
    while(vlist){
      struct OUT__3__1527___data __out_argv3__1527__;
      __out_argv3__1527__.OUT__3__1527___data::vlist = vlist;
      XOMP_task(OUT__3__1527__,&__out_argv3__1527__,0,sizeof(struct OUT__3__1527___data ),4,1,1);
      vlist = (vlist -> Village::next);
    }
  }
  else {
    while(vlist){
      sim_village_par(vlist);
      vlist = (vlist -> Village::next);
    }
  }
/* Uses lists v->hosp->inside, and v->return */
  check_patients_inside(village);
/* Uses lists v->hosp->assess, v->hosp->inside, v->population and (v->back->hosp->realloc) !!! */
  check_patients_assess_par(village);
/* Uses lists v->hosp->waiting, and v->hosp->assess */
  check_patients_waiting(village);
  if ((sim_level - (village -> Village::level)) < bots_cutoff_value) {
    XOMP_taskwait();
  }
/* Uses lists v->hosp->realloc, v->hosp->asses and v->hosp->waiting */
  check_patients_realloc(village);
/* Uses list v->population, v->hosp->asses and v->h->waiting */
  check_patients_population(village);
}
#else
// lowest level returns nothing
// only for sim_village first call with village = NULL
// recursive call cannot occurs
/* Traverse village hierarchy (lower level first)*/
/* Uses lists v->hosp->inside, and v->return */
/* Uses lists v->hosp->assess, v->hosp->inside, v->population and (v->back->hosp->realloc) !!! */
/* Uses lists v->hosp->waiting, and v->hosp->assess */
/* Uses lists v->hosp->realloc, v->hosp->asses and v->hosp->waiting */
/* Uses list v->population, v->hosp->asses and v->h->waiting */
#endif
/**********************************************************************/

void my_print(struct Village *village)
{
  struct Village *vlist;
  struct Patient *plist;
  struct Patient *p;
  if (village == ((0L))) 
    return ;
/* Traverse village hierarchy (lower level first)*/
  vlist = (village -> Village::forward);
  while(vlist){
    my_print(vlist);
    vlist = (vlist -> Village::next);
  }
  plist = (village -> Village::population);
  while(plist != ((0L))){
    p = plist;
    plist = (plist -> Patient::forward);
{
      if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
        fprintf(stdout,"[pid:%d]",(p -> Patient::id));
      }
    }
  }
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout,"[vid:%d]\n",(village -> Village::id));
    }
  }
}
/**********************************************************************/

void read_input_data(char *filename)
{
  FILE *fin;
  int res;
  if ((fin = fopen((filename),"r")) == ((0L))) {{
      if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
        fprintf(stdout,"Could not open sequence file (%s)\n",filename);
      }
    }
    exit((-1));
  }
  res = fscanf(fin,"%d %d %d %d %d %d %ld %f %f %f %d %d %d %d %d %d %d %d %f",&sim_level,&sim_cities,&sim_population_ratio,&sim_time,&sim_assess_time,&sim_convalescence_time,&sim_seed,&sim_get_sick_p,&sim_convalescence_p,&sim_realloc_p,&res_population,&res_hospitals,&res_personnel,&res_checkin,&res_village,&res_waiting,&res_assess,&res_inside,&res_avg_stay);
  if (res == (-1)) {{
      if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
        fprintf(stdout,"Bogus input file (%s)\n",filename);
      }
    }
    exit((-1));
  }
  fclose(fin);
// Printing input data
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout,"\n");
    }
  }
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout,"Number of levels    = %d\n",sim_level);
    }
  }
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout,"Cities per level    = %d\n",sim_cities);
    }
  }
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout,"Population ratio    = %d\n",sim_population_ratio);
    }
  }
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout,"Simulation time     = %d\n",sim_time);
    }
  }
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout,"Assess time         = %d\n",sim_assess_time);
    }
  }
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout,"Convalescence time  = %d\n",sim_convalescence_time);
    }
  }
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout,"Initial seed        = %d\n",((int )sim_seed));
    }
  }
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout,"Get sick prob.      = %f\n",(sim_get_sick_p));
    }
  }
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout,"Convalescence prob. = %f\n",(sim_convalescence_p));
    }
  }
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout,"Realloc prob.       = %f\n",(sim_realloc_p));
    }
  }
}

int check_village(struct Village *top)
{
  struct Results result = get_results(top);
  int answer = 1;
  if ((res_population) != result.Results::total_patients) 
    answer = 2;
  if ((res_hospitals) != result.Results::hosps_number) 
    answer = 2;
  if ((res_personnel) != result.Results::hosps_personnel) 
    answer = 2;
  if ((res_checkin) != result.Results::total_hosps_v) 
    answer = 2;
  if ((res_village) != result.Results::total_in_village) 
    answer = 2;
  if ((res_waiting) != result.Results::total_waiting) 
    answer = 2;
  if ((res_assess) != result.Results::total_assess) 
    answer = 2;
  if ((res_inside) != result.Results::total_inside) 
    answer = 2;
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout,"\n");
    }
  }
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout,"Sim. Variables      = expect / result\n");
    }
  }
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout,"Total population    = %6d / %6d people\n",res_population,((int )result.Results::total_patients));
    }
  }
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout,"Hospitals           = %6d / %6d people\n",res_hospitals,((int )result.Results::hosps_number));
    }
  }
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout,"Personnel           = %6d / %6d people\n",res_personnel,((int )result.Results::hosps_personnel));
    }
  }
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout,"Check-in\'s          = %6d / %6d people\n",res_checkin,((int )result.Results::total_hosps_v));
    }
  }
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout,"In Villages         = %6d / %6d people\n",res_village,((int )result.Results::total_in_village));
    }
  }
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout,"In Waiting List     = %6d / %6d people\n",res_waiting,((int )result.Results::total_waiting));
    }
  }
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout,"In Assess           = %6d / %6d people\n",res_assess,((int )result.Results::total_assess));
    }
  }
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout,"Inside Hospital     = %6d / %6d people\n",res_inside,((int )result.Results::total_inside));
    }
  }
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout,"Average Stay        = %6f / %6f u/time\n",(res_avg_stay),((result.Results::total_time / result.Results::total_patients)));
    }
  }
  my_print(top);
  return answer;
}
/**********************************************************************/

struct OUT__1__1527___data 
{
  void *top_p;
  void *i_p;
}
;
static void OUT__1__1527__(void *__out_argv);

struct OUT__2__1527___data 
{
  void *top_p;
  void *i_p;
}
;
static void OUT__2__1527__(void *__out_argv);

void sim_village_main_par(struct Village *top)
{
  long i;
  struct OUT__2__1527___data __out_argv2__1527__;
  __out_argv2__1527__.OUT__2__1527___data::i_p = ((void *)(&i));
  __out_argv2__1527__.OUT__2__1527___data::top_p = ((void *)(&top));
  XOMP_parallel_start(OUT__2__1527__,&__out_argv2__1527__,1,0,"OUT__2__1527__");
  XOMP_parallel_end();
}

static void OUT__1__1527__(void *__out_argv)
{
  struct Village **top = (struct Village **)(((struct OUT__1__1527___data *)__out_argv) -> OUT__1__1527___data::top_p);
  long *i = (long *)(((struct OUT__1__1527___data *)__out_argv) -> OUT__1__1527___data::i_p);
  for ( *i = (0);  *i < (sim_time); ( *i)++) 
    sim_village_par( *top);
}

static void OUT__2__1527__(void *__out_argv)
{
  struct Village **top = (struct Village **)(((struct OUT__2__1527___data *)__out_argv) -> OUT__2__1527___data::top_p);
  long *i = (long *)(((struct OUT__2__1527___data *)__out_argv) -> OUT__2__1527___data::i_p);
  if (XOMP_single()) {
    struct OUT__1__1527___data __out_argv1__1527__;
    __out_argv1__1527__.OUT__1__1527___data::i_p = ((void *)(&( *i)));
    __out_argv1__1527__.OUT__1__1527___data::top_p = ((void *)(&( *top)));
    XOMP_task(OUT__1__1527__,&__out_argv1__1527__,0,sizeof(struct OUT__1__1527___data ),4,1,1);
  }
  XOMP_barrier();
}

static void OUT__3__1527__(void *__out_argv)
{
  struct Village *vlist = (struct Village *)(((struct OUT__3__1527___data *)__out_argv) -> OUT__3__1527___data::vlist);
  struct Village *_p_vlist = vlist;
  sim_village_par(_p_vlist);
}
