#! /usr/bin/env bash

# Gather the list of libraries required for a PrgEnv compilation,
# or an LLVM compilation when on a Cray XC system.

if [[ -z $1 || -z $2 || -z $3 ]]; then
  echo "Usage: $0 \$CHPL_COMM \$CHPL_COMM_SUBSTRATE \$CHPL_AUXFS"
  exit 1
fi

chpl_comm="$1"
chpl_comm_substrate="$2"
chpl_auxfs="$3"
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
  if pkg-config --libs cray-lustre-api-devel 2>/dev/null; then
    pe_chapel_pkgconfig_libs="cray-lustre-api-devel:$pe_chapel_pkgconfig_libs"
  fi
fi

echo $pe_chapel_pkgconfig_libs
