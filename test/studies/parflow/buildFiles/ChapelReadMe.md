This readme provides details on how to build and run the C+Chapel variant of
ParFlow. All building and running of this variant is done through the
`performance-evaluation.sh` script found in the base directory.

### Prerequisites

A couple of tasks need to be complete before running the C+Chapel version.
First, the `PARFLOW_DIR` environment variable should be set to the 
base ParFlow directory (the directory containing this readme).
Second, the user should update and build a current version of Chapel and set the 
`CHPL_HOME` variable to the base directory of the Chapel repository.

Lastly, you should install Hypre and Silo as directed in the ParFlow manual and set
`HYPRE_DIR` and `SILO_DIR` to the directories containing their `lib` folders.

### Quickstart

To run the C+Chapel version, simply run:
```
bash performance-evaluation.sh --chapel-fast --n 1
```
The `--chapel-fast` flag indicates that the scripts should configure, build,
and run the C+Chapel variant. The Chapel library is built with the `--fast`
compiler flag.

The script can also run the original C implementation with the `--original` flag.

### Script Options

The script supports a number of capabilities. First, it can run 4 different
variants of ParFlow, and can run any number of them in one execution. The flags
are:
- `--original`: Original C variant, sequential
- `--omp`: Original C variant, parallelized with OpenMP
- `--chapel`: C+Chapel variant, no `--fast` compiler flag
- `--chapel-fast`: C +Chapel variant, Chapel library compiled with optimization

Executing the following line will run all four variants, useful for performance
comparisons:
```
bash performance-evaluation.sh --original --omp --chapel --chapel-fast
```

The script has two ways to modify the executed benchmark. First, the `--n` flag
changes the number of repetitions to run for each variant. Second, the `--lw`
flag changes the input `.tcl` file for the execution. Currently, 3 options are
supported. In increasing size order, they are `LW_Tiny.tcl`, `LW_Test.tcl`, and
`LW_Timing.tcl`. This correspond to the files in the
`test/tcl/washita/tcl_scripts/` directory.

Finally, the script supports a second mode for performing correctness checks.
To enable this mode, use the flag `--check-correctness`. To initialize the
files against which the runs are checked, use the `--new-correct` flag the
first time you run. It is recommended that you set `--n` to 1 and only provide
the `--new-correct` flag to ensure that the files are only generated once.
