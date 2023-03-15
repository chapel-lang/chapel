/*
 * Copyright © 2015-2022 Inria.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include "private/autogen/config.h"
#include "hwloc.h"
#include "misc.h"

#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#include "private/cpuid-x86.h"

#if defined(HWLOC_WIN_SYS) && !defined(__CYGWIN__)
#include <direct.h>
#define mkdir(name, mode) _mkdir(name)
#include <io.h>
#define access _access
#ifndef X_OK
#define X_OK 00 /* meaningless */
#endif
#ifndef W_OK
#define W_OK 02
#endif
#endif

static int verbose = 1;

static void dump_one_cpuid(FILE *output, unsigned *regs, unsigned inregmask)
{
  unsigned i;

  /* clear unused inputs */
  for(i=0; i<4; i++)
    if (!(inregmask & (1<<i)))
      regs[i] = 0;

  fprintf(output, "%x %x %x %x %x", inregmask, regs[0], regs[1], regs[2], regs[3]);
  hwloc_x86_cpuid(&regs[0], &regs[1], &regs[2], &regs[3]);
  fprintf(output, " => %x %x %x %x\n", regs[0], regs[1], regs[2], regs[3]);
}

static int dump_one_proc(hwloc_topology_t topo, hwloc_obj_t pu, const char *path)
{
  unsigned regs[4];
  unsigned highest_cpuid, highest_ext_cpuid;
  unsigned i;
  int has_intel_x2apic = 0;
  int has_intel_pconfig = 0;
  int has_intel_sgx = 0;
  int has_amd_topoext = 0;
  FILE *output;
  int err;

  err = hwloc_set_cpubind(topo, pu->cpuset, HWLOC_CPUBIND_PROCESS);
  if (err < 0) {
    err = hwloc_set_cpubind(topo, pu->cpuset, HWLOC_CPUBIND_THREAD);
    if (err < 0) {
      fprintf(stderr, "Cannot bind to PU P#%u\n", pu->os_index);
      return -1;
    }
  }

  if (path) {
    output = fopen(path, "w");
    if (!output) {
      fprintf(stderr, "Cannot open file '%s' for writing: %s\n", path, strerror(errno));
      return -1;
    }
    if (verbose)
      printf("Gathering CPUID of PU P#%u in path %s ...\n", pu->os_index, path);
  } else {
    output = stdout;
    if (verbose)
      printf("Gathering CPUID of PU P#%u on stdout ...\n", pu->os_index);
  }

  fprintf(output, "# mask e[abcd]x => e[abcd]x\n");

  regs[0] = 0;
  hwloc_x86_cpuid(&regs[0], &regs[1], &regs[2], &regs[3]);
  highest_cpuid = regs[0];
  regs[0] = 0x80000000;
  hwloc_x86_cpuid(&regs[0], &regs[1], &regs[2], &regs[3]);
  highest_ext_cpuid = regs[0];

  /* 0x0 = Highest cpuid + Vendor string */
  regs[0] = 0x0;
  dump_one_cpuid(output, regs, 0x1);

  /* 0x1 = Family, Model, Stepping, Topology, Features */
  if (highest_cpuid >= 0x1) {
    regs[0] = 0x1;
    dump_one_cpuid(output, regs, 0x1);
    if (regs[2] & (1 << 21))
      has_intel_x2apic = 1;
  }

  /* 0x2 = Cache + TLB on Intel ; Reserved on AMD */
  if (highest_cpuid >= 0x2) {
    regs[0] = 0x2;
    dump_one_cpuid(output, regs, 0x1);
  }

  /* 0x3 = Processor serial number on Intel P3, reserved otherwise ; Reserved on AMD */
  if (highest_cpuid >= 0x3) {
    regs[0] = 0x3;
    dump_one_cpuid(output, regs, 0x1);
  }

  /* 0x4 = Caches on Intel ; Reserved on AMD */
  if (highest_cpuid >= 0x4) {
    for(i=0; ; i++) {
      regs[0] = 0x4; regs[2] = i;
      dump_one_cpuid(output, regs, 0x5);
      if (!(regs[0] & 0x1f))
	/* invalid, no more caches */
	break;
    }
  }

  /* 0x5 = Monitor/mwait */
  if (highest_cpuid >= 0x5) {
    regs[0] = 0x5;
    dump_one_cpuid(output, regs, 0x1);
  }

  /* 0x6 = Thermal and Power management */
  if (highest_cpuid >= 0x6) {
    regs[0] = 0x6;
    dump_one_cpuid(output, regs, 0x1);
  }

  /* 0x7 = Extended features */
  if (highest_cpuid >= 0x7) {
    unsigned max;
    regs[0] = 0x7; regs[2] = 0;
    dump_one_cpuid(output, regs, 0x5);
    if (regs[3] & (1<<18))
      has_intel_pconfig = 1;
    if (regs[1] & (1<<2))
      has_intel_sgx = 1;
    max = regs[0];
    for(i=1; i<=max; i++) {
      regs[0] = 0x7; regs[2] = i;
      dump_one_cpuid(output, regs, 0x5);
    }
  }

  /* 0x9 = DCA on Intel ; Reserved on AMD */
  if (highest_cpuid >= 0x9) {
    regs[0] = 0x9;
    dump_one_cpuid(output, regs, 0x1);
  }

  /* 0xa = Perf monitoring on Intel ; Reserved on AMD */
  if (highest_cpuid >= 0xa) {
    regs[0] = 0xa;
    dump_one_cpuid(output, regs, 0x1);
  }

  /* 0xb = Extended Topology Enumeration */
  if (has_intel_x2apic && highest_cpuid >= 0xb) {
    for(i=0; ; i++) {
      regs[0] = 0xb; regs[2] = i;
      dump_one_cpuid(output, regs, 0x5);
      if (!(regs[2] & 0xff00))
	/* invalid, no more levels */
	break;
    }
  }

  /* 0xd = Extended state enumeration */
  if (highest_cpuid >= 0xd) {
    unsigned xcr0_l, xcr0_h, ia32xss_l, ia32xss_h;

    regs[0] = 0xd; regs[2] = 0;
    dump_one_cpuid(output, regs, 0x5);
    xcr0_l = regs[0]; xcr0_h = regs[3];

    regs[0] = 0xd; regs[2] = 1;
    dump_one_cpuid(output, regs, 0x5);
    ia32xss_l = regs[2]; ia32xss_h = regs[3];

    for(i=2; i<32; i++) {
      if ((xcr0_l | ia32xss_l) & (1<<i)) {
	regs[0] = 0xd; regs[2] = i;
	dump_one_cpuid(output, regs, 0x5);
      }
    }
    for(i=0; i<32; i++) {
      if ((xcr0_h | ia32xss_h) & (1<<i)) {
	regs[0] = 0xd; regs[2] = i+32;
	dump_one_cpuid(output, regs, 0x5);
      }
    }
  }

  /* 0xf = Platform/L3 QoS enumeration on Intel ; Reserved on AMD */
  if (highest_cpuid >= 0xf) {
    regs[0] = 0xf; regs[2] = 0;
    dump_one_cpuid(output, regs, 0x5);
    regs[0] = 0xf; regs[2] = 1;
    dump_one_cpuid(output, regs, 0x5);
  }

  /* 0x10 = Platform/L3 QoS enforcement enumeration on Intel ; Reserved on AMD */
  if (highest_cpuid >= 0x10) {
    /* Intel Resource Director Technology (Intel RDT) Allocation */
    regs[0] = 0x10; regs[2] = 0;
    dump_one_cpuid(output, regs, 0x5);
    /* L3 Cache Allocation Technology */
    regs[0] = 0x10; regs[2] = 1;
    dump_one_cpuid(output, regs, 0x5);
    /* L2 Cache Allocation Technology */
    regs[0] = 0x10; regs[2] = 2;
    dump_one_cpuid(output, regs, 0x5);
    /* Memory Bandwidth Allocation */
    regs[0] = 0x10; regs[2] = 3;
    dump_one_cpuid(output, regs, 0x5);
  }

  /* 0x12 = SGX Attributes Enumeration on Intel ; Reserved on AMD */
  if (has_intel_sgx && highest_cpuid >= 0x12) {
    regs[0] = 0x12; regs[2] = 0;
    dump_one_cpuid(output, regs, 0x5);
    regs[0] = 0x12; regs[2] = 1;
    dump_one_cpuid(output, regs, 0x5);
    for(i=2; ; i++) {
      regs[0] = 0x12; regs[2] = i;
      dump_one_cpuid(output, regs, 0x5);
      if (!(regs[0] & 0xf))
	/* invalid, no more subleaves */
	break;
    }
  }

  /* 0x14 = Processor trace enumeration on Intel ; Reserved on AMD */
  if (highest_cpuid >= 0x14) {
    regs[0] = 0x14; regs[2] = 0;
    dump_one_cpuid(output, regs, 0x5);
    regs[0] = 0x14; regs[2] = 1;
    dump_one_cpuid(output, regs, 0x5);
  }

  /* 0x15 = Timestamp counter/core crystal clock on Intel ; Reserved on AMD */
  if (highest_cpuid >= 0x15) {
    regs[0] = 0x15;
    dump_one_cpuid(output, regs, 0x1);
  }

  /* 0x16 = Processor frequency on Intel ; Reserved on AMD */
  if (highest_cpuid >= 0x16) {
    regs[0] = 0x16;
    dump_one_cpuid(output, regs, 0x1);
  }

  /* 0x17 = System-On-Chip Vendor Attribute on Intel ; Reserved on AMD */
  if (highest_cpuid >= 0x17) {
    unsigned maxsocid;
    regs[0] = 0x17; regs[2] = 0;
    dump_one_cpuid(output, regs, 0x5);
    maxsocid = regs[0];
    if (maxsocid >= 3) {
      for(i=1; i<=maxsocid; i++) {
	regs[0] = 0x17; regs[2] = i;
	dump_one_cpuid(output, regs, 0x5);
      }
    }
  }

  /* 0x18 = Deterministic Address Translation Parameters on Intel ; Reserved on AMD */
  if (highest_cpuid >= 0x18) {
    unsigned max;
    regs[0] = 0x18; regs[2] = 0;
    dump_one_cpuid(output, regs, 0x5);
    max = regs[0];
    for(i=1; i<=max; i++) {
      regs[0] = 0x18; regs[2] = i;
      regs[3] = 0; /* mark as invalid in case the cpuid call doesn't do anything */
      dump_one_cpuid(output, regs, 0x5);
      if (!(regs[3] & 0x1f))
	/* invalid, but it doesn't mean the next subleaf will be invalid */
        continue;
    }
  }

  /* 0x19 = Key Locker Leaf on Intel ; Reserved on AMD */
  if (highest_cpuid >= 0x19) {
    regs[0] = 0x19;
    dump_one_cpuid(output, regs, 0x1);
  }

  /* 0x1a = Hybrid Information Enumeration Leaf on Intel ; Reserved on AMD */
  if (highest_cpuid >= 0x1a) {
    regs[0] = 0x1a; regs[2] = 0;
    dump_one_cpuid(output, regs, 0x5);
  }

  /* 0x1b = PCONFIG Information on Intel ; Reserved on AMD */
  if (has_intel_pconfig && highest_cpuid >= 0x1b) {
    for(i=0; ; i++) {
      regs[0] = 0x1b; regs[2] = i;
      dump_one_cpuid(output, regs, 0x5);
      if (!(regs[0] & 0xfff))
	break;
    }
  }

  /* 0x1c = Last Branch Records Information on Intel ; Reserved on AMD */
  if (highest_cpuid >= 0x1c) {
    regs[0] = 0x1c; regs[2] = 0;
    dump_one_cpuid(output, regs, 0x5);
  }

  /* 0x1f = V2 Extended Topology Enumeration on Intel ; Reserved on AMD */
  if (highest_cpuid >= 0x1f) {
    for(i=0; ; i++) {
      regs[0] = 0x1f; regs[2] = i;
      dump_one_cpuid(output, regs, 0x5);
      if (!(regs[2] & 0xff00))
	/* invalid, no more levels */
	break;
    }
  }

  /* 0x20 = Processor History Reset on Intel ; Reserved on AMD */
  if (highest_cpuid >= 0x20) {
    regs[0] = 0x20; regs[2] = 0;
    dump_one_cpuid(output, regs, 0x5);
    /* eax is number of subleaves but subleaves aren't documented?! */
  }

  /* 0x21 is reserved on Intel */

  if (highest_cpuid > 0x21) {
    static int reported = 0;
    if (!reported)
      fprintf(stderr, "WARNING: Processor supports new CPUID leaves upto 0x%x\n", highest_cpuid);
    reported = 1;
  }

  /* 0x80000000 = Largest extended cpuid */
  regs[0] = 0x80000000;
  dump_one_cpuid(output, regs, 0x1);

  /* 0x80000001 = Extended processor signature and features */
  if (highest_ext_cpuid >= 0x80000001) {
    regs[0] = 0x80000001;
    dump_one_cpuid(output, regs, 0x1);
    if (regs[2] & (1 << 22))
      has_amd_topoext = 1;
  }

  /* 0x80000002-4 = Processor name string */
  if (highest_ext_cpuid >= 0x80000002) {
    regs[0] = 0x80000002;
    dump_one_cpuid(output, regs, 0x1);
  }
  if (highest_ext_cpuid >= 0x80000003) {
    regs[0] = 0x80000003;
    dump_one_cpuid(output, regs, 0x1);
  }
  if (highest_ext_cpuid >= 0x80000004) {
    regs[0] = 0x80000004;
    dump_one_cpuid(output, regs, 0x1);
  }

  /* 0x80000005 = L1 and TLB on AMD ; Reserved on Intel */
  if (highest_ext_cpuid >= 0x80000005) {
    regs[0] = 0x80000005;
    dump_one_cpuid(output, regs, 0x1);
  }

  /* 0x80000006 = L2, L3 and TLB on AMD ; L2 and reserved on Intel */
  if (highest_ext_cpuid >= 0x80000006) {
    regs[0] = 0x80000006;
    dump_one_cpuid(output, regs, 0x1);
  }

  /* 0x80000007 = Advanced power management on AMD ; Almost reserved on Intel */
  if (highest_ext_cpuid >= 0x80000007) {
    regs[0] = 0x80000007;
    dump_one_cpuid(output, regs, 0x1);
  }

  /* 0x80000008 = Long mode and topology on AMD ; Long mode on Intel */
  if (highest_ext_cpuid >= 0x80000008) {
    regs[0] = 0x80000008;
    dump_one_cpuid(output, regs, 0x1);
  }

  /* 0x8000000a = SVM on AMD ; Reserved on Intel */
  if (highest_ext_cpuid >= 0x8000000a) {
    regs[0] = 0x8000000a;
    dump_one_cpuid(output, regs, 0x1);
  }

  /* 0x80000019 = TLB1G + Perf optim identifiers on AMD ; Reserved on Intel */
  if (highest_ext_cpuid >= 0x80000019) {
    regs[0] = 0x80000019;
    dump_one_cpuid(output, regs, 0x1);
  }

  /* 0x8000001a = Performance Optimization Identifiers on AMD ; Reserved on Intel */
  if (highest_ext_cpuid >= 0x8000001a) {
    regs[0] = 0x8000001a;
    dump_one_cpuid(output, regs, 0x1);
  }

  /* 0x8000001b = IBS on AMD ; Reserved on Intel */
  if (highest_ext_cpuid >= 0x8000001b) {
    regs[0] = 0x8000001b;
    dump_one_cpuid(output, regs, 0x1);
  }

  /* 0x8000001c = Profiling on AMD ; Reserved on Intel */
  if (highest_ext_cpuid >= 0x8000001c) {
    regs[0] = 0x8000001c;
    dump_one_cpuid(output, regs, 0x1);
  }

  /* 0x8000001d = Cache properties on AMD ; Reserved on Intel */
  if (highest_ext_cpuid >= 0x8000001d) {
    for(i=0; ; i++) {
      regs[0] = 0x8000001d; regs[2] = i;
      dump_one_cpuid(output, regs, 0x5);
      if (!(regs[0] & 0x1f))
	/* no such cache, no more cache */
	break;
    }
  }

  /* 0x8000001e = Topoext on AMD ; Reserved on Intel */
  if (has_amd_topoext && highest_ext_cpuid >= 0x8000001e) {
    regs[0] = 0x8000001e;
    dump_one_cpuid(output, regs, 0x1);
  }

  /* 0x8000001f = Encrypted Memory Capabilities on AMD ; Reserved on Intel */
  if (highest_ext_cpuid >= 0x8000001f) {
    regs[0] = 0x8000001f;
    dump_one_cpuid(output, regs, 0x1);
  }

  /* 0x80000020 = Platform QoS Enforcement for Memory Bandwidth on AMD ; Reserved on Intel */
  if (highest_ext_cpuid >= 0x80000020) {
    regs[0] = 0x80000020; regs[2] = 0;
    dump_one_cpuid(output, regs, 0x5);
    regs[0] = 0x80000020; regs[2] = 1;
    dump_one_cpuid(output, regs, 0x5);
  }

  /* 0x80000021 = Extended Feature Identification 2 on AMD ; Reserved on Intel */
  if (highest_ext_cpuid >= 0x80000021) {
    regs[0] = 0x80000021;
    dump_one_cpuid(output, regs, 0x1);
  }

  /* 0x80000022 = Extended Performance Monitoring and Debug on AMD ; Reserved on Intel */
  if (highest_ext_cpuid >= 0x80000022) {
    regs[0] = 0x80000022;
    dump_one_cpuid(output, regs, 0x1);
  }

  if (highest_ext_cpuid > 0x80000022) {
    static int reported = 0;
    if (!reported)
      fprintf(stderr, "WARNING: Processor supports new extended CPUID leaves upto 0x%x\n", highest_ext_cpuid);
    reported = 1;
  }

  if (path)
    fclose(output);
  return 0;
}

