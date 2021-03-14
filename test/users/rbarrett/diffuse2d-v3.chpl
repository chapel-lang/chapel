use Random;   // For random filling of the initial grid.

/* ======================================================================================================
 *
 * Finite difference: 2d grid averaging.

 *
 * This program computes the solution of a Laplace equation.
 *
 * ====================================================================================================== */

// Configuration variables for program:

config var 
   check_step    = 10,       // Frequency of correctness reporting.
   eps           = 1.0e-6,   // Error tolerance.
   init_seed     = 12345,    // Random number generator seed.
   initial_spike = 1.0e6,    // For error checking.
   n             = 100,      // Size of computational grid.
   verbose       = true;     // Debug/printing control.
   
const 

   // 2D neighbors:
   
   northwest = (-1, -1),
   north     = (-1,  0),
   northeast = (-1,  1),
   west      = ( 0, -1),
   east      = ( 0,  1),
   southwest = ( 1, -1),
   south     = ( 1,  0),
   southeast = ( 1,  1),

   // 3D neighbors:
   
   frontnorthwest  = (-1, -1, 1),
   frontnorth      = (-1,  0, 1),
   frontnortheast  = (-1,  1, 1),
   frontwest       = ( 0, -1, 1),
   front           = ( 0,  0, 1),
   fronteast       = ( 0,  1, 1),
   frontsouthwest  = ( 1, -1, 1),
   frontsouth      = ( 1,  0, 1),
   frontsoutheast  = ( 1,  1, 1),
   
   centernorthwest = (-1, -1, 0),
   centernorth     = (-1,  0, 0),
   centernortheast = (-1,  1, 0),
   centerwest      = ( 0, -1, 0),
   centereast      = ( 0,  1, 0),
   centersouthwest = ( 1, -1, 0),
   centersouth     = ( 1,  0, 0),
   centersoutheast = ( 1,  1, 0),

   backnorthwest   = (-1, -1, -1),
   backnorth       = (-1,  0, -1),
   backnortheast   = (-1,  1, -1),
   backwest        = ( 0, -1, -1),
   back            = ( 0,  0, -1),
   backeast        = ( 0,  1, -1),
   backsouthwest   = ( 1, -1, -1),
   backsouth       = ( 1,  0, -1),
   backsoutheast   = ( 1,  1, -1);

/* ====================================================================================================== */
/*                                               Main program.                                            */    
/* ====================================================================================================== */

proc main() {

   /* ------------ */
   /* Declarations */
   /* ------------ */
   
   const ProblemSpace2d = {1..n, 1..n},       // domain for interior grid points
         BigDomain2d    = {0..n+1, 0..n+1};   // domain with boundary points

   var 
      max_iter = 1000;                      // Maximum number of iterations.

   var 
      grid1,   // 
      grid2 :  // 
         [BigDomain2d] real = 0.0;

   /* --------------------- */
   /* Executable Statements */
   /* --------------------- */

   // Initialize grid1.

   if ( verbose ) 
      {
         var
            spike_loc_i = (n+2) / 2,       // Row location of initial heat source.
            spike_loc_j = (n+2) / 2;       // Column location of initial heat source.
            
            grid1[spike_loc_i, spike_loc_j] = initial_spike;
      }
   else
      {
         fillRandom(grid1, init_seed );
         // fillRandom(grid1, init_seed );  Not yet implemented.
      }
      
   var
      iteration = 1;                      // iteration counter
   
   // Iterate.
   
   writeln ( "\n ** Begin iteration ** \n" );
   
   do 
      {
         /* ------------------------------------------- */
         /* Compute next approximation, store in grid2. */
         /* ------------------------------------------- */
          
         ApplyStencil2d5ptA ( ProblemSpace2d, grid2, grid1, iteration );
         iteration += 1;

         /* ------------------------------------------- */
         /* Compute next approximation, store in grid1. */
         /* ------------------------------------------- */
         
         ApplyStencil2d5ptA ( ProblemSpace2d, grid1, grid2, iteration );

     // Advance iteration counter.

     iteration += 1;

   } while ( iteration <= max_iter );

   writeln("\n ** Iteration complete ** \n" );

} // End main program.

/* ====================================================================================================== */

proc ApplyStencil2d5ptA ( ProblemSpace2d, newGrid, oldGrid, iteration ) {
      
   /* --------------------- */
   /* Executable Statements */
   /* --------------------- */

   // Apply 5-point difference stencil in 2d domain.
   
   forall (i,j) in ProblemSpace2d do
  
      newGrid(ij) = (                 oldGrid(i-1,j) + 
                     oldGrid(i,j-1) + oldGrid(  i,j) + oldGrid(i,j+1) + 
                                      oldGrid(i+1,j)                    )
                  / 5.0;
   // Check answer.
   
   CheckAnswer ( ProblemSpace2d, newGrid, iteration );

} // End function ApplyStencil2d5ptA.

/* ====================================================================================================== */

