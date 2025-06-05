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

extern int verbosity;

void Usage(char *name) {
  fprintf(stderr, "Usage: %s [-m mask] [-N nic] [-n numColocales] [-r rank]\n", name);
  fprintf(stderr, "\t-m <mask>\tMask off accessible PUs\n");
  fprintf(stderr, "\t-N <nic>\tNIC bus address\n");
  fprintf(stderr, "\t-n <num>\tExpected number of co-locales on node\n");
  fprintf(stderr, "\t-a <num>\tActual number of locales on node\n");
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
  int numCoLocales = -1;
  char *numCoLocalesStr = NULL;
  int numLocales = -1;
  char *nic = NULL;

  while ((opt = getopt(argc, argv, "a:m:N:n:r:v")) != -1) {
    switch(opt) {
      case 'a':
        numLocales = atoi(optarg);
        break;
      case 'm':
        mask = optarg;
        break;
      case 'n':
        numCoLocales = atoi(optarg);
        numCoLocalesStr = optarg;
        break;
      case 'r':
        rank = atoi(optarg);
        break;
      case 'N':
        nic = optarg;
        break;
      case 'v':
        verbosity++;
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

  if (numCoLocales == -1) {
    numCoLocales = 1;
    numCoLocalesStr = "1";
  }

  if (numLocales == -1) {
    numLocales = numCoLocales;
  }

  if (numLocales < 1) {
    fprintf(stderr, "There must be > 0 locales on the node\n");
    exit(1);
  }
  if (rank >= numLocales) {
    fprintf(stderr,
            "Rank (%d) must be less than number of locales on node (%d).\n",
            rank, numLocales);
    exit(1);
  }
  if (numCoLocales > 0) {
    setenv("CHPL_RT_LOCALES_PER_NODE", numCoLocalesStr, 1);
  }
  chpl__init_colocales(0, 0); // unsure why this is needed
  chpl_topo_pre_comm_init(mask);
  chpl_comm_init(NULL, NULL);
  chpl_set_num_locales_on_node(numLocales);
  if (rank != -1) {
    chpl_set_local_rank(rank);
  }
  chpl_topo_post_comm_init();
  chpl_topo_post_args_init();

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

  // Count the number of GPUs. This is a hack, there is probably a better way
  // to do it but I don't want to rely on a helper library such as CUDA or
  // PCI to do it better.
  count = 0;
  hwloc_obj_t obj = NULL;
  while ((obj = hwloc_get_next_pcidev(topology, obj)) != NULL) {
    if (obj->attr->pcidev.vendor_id == 0x10de) {
      // NVIDIA
      count++;
    }
  }
  // Find all the GPUs
  chpl_topo_pci_addr_t inAddrs[count];
  chpl_topo_pci_addr_t outAddrs[count];
  int i = 0;
  while ((obj = hwloc_get_next_pcidev(topology, obj)) != NULL) {
    if (obj->attr->pcidev.vendor_id == 0x10de) {
      inAddrs[i].domain = (uint8_t) obj->attr->pcidev.domain;
      inAddrs[i].bus = (uint8_t) obj->attr->pcidev.bus;
      inAddrs[i].device = (uint8_t) obj->attr->pcidev.dev;
      inAddrs[i].function = (uint8_t) obj->attr->pcidev.func;
      i++;
    }
  }
  // Select our GPUs.
  int rc = chpl_topo_selectMyDevices(inAddrs, outAddrs, &count);
  if (rc != 0) {
    fprintf(stderr, "chpl_topo_selectMyDevices returned %d\n", rc);
    exit(1);
  }
  printf("GPUS:");
  for(i = 0; i < count; i++) {
    printf(" %04x:%02x:%02x.%x", outAddrs[i].domain, outAddrs[i].bus,
           outAddrs[i].device, outAddrs[i].function);
  }
  printf("\n");
}
