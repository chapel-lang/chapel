        

            FCA support for Berkeley UPC:
            =============================

    Overview:

    FCA (Fabric Collective Accelerator) is a Mellanox software package that
    boosts collective network operations.
    
    Features:
    * Significantly reduce collective operations runtime
    * Efficient collective communication flow optimized to job and topology
    * Eliminate congestion caused by collective traffic
  
1.  Pre-requisites:

    Installed and properly configured Mellanox FCA library

2.  Build:

    Berkeley UPC by default will try to find FCA library in standard 
    locations (e.g. /opt/mellanox/fca) during configure stage. Available
    configure paramaters:

    * "--with-fca=<PATH>":                 forces BUPC to use user specified 
                                           path to FCA

    * "--enable-fca" and "--disable-fca":  enable/disable FCA support

    * "--with-fca-enabled-by-default":     forces BUPC to be built with FCA 
                                           support but disables FCA collectives 
                                           if the parameter is set to 0 or "no"

3.  Supported API:
    
    * upc_barrier
    * upc_all_broadcast
    * upc_all_gather_all
    * upc_all_reduceT (T - type)

4.  Runtime parameters:

    Following paramters can be passed to "upcrun" in order to change FCA
    support behaviour:

    * "-fca_enable <0|1>":    disable/enable FCA support at runtime. Default
      value of this parameter depends on configure flag 
      "--with-fca-enabled-by-default"

    * "-fca_np <value>":           FCA support for collective operations will be 
                                   enabled if number of processes in the job is
                                   greater than fca_np value. Note: if specified 
                                   "fca_enable" parameter is preferrable over 
                                   "fca_np" (default is 64)

    * "-fca_verbose <level>":      set verbosity level for FCA module

    * "fca_ops <+/->[op_list]":    op_list - comma separated list of collective
                                   operations. If provided with starting +/-
                                   then only specified operations are enabled/
                                   disabled. Allowed operation names are:
                                   barrier (br), bcast (bt), reduce (rc), 
                                   allgather (ag).
                                   Each operation can be also enabled/disabled 
                                   via environment variable:
                                                    GASNET_FCA_ENABLE_BARRIER,
                                                    GASNET_FCA_ENABLE_BCAST,
                                                    GASNET_FCA_ENABLE_REDUCE,
                                                    GASNET_FCA_ENABLE_ALLGATHER.
                                   +/- provided without op_list enables/disables
                                   all operations. By default all operations
                                   are enabled.

5.  Examples:

    * upcrun -np <np> -fca_enable 0 test // Run test without FCA support

    * upcrun -np <np> -fca_enable 1 -fca_np 10 -fca_verbose 5 // Run test with fca
    if <np> >= 10 and use fca verbosity level equal to 5

    * upcrun -np <np> -fca_ops +barrier,bt test // Run test with FCA support
    for barrier and broadcast operations only

    * upcrun -np <np> -fca_ops -rc test // Run test with FCA support for all
      collectives except for reduce

6.  Known issues:

    On some UPC test FCA based reduce operation may fail to obtain correct
    result. In this case FCA reduce operation is to be disabled with:
    -fca_ops -reduce
    However the only network operation that is used in Berkeley implementation
    of UPC Reduce is gasnet_coll_gather. FCA is still able to provide
    scalability to the Gather operation and this feature is enabled if FCA
    allgather operation is active. If one needs to use pure BUPC version of
    Reduce then FCA Allgather is to be disabled as well:
    -fca_ops -reduce,allgather
