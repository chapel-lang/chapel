/*
 Copyright 1994-2004 John Plevyak, All Rights Reserved
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef __alpha
#define atoll atol
#endif

/* Argument Handling
*/
struct ArgumentState;

typedef void ArgumentFunction(struct ArgumentState *arg_state, char *arg);

typedef struct {
  char *name;
  char key;
  char *description;
  char *type;
  void *location;
  char *env;
  ArgumentFunction *pfn;
} ArgumentDescription;

typedef struct ArgumentState {
  char **file_argument;
  int nfile_arguments;
  char *program_name;
  ArgumentDescription *desc;
} ArgumentState;

void usage(ArgumentState *arg_state, char *arg_unused);
void process_args(ArgumentState *arg_state, char **argv);
void free_args(ArgumentState *arg_state);