void usage(const char *callname, FILE *where)
{
  fprintf(where, "Usage : %s [ options ] ... [ outdir ]\n", callname);
  fprintf(where, "  outdir is an optional output directory instead of cpuid/\n");
  fprintf(where, "Options:\n");
  fprintf(where, "  -c <n>       Only gather for logical processor with logical index <n>\n");
  fprintf(where, "  -s --silent  Do not show verbose messages\n");
  fprintf(where, "  --version    Report version and exit\n");
  fprintf(where, "  -h --help    Show this usage\n");
}

int main(int argc, const char * const argv[])
{
  hwloc_topology_t topo;
  hwloc_obj_t pu;
  const char *basedir;
  const char *callname;
  char *path;
  size_t pathlen;
  unsigned idx = (unsigned) -1;
  int err;
  int ret = EXIT_SUCCESS;

  callname = strrchr(argv[0], '/');
  if (!callname)
    callname = argv[0];
  else
    callname++;

  argc--; argv++;

  hwloc_utils_check_api_version(callname);

  if (!hwloc_have_x86_cpuid()) {
    fprintf(stderr, "CPUID not supported.\n");
    ret = EXIT_FAILURE;
    goto out;
  }

  while (argc > 0 && argv[0][0] == '-' && argv[0][1] != '\0') {
    if (argc >= 2 && !strcmp(argv[0], "-c")) {
      idx = atoi(argv[1]);
      argc -= 2;
      argv += 2;
    } else if (argc >= 1 && (!strcmp(argv[0], "-s") || !strcmp(argv[0], "--silent"))) {
      verbose--;
      argc--;
      argv++;
    } else if (!strcmp (argv[0], "--version")) {
      printf("%s %s\n", callname, HWLOC_VERSION);
      exit(EXIT_SUCCESS);
    } else if (!strcmp(argv[0], "-h") || !strcmp(argv[0], "--help")) {
      usage(callname, stdout);
      goto out;
    } else {
      usage(callname, stderr);
      ret = EXIT_FAILURE;
      goto out;
    }
  }

  basedir = "./cpuid";
  if (argc >= 1)
    basedir = argv[0];

  if (!getenv("HWLOC_COMPONENTS"))
    putenv((char *) "HWLOC_COMPONENTS=no_os,stop");

  hwloc_topology_init(&topo);
  hwloc_topology_set_all_types_filter(topo, HWLOC_TYPE_FILTER_KEEP_NONE);
  hwloc_topology_set_flags(topo, HWLOC_TOPOLOGY_FLAG_NO_DISTANCES
                                 |HWLOC_TOPOLOGY_FLAG_NO_MEMATTRS
                                 |HWLOC_TOPOLOGY_FLAG_NO_CPUKINDS);
  err = hwloc_topology_load(topo);
  if (err < 0) {
    fprintf(stderr, "Failed to load topology\n");
    ret = EXIT_FAILURE;
    goto out;
  }

  if (!hwloc_topology_is_thissystem(topo)) {
    fprintf(stderr, "%s must run on the current system topology, while this topology doesn't come from this system.\n", callname);
    ret = EXIT_FAILURE;
    goto out;
  }

  if (!strcmp(basedir, "-")) {
    if (verbose)
      printf("Gathering on stdout ...\n");
    if (idx == (unsigned) -1) {
      fprintf(stderr, "Cannot gather multiple PUs on stdout.\n");
      ret = EXIT_FAILURE;
      goto out;
    }
    path = NULL;
    pathlen = 0;
  } else {
    err = mkdir(basedir, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH);
    if (err < 0) {
      if (access(basedir, X_OK|W_OK) < 0) {
	fprintf(stderr, "Could not create/open destination directory %s\n", basedir);
	ret = EXIT_FAILURE;
	goto out_with_topo;
      }
    }
    if (verbose)
      printf("Gathering in directory %s ...\n", basedir);

    pathlen = strlen(basedir) + 20; /* for '/pu%u' or '/hwloc-cpuid-info' */
    path = malloc(pathlen);
  }

  if (idx == (unsigned) -1) {
    FILE *file;
    pu = NULL;
    while ((pu = hwloc_get_next_obj_by_type(topo, HWLOC_OBJ_PU, pu)) != NULL) {
      idx = pu->os_index;
      if (path)
	snprintf(path, pathlen, "%s/pu%u", basedir, idx);
      dump_one_proc(topo, pu, path);
    }

    snprintf(path, pathlen, "%s/hwloc-cpuid-info", basedir);
    file = fopen(path, "w");
    if (file) {
      fprintf(file, "Architecture: x86\n");
      fclose(file);
      if (verbose)
	printf("Summary written to %s\n", path);
    } else {
      fprintf(stderr, "Failed to open summary file '%s' for writing: %s\n", path, strerror(errno));
    }
  } else {
    pu = hwloc_get_pu_obj_by_os_index(topo, idx);
    if (!pu) {
      fprintf(stderr, "Cannot find PU P#%u\n", idx);
      ret = EXIT_FAILURE;
      goto out_with_path;
    } else {
      if (path)
        snprintf(path, pathlen, "%s/pu%u", basedir, idx);
      dump_one_proc(topo, pu, path);
    }
  }

  if (verbose)
    printf("\n"
	   "WARNING: Do not post these files on a public list or website unless you\n"
	   "WARNING: are sure that no information about this platform is sensitive.\n");

 out_with_path:
  free(path);
 out_with_topo:
  hwloc_topology_destroy(topo);
 out:
  return ret;
}
