/*
 Copyright 1994-2003 John Plevyak, All Rights Reserved
*/
#include "d.h"

static char *SPACES = "                                                                               ";
static char *arg_types_keys = (char *)"ISDfF+TL";
static char *arg_types_desc[] = {
  (char *)"int     ",
  (char *)"string  ",
  (char *)"double  ",
  (char *)"set off ",
  (char *)"set on  ",
  (char *)"incr    ",
  (char *)"toggle  ",
  (char *)"int64   ",
  (char *)"        "
};

void 
process_arg(ArgumentState *arg_state, int i, char ***argv) {
  char * arg = NULL;
  ArgumentDescription *desc = arg_state->desc;
  if (desc[i].type) {
    char type = desc[i].type[0];
    if (type=='F'||type=='f')
      *(int *)desc[i].location = type=='F'?1:0;
    else if (type=='T')
      *(int *)desc[i].location = !*(int *)desc[i].location;
    else if (type == '+') 
      (*(int *)desc[i].location)++;
    else {
      arg = *++(**argv) ? **argv : *++(*argv);
      if (!arg) usage(arg_state, NULL);
      switch (type) {
        case 'I':
          *(int *)desc[i].location = atoi(arg);
          break;
        case 'D':
          *(double *)desc[i].location = atof(arg);
          break;
        case 'L':
          *(int64 *)desc[i].location = atoll(arg);
          break;
        case 'S': strncpy((char *)desc[i].location,arg, atoi(desc[i].type+1));
          break;
        default:
          fprintf(stderr, "%s:bad argument description\n", 
                 arg_state->program_name);
          exit(1);
          break;
      }
      **argv += strlen(**argv)-1;
    }
  }
  if (desc[i].pfn)
    desc[i].pfn(arg_state, arg);
}

void
process_args(ArgumentState *arg_state, char **argv) {
  int i = 0, len;
  char *end;
  ArgumentDescription *desc = arg_state->desc;
  /* Grab Environment Variables */
  for (i = 0;; i++) {
    if (!desc[i].name)
      break; 
    if (desc[i].env) {
      char type = desc[i].type[0];
      char * env = getenv(desc[i].env);
      if (!env) continue;
      switch (type) {
	case 'A':
	case 'f': 
	case 'F': 
	  break;
	case 'I': *(int *)desc[i].location = strtol(env, NULL, 0); break;
	case 'D': *(double *)desc[i].location = strtod(env, NULL); break;
	case 'L': *(int64 *)desc[i].location = strtoll(env, NULL, 0); break;
	case 'S': strncpy((char *)desc[i].location,env, 
			  strtol(desc[i].type+1, NULL, 0));
	  break;
      }
      if (desc[i].pfn)
	desc[i].pfn(arg_state, env);
    }
  }

  /*
    Grab Command Line Arguments
  */
  arg_state->program_name = argv[0];
  while ( *++argv ) {
    if ( **argv == '-' ) {
      if ((*argv)[1] == '-') {
        for (i = 0;; i++) {
          if (!desc[i].name)
            usage(arg_state,NULL);
	  if ((end = strchr((*argv)+2, '=')))
	    len = end - ((*argv) + 2);
	  else
	    len = strlen((*argv) + 2);
          if (len == strlen(desc[i].name) &&
	      !strncmp(desc[i].name,(*argv)+2, len))
	  {
	    if (!end)
	      *argv += strlen(*argv) - 1;
	    else
	      *argv = end;
            process_arg(arg_state, i, &argv);
            break;
          }
        }
      } else {
        while (*++(*argv))
          for (i = 0;; i++) {
            if (!desc[i].name)
              usage(arg_state, NULL);
            if (desc[i].key == **argv) {
              process_arg(arg_state, i, &argv);
              break;
            }
          }
      }
    } else {
      arg_state->file_argument = (char **)REALLOC(
        arg_state->file_argument, 
        sizeof(char**) * (arg_state->nfile_arguments + 2));
      arg_state->file_argument[arg_state->nfile_arguments++] = *argv;
      arg_state->file_argument[arg_state->nfile_arguments] = NULL;
    }
  }
}

void 
usage(ArgumentState *arg_state, char *arg_unused) {
  ArgumentDescription *desc = arg_state->desc;
  int i;

  (void)arg_unused;
  fprintf(stderr,"Usage: %s [flags|args]\n",arg_state->program_name);
  for (i = 0;; i++) {
    if (!desc[i].name)
      break;
    if (!desc[i].description)
      continue;
    fprintf(stderr,"  %c%c%c --%s%s%s", 
	    desc[i].key != ' ' ? '-' : ' ', desc[i].key, 
	    desc[i].key != ' ' ? ',' : ' ', desc[i].name,
            (strlen(desc[i].name) + 61 < 81) ?
             &SPACES[strlen(desc[i].name)+61] : "",
            arg_types_desc[desc[i].type?strchr(arg_types_keys,desc[i].type[0])-
                arg_types_keys : strlen(arg_types_keys)]);
    switch(desc[i].type?desc[i].type[0]:0) {
      case 0: fprintf(stderr, "          "); break;
      case 'L':
        fprintf(stderr,
#ifdef __alpha
                " %-9ld",
#else
#ifdef FreeBSD
                " %-9qd",
#else
                " %-9lld",
#endif
#endif
                *(int64*)desc[i].location);
        break;
      case 'S':
        if (*(char*)desc[i].location) {
          if (strlen((char*)desc[i].location) < 10)
            fprintf(stderr, " %-9s", (char*)desc[i].location);
          else {
            ((char*)desc[i].location)[7] = 0;
            fprintf(stderr, " %-7s..", (char*)desc[i].location);
          }
        } else
          fprintf(stderr, " (null)   ");
        break;
      case 'D':
        fprintf(stderr, " %-9.3e", *(double*)desc[i].location);
        break;
      case '+': 
      case 'I':
        fprintf(stderr, " %-9d", *(int *)desc[i].location);
        break;
      case 'T': case 'f': case 'F':
        fprintf(stderr, " %-9s", *(int *)desc[i].location?"true ":"false");
        break;
    }
    fprintf(stderr," %s\n",desc[i].description);
  }
  exit(1);
}

void
free_args(ArgumentState *arg_state) {
  if (arg_state->file_argument)
    FREE(arg_state->file_argument);
}
