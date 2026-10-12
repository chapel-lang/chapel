/*
 * This is a test harness for testing which physical and logical CPUs are
 * available to the process. It initializes just enough of the runtime as
 * necessary to provide this information, then prints the results.
 */

#include <stdio.h>
#include "lib/cpuKinds.h"

void Usage(char *name) {
  fprintf(stderr, "Usage: %s [-m mask]\n", name);
  fprintf(stderr, "\t-m <mask>\tMask off accessible PUs\n");
  fprintf(stderr, "\t-h\t\tPrint this message\n");
}

// Prints all physical and logical CPUs.
int main(int argc, char* argv[]) {
  int opt;
  int count;
  int *cpus = NULL;
  char *mask = NULL;

  while ((opt = getopt(argc, argv, "m:")) != -1) {
    switch(opt) {
      case 'm':
        mask = optarg;
        break;
      case 'h':
        Usage(argv[0]);
        exit(0);
        break;
      case '?':
      default:
        Usage(argv[0]);
        exit(1);
    }
  }
  if (optind < argc) {
    Usage(argv[0]);
    exit(1);
  }

  setupProgramInfoHereManually();
  chpl__init_cpuKinds(0, 0); // unsure why this is needed
  chpl_set_num_locales_on_node(1);
  chpl_topo_pre_comm_init(mask);
  chpl_topo_post_comm_init();

  bool physical = false;
  do {
    physical = !physical;
    if (physical) {
      count = chpl_topo_getNumCPUsPhysical(true /* accessible_only */);
    } else {
      count = chpl_topo_getNumCPUsLogical(true /* accessible_only */);
    }
    cpus = malloc(count * sizeof(*cpus));
    count = chpl_topo_getCPUs(physical, cpus, count);
    printf("%s CPUs:", physical ? "Physical" : "Logical");
    for (int i = 0; i < count; i++) {
      printf(" %d", cpus[i]);
    }
    printf("\n");
    free(cpus);
  } while (physical);
}
