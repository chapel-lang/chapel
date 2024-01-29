/*
 * This is a test harness for testing which physical CPUs, logical CPUs, and
 * NIC the runtime assigns to the process (locale) based on how many
 * processes are running on the node and the rank of this process. It
 * initializes just enough of the runtime as necessary to provide this
 * information, then prints the results. The '-N' argument specifies the
 * initial NIC such as might be returned by fi_getinfo, and if specified
 * chpl_topo_selectNicByType() is called to determine which NIC of the same
 * type the process should use.
 */

#include <stdio.h>
#include "lib/colocales.h"
#include <hwloc.h>

void Usage(char *name) {
  fprintf(stderr, "Usage: %s [-m mask] [-N nic] [-n numColocales] [-r rank]\n", name);
  fprintf(stderr, "\t-m <mask>\tMask off accessible PUs\n");
  fprintf(stderr, "\t-N <nic>\tNIC bus address\n");
  fprintf(stderr, "\t-n <numLocales>\tNumber of locales on node\n");
  fprintf(stderr, "\t-r <rank>\tLocal rank\n");
  fprintf(stderr, "\t-h\t\tPrint this message\n");
}

// Prints all physical and logical CPUs.
int main(int argc, char* argv[]) {
  int opt;
  int count;
  int *cpus = NULL;
  char *mask = NULL;
  int rank = -1;
  int numLocales = 1;
  char *numLocalesStr = "1";
  char *nic = NULL;

  while ((opt = getopt(argc, argv, "m:N:n:r:")) != -1) {
    switch(opt) {
      case 'm':
        mask = optarg;
        break;
      case 'n':
        numLocales = atoi(optarg);
        numLocalesStr = optarg;
        break;
      case 'r':
        rank = atoi(optarg);
        break;
      case 'N':
        nic = optarg;
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

  if (rank >= numLocales) {
    fprintf(stderr, "Rank must be less than number of locales on node.\n");
    exit(1);
  }
  setenv("CHPL_RT_LOCALES_PER_NODE", numLocalesStr, 1);
  chpl__init_colocales(0, 0); // unsure why this is needed
  chpl_set_num_locales_on_node(numLocales);
  if (rank != -1) {
    chpl_set_local_rank(rank);
  }
  chpl_topo_pre_comm_init(mask);
  chpl_topo_post_comm_init();

  hwloc_topology_t topology = (hwloc_topology_t) chpl_topo_getHwlocTopology();

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

  if (nic != NULL) {
    hwloc_obj_t obj = hwloc_get_pcidev_by_busidstring(topology, nic);
    if (obj == NULL) {
      fprintf(stderr, "Cannot find NIC w/ bus address \"%s\"\n", nic);
      exit(1);
    }
    struct hwloc_pcidev_attr_s *attr = &(obj->attr->pcidev);
    chpl_topo_pci_addr_t inAddr;
    chpl_topo_pci_addr_t outAddr;
    chpl_topo_pci_addr_t *result;

    inAddr.domain = attr->domain;
    inAddr.bus = attr->bus;
    inAddr.device = attr->dev;
    inAddr.function = attr->func;

    result = chpl_topo_selectNicByType(&inAddr, &outAddr);
    if (result == NULL) {
      fprintf(stderr, "chpl_topo_selectNicByType returned NULL\n");
      exit(1);
    }
    printf("NIC: %04x:%02x:%02x.%x\n", result->domain, result->bus,
           result->device, result->function);
  }
}
