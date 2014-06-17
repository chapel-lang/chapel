use Random;

/*
 *   Finite difference: 2d grid averaging.
 *
 *   This program computes the solution of a Laplace equation.
 *
 */

// Configuration variables for program:

config var 
   n = 100,           // Size of computational grid
   max_iter = 100,    // max_iter
   eps = 1.0e-6,      // Error tolerance [back-ported from v3]
   verbose = false;   // printing control

config var initial_spike = 1000.0;

   
proc main() {

   const ProblemSpace = {1..n, 1..n},       // domain for interior grid points
         BigDomain    = {0..n+1, 0..n+1};   // domain with boundary points

   var 
      grid1,   // 
      grid2 :  // 
         [BigDomain] real = 0.0;

   /* --------------------- */
   /* Executable Statements */
   /* --------------------- */

   // Initialize grid1.
   
   if ( verbose ) 
      {
         var
           spike_loc_i = sqrt ( n ) : int,       // Row location of initial heat source.
           spike_loc_j = sqrt ( n ) : int;       // Column location of initial heat source.

            grid1[spike_loc_i, spike_loc_j] = initial_spike;
      }
   else
      fillRandom(grid1);

   var 
      iteration = 0;                    // iteration counter

   do 
      {
         /* ------------------------------------------- */
         /* Compute next approximation, store in grid2. */
         /* ------------------------------------------- */

         computeNextApproximation(ProblemSpace, grid2, grid1, iteration);

         /* ------------------------------------------- */
         /* Compute next approximation, store in grid1. */
         /* ------------------------------------------- */

         computeNextApproximation(ProblemSpace, grid1, grid2, iteration);


     // Advance iteration counter.

     iteration += 2;

   } while ( iteration < max_iter );

   writeln("Iteration complete.");
   writeln("# of iterations: ", iteration);
}


proc computeNextApproximation(ProblemSpace, newGrid, oldGrid, iteration) {
  forall (i,j) in ProblemSpace do
  
    newGrid(i,j) = (                 oldGrid(i-1,j) + 
                    oldGrid(i,j-1) + oldGrid(  i,j) + oldGrid(i,j+1) + 
                                     oldGrid(i+1,j)                    )
                    / 5.0;
                           
  if ( verbose ) 
     {
       if ( iteration < ( n / 2 ) )
          {
            const sum = + reduce newGrid[ProblemSpace];
            const err = abs(initial_spike - sum);
            if ( err > eps )
               {
                 writeln ( " ** Error ** diffuse2d: grid sum != initial_spike.",
                           " (by ", err, ")");
               }
          }
     }
}
