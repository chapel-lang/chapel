
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
   verbose = false;   // printing control
   
proc main() {

   var
      sum : real;
      
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
            spike_loc_i = int ( sqrt ( n ) ),       // Row location of initial heat source.
            spike_loc_j = int ( sqrt ( n ) );       // Column location of initial heat source.

         var
            initial_spike = 1000.0 : real; // Initial heat source value.

         grid1[spike_loc_i*spike_loc_j] = initial_spike;
      }
   else
      grid1[ProblemSpace] = rand ( );

   var 
      iteration = 0;                    // iteration counter

   do 
      {
         /* ------------------------------------------- */
         /* Compute next approximation, store in grid2. */
         /* ------------------------------------------- */

         forall (i,j) in ProblemSpace do
  
            grid2(i,j) = (                  grid1(i-1,j-1) + 
                           grid1(i+1,j-1) + grid1(  i,  i) + grid1(i-1,j+1) + 
                                            grid1(i+1,j+1)                    )
                           / 5.0;

         // Compute the sum of the grid point.

         if ( verbose ) 
            {
               if ( iteration < ( n / 2 ) )
                  {
                     sum = max reduce sum ( grid2[ProblemSpace] );
                     if ( sum != initial_spike )
                        {
                           writeln ( " ** Error ** diffuse2d: grid sum != initial_spike. ");
                           writeln ( "\n" );
                        }
                  }
            }

         /* ------------------------------------------- */
         /* Compute next approximation, store in grid2. */
         /* ------------------------------------------- */

         forall (i,j) in ProblemSpace do
  
            grid1(i,j) = (                  grid2(i-1,j-1) + 
                           grid2(i+1,j-1) + grid2(  i,  i) + grid2(i-1,j+1) + 
                                            grid2(i+1,j+1)                    )
                           / 5.0;
                           
         if ( verbose ) 
            {
               if ( iteration < ( n / 2 ) )
                  {
                     sum = max reduce sum(grid2[ProblemSpace]);
                     if ( sum != initial_spike )
                        {
                           writeln ( " ** Error ** diffuse2d: grid sum != initial_spike. \n" );
                        }
                  }
            }


     // Advance iteration counter.

     iteration += 2;

   } while ( iteration < max_iter );

   writeln("Iteration complete.");
   writeln("# of iterations: ", iteration);
}
