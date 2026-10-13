single-node (CHPL_COMM=none)
----------------------------

.. list-table::
   :header-rows: 1

   * - CHPL_TARGET_COMPILER
     - CHPL_LAUNCHER
     - CHPL_TARGET_MEM
     - CHPL_SANITIZE_EXE
     - CHPL_LIB_PIC
     - OS compatibility
   * - llvm
     - none
     - jemalloc
     - none
     - none
     -
   * - llvm
     - none
     - jemalloc
     - none
     - pic
     -
   * - llvm
     - none
     - cstdlib
     - address
     - none
     - unsupported on Debian 12, Ubuntu 22, and AL2023
   * - llvm
     - none
     - cstdlib
     - address
     - pic
     - unsupported on Debian 12, Ubuntu 22, and AL2023
   * - llvm
     - slurm-srun
     - jemalloc
     - none
     - none
     -
   * - llvm
     - slurm-srun
     - jemalloc
     - none
     - pic
     -
   * - llvm
     - slurm-srun
     - cstdlib
     - address
     - none
     - unsupported on Debian 12, Ubuntu 22, and AL2023
   * - llvm
     - slurm-srun
     - cstdlib
     - address
     - pic
     - unsupported on Debian 12, Ubuntu 22, and AL2023
   * - clang
     - none
     - jemalloc
     - none
     - none
     -
   * - clang
     - none
     - jemalloc
     - none
     - pic
     -
   * - clang
     - none
     - cstdlib
     - address
     - none
     -
   * - clang
     - none
     - cstdlib
     - address
     - pic
     -
   * - clang
     - slurm-srun
     - jemalloc
     - none
     - none
     -
   * - clang
     - slurm-srun
     - jemalloc
     - none
     - pic
     -
   * - clang
     - slurm-srun
     - cstdlib
     - address
     - none
     -
   * - clang
     - slurm-srun
     - cstdlib
     - address
     - pic
     -


multi-node (CHPL_COMM=gasnet over udp/smp)
------------------------------------------

Common settings:

* ``CHPL_COMM=gasnet``

.. list-table::
   :header-rows: 1

   * - CHPL_TARGET_COMPILER
     - CHPL_COMM_SUBSTRATE
     - CHPL_GASNET_SEGMENT
     - CHPL_TARGET_MEM
     - CHPL_SANITIZE_EXE
     - CHPL_LIB_PIC
     - OS compatibility
   * - llvm
     - udp
     - everything
     - jemalloc
     - none
     - none
     -
   * - llvm
     - udp
     - everything
     - jemalloc
     - none
     - pic
     -
   * - llvm
     - udp
     - everything
     - cstdlib
     - address
     - none
     - unsupported on Debian 12, Ubuntu 22, and AL2023
   * - llvm
     - udp
     - everything
     - cstdlib
     - address
     - pic
     - unsupported on Debian 12, Ubuntu 22, and AL2023
   * - llvm
     - udp
     - fast
     - jemalloc
     - none
     - none
     -
   * - llvm
     - udp
     - fast
     - jemalloc
     - none
     - pic
     -
   * - llvm
     - smp
     - fast
     - jemalloc
     - none
     - none
     -
   * - llvm
     - smp
     - fast
     - jemalloc
     - none
     - pic
     -
   * - clang
     - udp
     - everything
     - jemalloc
     - none
     - none
     -
   * - clang
     - udp
     - everything
     - jemalloc
     - none
     - pic
     -
   * - clang
     - udp
     - everything
     - cstdlib
     - address
     - none
     -
   * - clang
     - udp
     - everything
     - cstdlib
     - address
     - pic
     -
   * - clang
     - udp
     - fast
     - jemalloc
     - none
     - none
     -
   * - clang
     - udp
     - fast
     - jemalloc
     - none
     - pic
     -
   * - clang
     - smp
     - fast
     - jemalloc
     - none
     - none
     -
   * - clang
     - smp
     - fast
     - jemalloc
     - none
     - pic
     -


multi-node (CHPL_COMM=ofi over pmi2)
------------------------------------

Common settings:

* ``CHPL_COMM=ofi``
* ``CHPL_LIBFABRIC=bundled``
* ``CHPL_COMM_OFI_OOB=pmi2``
* ``CHPL_NETWORK_ATOMICS=ofi``

.. list-table::
   :header-rows: 1

   * - CHPL_TARGET_COMPILER
     - CHPL_LAUNCHER
   * - llvm
     - none
   * - llvm
     - slurm-srun
   * - clang
     - none
   * - clang
     - slurm-srun


single-node emulated GPU
------------------------

Common settings:

* ``CHPL_LOCALE_MODEL=gpu``
* ``CHPL_GPU=cpu``

