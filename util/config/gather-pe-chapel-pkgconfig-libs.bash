#! /usr/bin/env bash

# Gather the list of libraries required for a PrgEnv compilation,
# or an LLVM compilation when on a Cray system.

chpl_target_platform="${CHPL_MAKE_TARGET_PLATFORM:-$1}"
chpl_comm="${CHPL_MAKE_COMM:-$2}"
chpl_comm_substrate="${CHPL_MAKE_COMM_SUBSTRATE:-$3}"
chpl_auxfs="${CHPL_MAKE_AUXFS:-$4}"

if [[ -z $chpl_target_platform || -z $chpl_comm || -z $chpl_comm_substrate || -z $chpl_auxfs ]]; then
  echo "Usage: $0 [\$CHPL_TARGET_PLATFORM \$CHPL_COMM \$CHPL_COMM_SUBSTRATE \$CHPL_AUXFS]"
  echo "  CHPL_MAKE_* must be present if args are empty"
  exit 1
fi

pe_chapel_pkgconfig_libs="$PE_CHAPEL_PKGCONFIG_LIBS"

if [[ "$chpl_comm" != none ]]; then
  if [[ "$chpl_target_platform" != hpe-cray-ex ]]; then
    pe_chapel_pkgconfig_libs="cray-pmi:$pe_chapel_pkgconfig_libs"
  fi

  if [[ "$chpl_target_platform" == cray-x* ]]; then
    pe_chapel_pkgconfig_libs="cray-ugni:$pe_chapel_pkgconfig_libs"
    if [[ "$chpl_comm" == gasnet && "$chpl_comm_substrate" == aries  ]]; then
      pe_chapel_pkgconfig_libs="cray-udreg:$pe_chapel_pkgconfig_libs"
    fi
  fi

  if [[ "$chpl_comm" == ofi && $($CHPL_HOME/util/chplenv/chpl_libfabric.py) == system ]]; then
    pe_chapel_pkgconfig_libs="libfabric:$pe_chapel_pkgconfig_libs"
  fi
fi


# on login/compute nodes, lustre requires the devel api to make
# lustre/lustreapi.h available (it's implicitly available on esl nodes)
if [[ "$chpl_auxfs" == *lustre* ]]; then
  if pkg-config --exists cray-lustre-api-devel; then
    pe_chapel_pkgconfig_libs="cray-lustre-api-devel:$pe_chapel_pkgconfig_libs"
  fi
fi

echo $pe_chapel_pkgconfig_libs
