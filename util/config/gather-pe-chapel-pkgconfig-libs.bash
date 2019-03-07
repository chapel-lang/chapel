#! /usr/bin/env bash

# Gather the list of libraries required for a PrgEnv compilation,
# or an LLVM compilation when on a Cray XC system.

chpl_comm="${CHPL_MAKE_COMM:-$1}"
chpl_comm_substrate="${CHPL_MAKE_COMM_SUBSTRATE:-$2}"
chpl_auxfs="${CHPL_MAKE_AUXFS:-$3}"

if [[ -z $chpl_comm || -z $chpl_comm_substrate || -z $chpl_auxfs ]]; then
  echo "Usage: $0 [\$CHPL_COMM \$CHPL_COMM_SUBSTRATE \$CHPL_AUXFS]"
  echo "  CHPL_MAKE_* must be present if args are empty"
  exit 1
fi

pe_chapel_pkgconfig_libs="$PE_CHAPEL_PKGCONFIG_LIBS"

# ugni, ofi-gni, and gasnet-{aries,gemini} require pmi and ugni
if [[ "$chpl_comm" == ugni ]]; then
  pe_chapel_pkgconfig_libs="cray-pmi:cray-ugni:$pe_chapel_pkgconfig_libs"
elif [[ "$chpl_comm" == ofi ]]; then
  # Don't need to add ugni, because it will be added by libfabric (?)
  pe_chapel_pkgconfig_libs="cray-pmi:cray-ugni:$pe_chapel_pkgconfig_libs"
elif [[ "$chpl_comm" == gasnet ]]; then
  if [[ "$chpl_comm_substrate" == gemini || "$chpl_comm_substrate" == aries ]]; then
    pe_chapel_pkgconfig_libs="cray-pmi:cray-udreg:cray-ugni:$pe_chapel_pkgconfig_libs"
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
