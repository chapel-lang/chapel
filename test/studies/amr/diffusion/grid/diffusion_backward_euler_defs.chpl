//===> Description ===>
//
// Initializes grid_base_defs and creates a method for a bacward Euler
// diffusion solver.
//
//<=== Description <===


use grid_base_defs;
use grid_solution_defs;
use grid_bc_defs;




// //==== Form rhs for conjugate gradients ====
// q.time += dt_used;  // Use boundary data at t+dt to set flux divergence
// bc.ghostFill(q);
// fluxDivergence(rhs,  q, diffusivity);
// rhs *= -dt_used;


// //==== Update solution ====
// conjugateGradient(dq, bc, rhs, diffusivity, dt_used, dt_used/4.0);
// q.value(cells) += dq.value(cells);



//===> inverseBEOperator method ===>
//=================================>
//--------------------------------------------------------------------------
// Note that when writing Backward Euler as
//    (I - dt*A)Q^{n+1} = Q^n + b^n,
// I'm using "inverse backward Euler operator" to refer to (I - dt*A)^{-1}.
// My "non-inverse backward Euler operator" would be (I-dt*A), the same way
// that (I + dt*A) is the forward Euler operator.
//--------------------------------------------------------------------------
def RectangularGrid.invHomBEOperator(
  sol:         ScalarGridSolution,
  bc:          GridBC,
  diffusivity: real,
  dt:          real, 
  tolerance:   real
){

  //===> Safety checks ===>
  assert(sol.grid == this);
  //<=== Safety checks <===


  //===== Assign names to solution components ====
  var dq        = sol.space_data(1);  // will eventually be q_new, but is dq until the end
  var q_current = sol.space_data(2);
  var t_current = sol.time(2);
  var t_new     = sol.time(2) + dt;
  
  
  //==== Array allocations ====
  var rhs, residual, search_dir, residual_update_dir: [ext_cells] real;
  

  //==== Maximum number of iterations before giving up ====
  var maxiter = 100;


  //==== Calculate the rhs for conjugate gradients ====
  bc.ghostFill(q_current, t_current+dt);
  fluxDivergence(rhs, q_current, diffusivity);
  rhs(cells) *= -dt;
                                            

  //==== Initial guess ====
  dq(cells) = rhs(cells);


  //==== Initialize residual ====
  //------------------------------------------------------------
  // residual = rhs - (dq + dt*flux_divergence(dq))
  //------------------------------------------------------------
  bc.homogeneousGhostFill(dq);
  homBEOperator(residual, dq, diffusivity, dt);
  residual(cells) = rhs(cells) - residual(cells);


  //==== Initialize search direction ====
  search_dir(cells) = residual(cells);


  //==== Initialize residual update direction ====
  //-------------------------------------------
  // Initializes to homBEOperator(search_dir)
  //-------------------------------------------
  bc.homogeneousGhostFill(search_dir);
  homBEOperator(residual_update_dir, search_dir, diffusivity, dt);


  //==== Initialize scalars ====
  var residual_norm = +reduce(residual*residual);
  var alpha, beta, residual_norm_old: real;
  
  
  //===> CG iteration ===>
  for iter in [1..maxiter] {
 
    //==== Update the solution and residual ====
    alpha = +reduce( residual * residual )  
             / +reduce( residual_update_dir(cells) * search_dir(cells) );
 
    dq(cells) += alpha * search_dir(cells);
 
    residual(cells) -= alpha * residual_update_dir(cells);


    //==== Compute norm of residual, and check for convergence ====
    residual_norm_old = residual_norm;
    residual_norm     = +reduce(residual(cells) * residual(cells));
    writeln("Iteration ", iter, ": residual_norm = ", residual_norm);
    if residual_norm < tolerance then break;
    if iter==maxiter then writeln("Warning: conjugate gradient method failed to converge.");


    //==== Update directions for search and residual update ====
    beta                = residual_norm / residual_norm_old;
    search_dir(cells)   = residual + beta * search_dir(cells);

    bc.homogeneousGhostFill(search_dir);
    backwardEulerOperator(residual_update_dir,  search_dir, diffusivity, dt);
    
  }
  //<=== CG iteration <===
  
  
  //==== Update solution ====
  //----------------------------------------------
  // Recalling that dq = q.time_layer(1).value...
  //----------------------------------------------
  dq(cells) += q_current(cells);
  sol.time_layer(1).time = time_current + dt;
  sol.time_layer(1) <=> sol.time_layer(2);
    
}
//<=== inverseBEOperator method <===
//<=================================




//===> Backward Euler diffusion operator ===>
//==========================================>
//----------------------------------------------------------------
//      L(q) = q + dt*flux_divergence(q), values on [cells]
//
// As with FluxDivergence, ghost cells must be filled beforehand.
//----------------------------------------------------------------
def RectangularGrid.backwardEulerOperator(
  Lq:          [ext_cells] real,
  q:           [ext_cells] real,
  diffusivity: real,
  dt:          real
){

  fluxDivergence(Lq,  q, diffusivity);
  Lq(cells) = q(cells) + dt * Lq(cells);

}
//<==========================================
//<=== Backward Euler diffusion operator <===




//===> fluxDivergence method ===>
//===============================>
//-----------------------------------------------------------
// Calculates the flux divergence of q.  Note that the ghost
// cells of q must be filled beforehand, as in general this
// may need to be done using either bc.ghostFill or
// bc.homogeneousGhostFill.
//-----------------------------------------------------------
def RectangularGrid.fluxDivergence(
  flux_div:    [ext_cells] real,
  q:           [ext_cells] real,
  diffusivity: real
){

  forall cell_pretuple in cells {

    //==== 1D/tuple fix ====
    var cell = tuplify(cell_pretuple);


    //==== Initialize to zero ====
    flux_div(cell) = 0.0;

    
    //==== Apply contribution in each dimension ====
    for d in dimensions {
      var cell_shift: dimension*int; // initializes to 0
      cell_shift(d) = 2;  // Use IntVect here?
      flux_div(cell) += (      -q(cell-cell_shift) 
			                    + 2.0*q(cell) 
			                    -     q(cell+cell_shift)
			                         ) / dx(d)**2;
    }

  }


  //==== Finish by multiplying the whole array by diffusivity ====
  flux_div(cells) *= diffusivity;


}
//<==============================
//<=== fluxDivergence method <===





