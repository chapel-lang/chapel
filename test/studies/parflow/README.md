This directory contains files relevant to my (Brandon) Summer 2022 internship project: a partial port of the ParFlow application from C to Chapel.
ParFlow is a large project, in development for nearly 30 years, so including all files required to build and run the application as part of this study is not realistic. 
However, I have included the files most directly related to the project, namely the Chapel code written as part of the port (`chapelCode` subdirectory), the C code that calls out to the Chapel routines (`cInteropCode` subdirectory), and the build script I used to run experiments comparing the original and C+Chapel versions of the application (`buildFiles`).
The `ChapelReadMe.md` file in the `buildFiles` subdirectory contains information on how this code is configured and run when integrated into the ParFlow repository.

ParFlow is a watershed flow model, meaning it simulates the flow of water both above and below ground.
The simulation code is written mostly in C, with some use of Fortran and C++, and is parallelized primarily with MPI.
While there is limited support for OpenMP shared memory parallelism, canonical use runs multiple MPI processes on each node rather than use MPI+OpenMP.
Simulation runs are configured and launched using `tclsh` or `python` scripts, which set the boundary conditions and topology/subsurface characteristics.

The simulation geometry is organized using an octree data structure with variable resolution, so important or complex parts of the domain can be represented with finer granularity.
ParFlow makes heavy use of C preprocessor macros to standardize the complex iteration through the octree data structures.
This complex iteration was the motivation for partial porting, as Chapel has strong support for complex, user-defined iteration.
The iterators implementing the C macros are found in the `GroundGeometry.chpl` file and the macros themselves are found in the various `grgeom*.c` files.

Based on hpctoolkit profiling, the most expensive kernels in ParFlow were the PhaseRelPerm and the RichardsJacobianEval methods.
Ports of those kernels are found in the `PhaseRelPerm.chpl` and `RichardsJacobianEval.chpl` files, respectively. 
While a significant number of lines of code are necessary to pass data from the C code to the Chapel routines, the rest of the routines are clarified by the use of the iterators rather than the macros.

Future development of a Chapel port of ParFlow could start from the partial port present here, but because ParFlow uses MPI for distributed parallelism, the approach of C code calling Chapel routines has limitations.
Instead, it may be beneficial to start a porting effort from the top-down, with a distributed Chapel application handling data distribution and communication and calling out to the existing C routines to run the computationally intensive kernels.



