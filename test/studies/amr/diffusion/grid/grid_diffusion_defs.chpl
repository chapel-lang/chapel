//===> Description ===>
//
// Initializes grid_base_defs and creates a method for a bacward Euler
// diffusion solver.
//
//<=== Description <===


use grid_solution_defs;
use grid_bc_defs;




//===> Grid.stepDiffusionBE method ===>
//====================================>
def Grid.stepDiffusionBE(
  sol:         GridSolution,
  bc:          GridBC,
  diffusivity: real,
  dt:          real, 
  tolerance:   real
){

  //==== Safety check ====
  assert(sol.grid == this);


  //===== Assign names to solution components ====
  var dq        = sol.space_data(1);  // will eventually be q_new, but is dq until the end
  var q_current = sol.space_data(2);
  var t_current = sol.time(2);
  var t_new     = sol.time(2) + dt;


  //==== Storage allocations ====
  var rhs                 = new GridArray(grid = this);
  var residual            = new GridArray(grid = this);
  var search_dir          = new GridArray(grid = this);
  var residual_update_dir = new GridArray(grid = this);

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
  bc.fillGhostCells(q_current, t_new);
  fluxDivergence(rhs, q_current, diffusivity);
  rhs *= -dt;


  //==== Initial guess ====
  dq = 1.0 * rhs;

  //==== Initialize residual ====
  //------------------------------------------------------------
  // residual = rhs - (dq + dt*flux_divergence(dq))
  //------------------------------------------------------------
  homogeneousBEOperator(residual,
                        dq,
                        bc, diffusivity, dt);
  residual = rhs - residual;
  residual_norm_square = +reduce(residual.value(cells)**2);
  
  


  //==== Initialize search direction ====
  search_dir.value(cells) = residual.value(cells);


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
    inner_product = +reduce( residual_update_dir.value(cells) * search_dir.value(cells) );
    alpha = residual_norm_square / inner_product;
 
    dq       += alpha * search_dir;
    residual -= alpha * residual_update_dir;


    //==== Compute norm of residual, and check for convergence ====
    old_residual_norm_square = residual_norm_square;
    residual_norm_square     = +reduce(residual.value(cells)**2);
    writeln("Iteration ", iter, ": residual_norm = ", sqrt(residual_norm_square));
    if sqrt(residual_norm_square) < tolerance then break;
    if iter==maxiter then writeln("Warning: conjugate gradient method failed to converge.");


    //==== Update directions for search and residual update ====
    beta                = residual_norm_square / old_residual_norm_square;
    search_dir = residual + beta * search_dir;

    homogeneousBEOperator(residual_update_dir,
                          search_dir,
                          bc, diffusivity, dt);

  }
  //<=== CG iteration <===
  
  
  //==== Update solution ====
  //----------------------------------------------
  // Recalling that dq = q.time_layer(1).value...
  //----------------------------------------------
  dq += q_current;
  sol.time(1) = t_current;
  sol.time(2) = t_current + dt;
  sol.space_data(1) <=> sol.space_data(2);
}
//<=== Grid.stepDiffusionBE method <===
//<========================================




//===> Backward Euler diffusion operator ===>
//==========================================>
//----------------------------------------------------------------
//      L(q) = q + dt*flux_divergence(q), values on [cells]
//
// As with FluxDivergence, ghost cells must be filled beforehand.
//----------------------------------------------------------------
def Grid.homogeneousBEOperator(
  inout Lq:          GridArray,
  q:           GridArray,
  bc:          GridBC,
  diffusivity: real,
  dt:          real
){

  bc.fillGhostCellsHomogeneous(q);
  fluxDivergence(Lq,  q, diffusivity);
  Lq = q + dt * Lq;

}
//<==========================================
//<=== Backward Euler diffusion operator <===




//===> fluxDivergence method ===>
//===============================>
//-----------------------------------------------------------
// Calculates the flux divergence of q.  Note that the ghost
// cells of q must be filled beforehand
//-----------------------------------------------------------
def Grid.fluxDivergence(
  inout flux_div:  GridArray,
  q:               GridArray,
  diffusivity:     real)
{

  forall cell_pretuple in cells {

    //==== 1D/tuple fix ====
    var cell = tuplify(cell_pretuple);


    //==== Initialize to zero ====
    flux_div.value(cell) = 0.0;

    
    //==== Apply contribution in each dimension ====
    for d in dimensions {
      var cell_shift: dimension*int; // initializes to 0
      cell_shift(d) = 2;  // Use IntVect here?
      flux_div.value(cell) += (     -q.value(cell-cell_shift) 
                               + 2.0*q.value(cell) 
                               -     q.value(cell+cell_shift)
                              ) / dx(d)**2;
    }

  }


  //==== Finish by multiplying the whole array by diffusivity ====
  flux_div *= diffusivity;


}
//<==============================
//<=== fluxDivergence method <===





//===> ZeroFluxDiffusionGridBC derived class ===>
//==============================================>
class ZeroFluxDiffusionGridBC: GridBC {
  
  //===> fillGhostCells method ===>
  //==============================>
  def fillGhostCells(q: GridArray, t: real) {
    fillGhostCellsHomogeneous(q);
  }
  //<=== fillGhostCells method <===
  //<==============================


  //===> fillGhostCellsHomogeneous method ===>
  //=========================================>
  def fillGhostCellsHomogeneous(q: GridArray) {
    
    for loc in ghost_locations {
      var shift: dimension*int;

      for d in dimensions {
	if loc(d) == loc1d.low then
	  shift(d) = 2;
	else if loc(d) == loc1d.high then
	  shift(d) = -2;
      }

      forall precell in grid.ghost_domain_set(loc) {
	var cell = tuplify(precell);
	q.value(cell) = q.value(cell+shift);
      }

    }

  }
  //<=== fillGhostCellsHomogeneous method <===
  //<=========================================

  
}
//<=== ZeroFluxDiffusionGridBC derived class <===
//<==============================================