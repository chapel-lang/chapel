//===> Description ===>
//
// Initializes grid_base_defs and creates a method for a bacward Euler
// diffusion solver.
//
//<=== Description <===


use grid_base_defs;
use grid_solution_defs;
use grid_bc_defs;




//===> BaseGrid.stepBE method ===>
//===============================>
def BaseGrid.stepBE(
  sol:         ScalarGridSolution,
  bc:          GridBC,
  diffusivity: real,
  dt:          real, 
  tolerance:   real
){

  //==== Safety check ====
  assert(sol.grid == this);


  //===== Assign names to solution components ====
  var dq        => sol.space_data(1).value;  // will eventually be q_new, but is dq until the end
  var q_current => sol.space_data(2).value;
  var t_current = sol.time(2);
  var t_new     = sol.time(2) + dt;


  //==== Storage allocations ====
  var rhs, residual, search_dir, residual_update_dir: [ext_cells] real;
  var residual_norm_square, old_residual_norm_square: real;
  var alpha, beta, inner_product: real;
  

  //==== Maximum number of iterations before giving up ====
  var maxiter = 100;


  //==== Calculate the rhs for conjugate gradients ====
  //----------------------------------------------------------------------
  // This is calculated by filling q_current's ghost cells with boundary
  // data from the *next* time, and then calculating -dt*flux_divergence.
  // I haven't actually seen this done before, but it makes possible
  // a clean decoupling of the "ghostFill" and "homogeneousGhostFill"
  // methods.
  //----------------------------------------------------------------------
  bc.ghostFill(q_current, t_new);
  fluxDivergence(rhs, q_current, diffusivity);
  rhs(cells) *= -dt;


  //==== Initial guess ====
  dq(cells) = rhs(cells);


  //==== Initialize residual ====
  //------------------------------------------------------------
  // residual = rhs - (dq + dt*flux_divergence(dq))
  //------------------------------------------------------------
  homogeneousBEOperator(residual,
                        dq,
                        bc, diffusivity, dt);
  residual(cells) = rhs(cells) - residual(cells);
  residual_norm_square = +reduce(residual(cells)**2);
  
  


  //==== Initialize search direction ====
  search_dir(cells) = residual(cells);


  //==== Initialize residual update direction ====
  //--------------------------------------------------
  // Initializes to homogeneousBEOperator(search_dir)
  //--------------------------------------------------
  homogeneousBEOperator(residual_update_dir,
                        search_dir, 
                        bc, diffusivity, dt);
  
  
  //===> CG iteration ===>
  for iter in [1..maxiter] {
 
    //==== Update the solution and residual ====
    inner_product = +reduce( residual_update_dir(cells) * search_dir(cells) );
    alpha = residual_norm_square / inner_product;
 
    dq(cells)       += alpha * search_dir(cells);
    residual(cells) -= alpha * residual_update_dir(cells);

    // for cell in cells {
    //   writeln(cell, "  ", dq(cell));
    //   writeln(cell, "  ", residual(cell));
    // }
    // writeln("alpha = ", alpha);
    // halt();


    //==== Compute norm of residual, and check for convergence ====
    old_residual_norm_square = residual_norm_square;
    residual_norm_square     = +reduce(residual(cells)**2);
    writeln("Iteration ", iter, ": residual_norm = ", sqrt(residual_norm_square));
    if sqrt(residual_norm_square) < tolerance then break;
    if iter==maxiter then writeln("Warning: conjugate gradient method failed to converge.");


    //==== Update directions for search and residual update ====
    beta                = residual_norm_square / old_residual_norm_square;
    search_dir(cells)   = residual(cells) + beta * search_dir(cells);

    homogeneousBEOperator(residual_update_dir,
                          search_dir,
                          bc, diffusivity, dt);

  }
  //<=== CG iteration <===
  
  
  //==== Update solution ====
  //----------------------------------------------
  // Recalling that dq = q.time_layer(1).value...
  //----------------------------------------------
  dq(cells) += q_current(cells);
  sol.time(1) = t_current;
  sol.time(2) = t_current + dt;
  sol.space_data(1) <=> sol.space_data(2);
}
//<=== BaseGrid.stepBE method <===
//<===============================




//===> Backward Euler diffusion operator ===>
//==========================================>
//----------------------------------------------------------------
//      L(q) = q + dt*flux_divergence(q), values on [cells]
//
// As with FluxDivergence, ghost cells must be filled beforehand.
//----------------------------------------------------------------
def BaseGrid.homogeneousBEOperator(
  Lq:          [ext_cells] real,
  q:           [ext_cells] real,
  bc:          GridBC,
  diffusivity: real,
  dt:          real
){

  bc.homogeneousGhostFill(q);
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
def BaseGrid.fluxDivergence(
  flux_div:    [ext_cells] real,
  q:           [ext_cells] real,
  diffusivity: real)
{

  forall cell_pretuple in cells {

    //==== 1D/tuple fix ====
    var cell = tuplify(cell_pretuple);


    //==== Initialize to zero ====
    flux_div(cell) = 0.0;

    
    //==== Apply contribution in each dimension ====
    for d in dimensions {
      var cell_shift: dimension*int; // initializes to 0
      cell_shift(d) = 2;  // Use IntVect here?
      flux_div(cell) += (     -q(cell-cell_shift) 
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





//===> ZeroFluxDiffusionGridBC derived class ===>
//==============================================>
class ZeroFluxDiffusionGridBC: GridBC {
  
  
  //===> ghostFill method ===>
  //=========================>
  def ghostFill(q: [grid.ext_cells] real, t: real) {
    homogeneousGhostFill(q);
  }
  //<=== ghostFill method <===
  //<=========================


  //===> homogeneousGhostFill method ===>
  //====================================>
  def homogeneousGhostFill(q: [grid.ext_cells] real) {
    
    for precell in grid.ghost_cells {
        var cell = tuplify(precell);

	//==== Set target_cell to the nearest interior cell ====
        var target_cell = cell;
        for d in dimensions {
          target_cell(d) = min(target_cell(d), grid.cells.dim(d).high);
          target_cell(d) = max(target_cell(d), grid.cells.dim(d).low);
	}
        
	//==== Copy target_cell's value to the ghost cell ====
        q(cell) = q(target_cell);
    }

  }
  //<=== homogeneousGhostFill method <===
  //<====================================

  
}
//<=== ZeroFluxDiffusionGridBC derived class <===
//<==============================================