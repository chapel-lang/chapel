single-node (CHPL_COMM=none)
----------------------------

.. list-table::
   :header-rows: 1

   * - CHPL_TARGET_COMPILER
     - CHPL_LAUNCHER
     - CHPL_TARGET_MEM
     - CHPL_SANITIZE_EXE
     - OS compatibility
   * - llvm
     -
     - jemalloc
     - none
     -
   * - llvm
     -
     - cstdlib
     - address
     - unsupported on Debian 12, Ubuntu 22, and AL2023
   * - llvm
     - slurm-srun
     - jemalloc
     - none
     -
   * - llvm
     - slurm-srun
     - cstdlib
     - address
     - unsupported on Debian 12, Ubuntu 22, and AL2023
   * - clang
     -
     - jemalloc
     - none
     -
   * - clang
     -
     - cstdlib
     - address
     -
   * - clang
     - slurm-srun
     - jemalloc
     - none
     -
   * - clang
     - slurm-srun
     - cstdlib
     - address
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
     - OS compatibility
   * - llvm
     - udp
     - everything
     - jemalloc
     - none
     -
   * - llvm
     - udp
     - everything
     - cstdlib
     - address
     - unsupported on Debian 12, Ubuntu 22, and AL2023
   * - llvm
     - udp
     - fast
     - jemalloc
     - none
     -
   * - llvm
     - smp
     - fast
     - jemalloc
     - none
     -
   * - clang
     - udp
     - everything
     - jemalloc
     - none
     -
   * - clang
     - udp
     - everything
     - cstdlib
     - address
     -
   * - clang
     - udp
     - fast
     - jemalloc
     - none
     -
   * - clang
     - smp
     - fast
     - jemalloc
     - none
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
     -
   * - llvm
     - slurm-srun
   * - clang
     -
   * - clang
     - slurm-srun


single-node emulated GPU
------------------------

Common settings:

* ``CHPL_LOCALE_MODEL=gpu``
* ``CHPL_GPU=cpu``

