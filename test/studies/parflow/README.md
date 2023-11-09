This directory contains files relevant to my (Brandon) Summer 2022 internship project: a partial port of the ParFlow application from C to Chapel.
ParFlow is a large project, in development for nearly 30 years, so including all files required to build and run the application as part of this study is not realistic. 
However, I have included the files most directly related to the project, namely the Chapel code written as part of the port (`chapelCode` subdirectory) and the build script I used to run experiments comparing the original and C+Chapel versions of the application (`buildFiles`).
I also include links below to the C code relevant to the interoperability.
The `ChapelReadMe.md` file in the `buildFiles` subdirectory contains information on how this code is configured and run when integrated into the ParFlow repository.

## ParFlow Overview

ParFlow is a watershed flow model, meaning it simulates the flow of water both above and below ground.
The simulation code is written mostly in C, with some use of Fortran and C++, and is parallelized primarily with MPI.
While there is limited support for OpenMP shared memory parallelism, canonical use runs multiple MPI processes on each node rather than use MPI+OpenMP.
Simulation runs are configured and launched using `tclsh` or `python` scripts, which set the boundary conditions and topology/subsurface characteristics.

The simulation geometry is organized using an octree data structure with variable resolution, so important or complex parts of the domain can be represented with finer granularity.
ParFlow makes heavy use of C preprocessor macros to standardize the complex iteration through the octree data structures.
This complex iteration was the motivation for partial porting, as Chapel has strong support for complex, user-defined iteration.
The iterators implementing the C macros are found in the `GroundGeometry.chpl` file and the macros themselves are found in the various `grgeom*.{c,h}` files, found in [this directory](https://github.com/brandonneth/parflow/tree/chapel/pfsimulator/parflow_lib).
These files also contain the data structures that facilitate the transfer of data between C and Chapel.

## Interoperability Calls

Based on hpctoolkit profiling, the most expensive kernels in ParFlow were the PhaseRelPerm and the RichardsJacobianEval methods.
Ports of those kernels are found in the `PhaseRelPerm.chpl` and `RichardsJacobianEval.chpl` files, respectively. 
While a significant number of lines of code are necessary to pass data from the C code to the Chapel routines, the rest of the routines are clarified by the use of the iterators rather than the macros.
The full files can be found in [this directory](https://github.com/brandonneth/parflow/tree/chapel/pfsimulator/parflow_lib), but the snippet of code that makes the call to the Chapel function is reproduced here:

```
real * pp_chapel = p_sub->data;
real * dp_chapel = d_sub->data;
real * rpp_chapel = rp_sub->data;
real * ddp_chapel = dd_sub->data;
real * rpdp_chapel = rpd_sub->data;

real * permxp_chapel = permx_sub->data;
real * permyp_chapel = permy_sub->data;
real * permzp_chapel = permz_sub->data;

real * fb_x_chapel = FBx_sub->data;
real * fb_y_chapel = FBy_sub->data;
real * fb_z_chapel = FBz_sub->data;

real * x_ssl_chapel = x_ssl_sub->data;
real * y_ssl_chapel = y_ssl_sub->data;
real * z_mult_chapel = z_mult_sub->data;

real * J_chapel = J_sub->data;
real * cp_chapel = cp;
real * wp_chapel = wp;
real * ep_chapel = ep;
real * sop_chapel = sop;
real * np_chapel = np;
real * lp_chapel = lp;
real * up_chapel = up;

richards_gravity_and_second_order_derivative_interior(
    gr_domain, r, ix, iy, iz, nx, ny, nz,
    pp_chapel, dp_chapel, rpp_chapel, ddp_chapel, rpdp_chapel,
    permxp_chapel, permyp_chapel, permzp_chapel,
    fb_x_chapel, fb_y_chapel, fb_z_chapel,
    x_ssl_chapel, y_ssl_chapel, z_mult_chapel,
    J_chapel,cp, wp_chapel,ep_chapel,sop_chapel,np_chapel,lp_chapel,up_chapel,
    grid2d_iz, dx,dy,dz,dt,sy_v, sz_v, sy_m, sz_m,
    public_xtra->tfgupwind, gravity, viscosity,
    SubvectorIX(p_sub), SubvectorIY(p_sub), SubvectorIZ(p_sub), SubvectorNX(p_sub), SubvectorNY(p_sub),
    SubvectorIX(J_sub), SubvectorIY(J_sub), SubvectorIZ(J_sub), SubvectorNX(J_sub), SubvectorNY(J_sub),
    SubvectorIX(x_ssl_sub), SubvectorIY(x_ssl_sub), SubvectorIZ(x_ssl_sub), SubvectorNX(x_ssl_sub), SubvectorNY(x_ssl_sub),
    symm_part);
```

## Future Development
Future development of a Chapel port of ParFlow could start from the partial port present here, but because ParFlow uses MPI for distributed parallelism, the approach of C code calling Chapel routines has limitations.
Instead, it may be beneficial to start a porting effort from the top-down, with a distributed Chapel application handling data distribution and communication and calling out to the existing C routines to run the computationally intensive kernels.