proc ApplyStencil2d5ptB ( ProblemSpace2d, newGrid, oldGrid, iteration ) {
      
   /* --------------------- */
   /* Executable Statements */
   /* --------------------- */

   // Apply 5-point difference stencil in 2d domain.
   
   forall (i,j) in ProblemSpace2d do
  
      newGrid(ij) = (                    oldGrid(ij+(-1,0)) +
                     oldGrid(ij+(0,-1) + oldGrid(ij)        + oldGrid(ij+(0,1)) +
                                         oldGrid(ij+(1,0))                        )
                  / 5.0;
                            
   // Check answer.
   
   CheckAnswer ( ProblemSpace2d, newGrid, iteration );
   
} // End function ApplyStencil2d5ptB.

/* ====================================================================================================== */

proc ApplyStencil2d5ptC ( ProblemSpace2d, newGrid, oldGrid, iteration ) {
      
   /* --------------------- */
   /* Executable Statements */
   /* --------------------- */

   // Apply 5-point difference stencil in 2d domain.
   
   forall (i,j) in ProblemSpace2d do
  
      newGrid(ij) = (                   oldGrid(ij+north) +
                     oldGrid(ij+west) + oldGrid(ij)       + oldGrid(ij+east)     +
                                        oldGrid(ij+south)                    )
                  / 5.0;

   // Check answer.

   CheckAnswer ( ProblemSpace2d, newGrid, iteration );
   
} // End function ApplyStencil2d5ptC.

/* ====================================================================================================== */

proc ApplyStencil2d9pt ( ProblemSpace2d, newGrid, oldGrid, iteration ) {
      
   /* --------------------- */
   /* Executable Statements */
   /* --------------------- */

   // Apply 9-point difference stencil in 2d domain.
   
   forall (i,j) in ProblemSpace2d do
  
      newGrid(ij) = ( oldGrid(ij+northwest) + oldGrid(ij+north) + oldGrid(ij+northeast)     +
                      oldGrid(ij+west)      + oldGrid(ij)       + oldGrid(ij+east)          +
                      oldGrid(ij+southwest) + oldGrid(ij+south) + oldGrid(ij+southeast)   )
                  / 9.0;
                  
   // Check answer.
   
   CheckAnswer ( ProblemSpace2d, newGrid, iteration );
   
} // End function ApplyStencil2d5pt.

/* ====================================================================================================== */

proc ApplyStencil3d7pt ( ProblemSpace2d, newGrid, oldGrid, iteration ) {
      
   /* --------------------- */
   /* Executable Statements */
   /* --------------------- */

   // Apply 7-point difference stencil to 3d domain.
   
   forall (i,j,k) in ProblemSpace2d do
  
      newGrid(ijk) = (                               oldGrid(ijk+back)                 + 
                                           oldGrid(ijk+north)                          +
                      oldGrid(ijk+west)  + oldGrid(ijk)        + oldGrid(ijk+east)     +
                                           oldGrid(ijk+south)  +
                                    oldGrid(ijk+front)                               )
                    
                  / 7.0;
                  
   // Check answer.
   
   CheckAnswer ( ProblemSpace2d, newGrid, iteration );
   
} // End function ApplyStencil3d7pt.

/* ====================================================================================================== */

proc ApplyStencil3d27pt ( ProblemSpace2d, newGrid, oldGrid, iteration ) {
      
   /* --------------------- */
   /* Executable Statements */
   /* --------------------- */

   // Apply 27-point difference stencil to 3d domain.
   
   forall (i,j, k) in ProblemSpace2d do
  
      newGrid(ijk) = (
                                 oldGrid(ijk+backnorthwest)  + oldGrid(ijk+backnorth)  + oldGrid(ijk+backnortheast) +
                                 oldGrid(ijk+backwest)       + oldGrid(ijk)            + oldGrid(ijk+backeast)      +
                                 oldGrid(ijk+backsouthwest)  + oldGrid(ijk+backsouth)  + oldGrid(ijk+backsoutheast) +
                    
                           oldGrid(ijk+northwest)      + oldGrid(ijk+north)      + oldGrid(ijk+northeast)           +
                           oldGrid(ijk+west)           + oldGrid(ijk)            + oldGrid(ijk+east)                +
                           oldGrid(ijk+southwest)      + oldGrid(ijk+south)      + oldGrid(ijk+southeast)           +

                     oldGrid(ijk+frontnorthwest) + oldGrid(ijk+frontnorth) + oldGrid(ijk+frontnortheast)            +
                     oldGrid(ijk+frontwest)      + oldGrid(ijk)            + oldGrid(ijk+fronteast)                 +
                     oldGrid(ijk+frontsouthwest) + oldGrid(ijk+frontsouth) + oldGrid(ijk+frontsoutheast) )          +

                  / 27.0;
                  
   // Check answer.
   
   CheckAnswer ( ProblemSpace2d, newGrid, iteration );
   
} // End function ApplyStencil3d27pt.

/* ====================================================================================================== */

proc CheckAnswer ( ProblemSpace2d, newGrid, iteration ) {
   
   /* ------------------ */
   /* Local Declarations */
   /* ------------------ */
   
   var
      err : real;  // Computed error.

   /* --------------------- */
   /* Executable Statements */
   /* --------------------- */

   if ( verbose ) 
      {
         if ( iteration < ( n : real / 2.0 ) : int )
            {
               const sum = + reduce newGrid[ProblemSpace2d];
               err = initial_spike - sum;
               if ( abs ( err ) > eps )
                  {
                     writeln ( " ** Error ** diffuse2d, after ", iteration, " iterations; (error = ", err, ")" );
                  }
               else if ( iteration % check_step == 0 )
                  {
                     writeln ( " Iteration correct after ", iteration, " steps." );
                  }
            }
         else
            {
               const sum = + reduce newGrid[ProblemSpace2d];
               writeln ( sum );
            }
      }
} // End function ApplyStencil2d5pt.

/* ====================================================================================================== */
/* End diffuse2d.c                                                                                        */
/* ====================================================================================================== */
