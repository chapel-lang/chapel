//===> Description ===>
//
// Defines a time-stepper for diffusion via Backward Euler
// on a single Level.
//
//<=== Description <===


use level_base_defs;
use level_solution_defs;
use level_bc_defs;



//===> Level.stepDiffusionBE method ===>
//=====================================>
def Level.stepDiffusionBE(
  level_sol:    LevelSolution,
  bc:           LevelBC,
  diffusivity:  real,
  dt:           real,
  tolerance:    real)
{

  //==== Safety check ====
  assert(level_sol.level == this);


  //==== Assign names to solution components ====
  var q_current = level_sol.space_data(2);
  var t_current = level_sol.time(2);
  var dq        = level_sol.space_data(1);
  var t_new     = t_current + dt;


  //==== Allocate space for CG storage ====
  var rhs                 = new LevelArray(level = this);
  var residual            = new LevelArray(level = this);
  var search_dir          = new LevelArray(level = this);
  var residual_update_dir = new LevelArray(level = this);


  //==== Maximum number of iterations before giving up ====
  var maxiter = 100;



  //===> Initialize ===>
  var grid_res_norm_squares: [grids] real;
  var grid_inner_products:   [grids] real;
  var alpha, beta, 
    level_res_norm_square, old_level_res_norm_square: real;


  //==== Set rhs and initial guess ====
  bc.fillGhostCells(q_current, t_new);
  fluxDivergence(rhs, q_current, diffusivity);


  for grid in grids {
    //    rhs(grid).value(grid.cells) *= -dt;
    rhs(grid) *= -dt;

    //    dq(grid).value(grid.cells) = rhs(grid).value(grid.cells);
    dq(grid) = 1.0*rhs(grid);
  }
  

  //==== Initialize residual ====
  homogeneousBEOperator(residual, dq, bc, diffusivity, dt);


  for grid in grids {
    //    residual(grid).value(grid.cells) = rhs(grid).value(grid.cells)
    //                                       - residual(grid).value(grid.cells);
    residual(grid) = rhs(grid) - residual(grid);			       

    grid_res_norm_squares(grid) = +reduce(residual(grid).value(grid.cells)**2);
  }

  level_res_norm_square = +reduce(grid_res_norm_squares);


  //==== Initialize search direction ====
/*   search_dir = 1.0*residual; */

  coforall grid in grids {
    //    search_dir(grid).value(grid.cells) = residual(grid).value(grid.cells);
    search_dir(grid) = 1.0*residual(grid);
  }


  //==== Initialize residual update direction ====
  homogeneousBEOperator(residual_update_dir, search_dir, bc, diffusivity, dt);

  //<=== Initialize <===


  
  
  
  //===> CG iteration ===>
  for iter in [1..maxiter] {

    //===> Update solution and residual ===>
    for grid in grids {
      grid_inner_products(grid) = +reduce( residual_update_dir(grid).value(grid.cells)
                                            * search_dir(grid).value(grid.cells) );
    }

    alpha = +reduce(grid_inner_products);
    alpha = level_res_norm_square / alpha;


    for grid in grids {
      //      dq(grid).value(grid.cells)       += alpha * search_dir(grid).value(grid.cells);
      dq(grid) += alpha * search_dir(grid);

      //      residual(grid).value(grid.cells) -= alpha * residual_update_dir(grid).value(grid.cells);
      residual(grid) -= alpha * residual_update_dir(grid);

      grid_res_norm_squares(grid) = +reduce( residual(grid).value(grid.cells)**2 );
    }
    //<=== Update solution and residual <===



    //==== Update residual norm, and check for convergence ====
    old_level_res_norm_square = level_res_norm_square;
    level_res_norm_square     = +reduce(grid_res_norm_squares);
    writeln("Iteration ", iter, ": residual_norm = ", sqrt(level_res_norm_square));
    if sqrt(level_res_norm_square) < tolerance then break;
    if iter == maxiter then writeln("Warning: conjugate gradient method failed to converge.");



    //==== Update directions for search and residual update ====
    beta = level_res_norm_square / old_level_res_norm_square;
    
    for grid in grids {
      //      search_dir(grid).value(grid.cells) = residual(grid).value(grid.cells)
      //                                   + beta * search_dir(grid).value(grid.cells);
      search_dir(grid) = residual(grid) + beta*search_dir(grid);

    }
    
    homogeneousBEOperator(residual_update_dir, search_dir, bc, diffusivity, dt);

  }
  //<=== CG iteration <===


  //===> Update solution ===>
  for grid in grids {
    //    dq(grid).value(grid.cells) += q_current(grid).value(grid.cells);
    dq(grid) += q_current(grid);
  }
  level_sol.time(1) = t_current;
  level_sol.time(2) = t_current + dt;
  level_sol.space_data(1) <=> level_sol.space_data(2);
  

}
//<=== Level.stepDiffusionBE method <===
//<=====================================






//===> Level.homogeneousBEOperator method ===>
//===========================================>
//----------------------------------------------------------------
//      L(q) = q + dt*flux_divergence(q), values on [cells]
//----------------------------------------------------------------
def Level.homogeneousBEOperator(
  Lq:          LevelArray,
  q:           LevelArray,
  bc:          LevelBC,
  diffusivity: real,
  dt:          real)
{

  bc.fillGhostCellsHomogeneous(q);

  fluxDivergence(Lq, q, diffusivity);

  coforall grid in grids do
    Lq(grid).value(grid.cells) = q(grid).value(grid.cells) 
                                  + dt * Lq(grid).value(grid.cells);

}
//<=== Level.homogeneousBEOperator method <===
//<===========================================




//===> Level.fluxDivergence method ===>
//====================================>
def Level.fluxDivergence(
  flux_div:    LevelArray,
  q:           LevelArray,
  diffusivity: real)
{

  coforall grid in grids do
    grid.fluxDivergence(flux_div(grid), q(grid), diffusivity);

}
//<=== Level.fluxDivergence method <===
//<====================================






//===> Grid.fluxDivergence method ===>
//===================================>
def Grid.fluxDivergence(
  flux_div:    GridArray,
  q:           GridArray,
  diffusivity: real)
{

  //==== Safety checks ====
  assert(flux_div.grid == this);
  assert(q.grid == this);



  //==== Calculate flux divergence cell-by-cell ====
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
  flux_div.value(cells) *= diffusivity;

}
//<=== Grid.fluxDivergence method <===
//<===================================






//===> ZeroFluxDiffusionLevelBC derived class ===>
//===============================================>
class ZeroFluxDiffusionLevelBC: LevelBC {
  
  def fillBoundaryGhosts(q: LevelArray, t: real) {
    fillBoundaryGhostsHomogeneous(q);
  }

  
  def fillBoundaryGhostsHomogeneous(q: LevelArray) {
    
    for grid in level.grids {
    
      for loc in ghost_locations {
	var shift: dimension*int;

	for d in dimensions {
	  if loc(d) == loc1d.low then shift(d) = 2;
	  else if loc(d) == loc1d.high then shift(d) = -2;
	}

	forall cell in grid.ghost_domain_set(loc) do
	  q(grid).value(cell) = q(grid).value(cell+shift);

      }

    }
  }

}
//<=== ZeroFluxDiffusionLevelBC derived class <===
//<===============================================