//===> Description ===>
//
// Defines a time-stepper for diffusion via Backward Euler
// on a single BaseLevel.
//
//<=== Description <===


use level_base_defs;
use level_solution_defs;
use level_bc_defs;



//===> BaseLevel.stepDiffusionBE method ===>
//=========================================>
def BaseLevel.stepDiffusionBE(
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
  var grid_res_norm_squares: [child_grids] real;
  var grid_inner_products: [child_grids] real;
  var alpha, beta, 
    level_res_norm_square, old_level_res_norm_square: real;


  //==== Set rhs and initial guess ====
  coforall grid in child_grids {
    bc.ghostFill(q_current(grid), t_new);
    grid.fluxDivergence(rhs(grid), q_current(grid), diffusivity);
    rhs(grid).value(grid.cells) *= -dt;

    dq(grid).value(grid.cells) = rhs(grid).value(grid.cells);
  }
  


  //==== Initialize residual ====
  coforall grid in child_grids {
    grid.homogeneousBEOperator(residual(grid), 
                               dq(grid),
                               bc, diffusivity, dt);

    residual(grid).value(grid.cells) = rhs(grid).value(grid.cells)
                                       - residual(grid).value(grid.cells);

    grid_res_norm_squares(grid) = +reduce(residual(grid).value(grid.cells)**2);
  }

  level_res_norm_square = +reduce(grid_res_norm_squares);




  //==== Initialize search direction ====
  coforall grid in child_grids {
    search_dir(grid).value(grid.cells) = residual(grid).value(grid.cells);
  }


  //==== Initialize residual update direction ====
  coforall grid in child_grids {
    grid.homogeneousBEOperator(residual_update_dir(grid),
                               search_dir(grid),
                               bc, diffusivity, dt);

  }
  //<=== Initialize <===


  
  
  
  //===> CG iteration ===>
  for iter in [1..maxiter] {


    //===> Update solution and residual ===>
    coforall grid in child_grids {
      grid_inner_products(grid) = +reduce( residual_update_dir(grid).value(grid.cells)
                                            * search_dir(grid).value(grid.cells) );
    }

    alpha = +reduce(grid_inner_products);
    alpha = level_res_norm_square / alpha;

    coforall grid in child_grids {
      dq(grid).value(grid.cells)       += alpha * search_dir(grid).value(grid.cells);

      residual(grid).value(grid.cells) -= alpha * residual_update_dir(grid).value(grid.cells);

      grid_res_norm_squares(grid) = +reduce( residual(grid).value(grid.cells)**2 );
    }
    //<=== Update solution and residual <===

    // for grid in child_grids {
    //   for cell in grid.cells {
    //     writeln(cell, "  ", dq(grid).value(cell));
    //     writeln(cell, "  ", residual(grid).value(cell));
    //   }
    // }
    // writeln("alpha = ", alpha);
    // halt();


    //==== Update residual norm, and check for convergence ====
    old_level_res_norm_square = level_res_norm_square;
    level_res_norm_square     = +reduce(grid_res_norm_squares);
    writeln("Iteration ", iter, ": residual_norm = ", sqrt(level_res_norm_square));
    if sqrt(level_res_norm_square) < tolerance then break;
    if iter == maxiter then writeln("Warning: conjugate gradient method failed to converge.");



    //==== Update directions for search and residual update ====
    beta = level_res_norm_square / old_level_res_norm_square;
    
    coforall grid in child_grids {
      search_dir(grid).value(grid.cells) = residual(grid).value(grid.cells)
                                           + beta * search_dir(grid).value(grid.cells);
    }
    
    coforall grid in child_grids {
      grid.homogeneousBEOperator(residual_update_dir(grid),
                                 search_dir(grid),
                                 bc, diffusivity, dt);
    }

  }
  //<=== CG iteration <===


  //===> Update solution ===>
  coforall grid in child_grids {
    dq(grid).value(grid.cells) += q_current(grid).value(grid.cells);
  }
  level_sol.time(1) = t_current;
  level_sol.time(2) = t_current + dt;
  level_sol.space_data(1) <=> level_sol.space_data(2);
  

}
//<=== BaseLevel.stepDiffusionBE method <===
//<=========================================






//===> BaseGrid.homogeneousBEOperator method ===>
//==============================================>
//----------------------------------------------------------------
//      L(q) = q + dt*flux_divergence(q), values on [cells]
//
// As with FluxDivergence, ghost cells must be filled beforehand.
//----------------------------------------------------------------
def LevelGrid.homogeneousBEOperator(
  Lq:          LevelGridArray,
  q:           LevelGridArray,
  bc:          LevelBC,
  diffusivity: real,
  dt:          real
){

  bc.homogeneousGhostFill(q);
  fluxDivergence(Lq,  q, diffusivity);
  Lq.value(cells) = q.value(cells) + dt * Lq.value(cells);

}
//<=== BaseGrid.homogeneousBEOperator method <===
//<==============================================




//===> BaseGrid.fluxDivergence method ===>
//=======================================>
def LevelGrid.fluxDivergence(
  flux_div:    LevelGridArray,
  q:           LevelGridArray,
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
//<=== BaseGrid.fluxDivergence method <===
//<=======================================
