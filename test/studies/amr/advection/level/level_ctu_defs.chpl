//===> Description ===>
//
// Definitions for an advection/CTU solver on a
// RefinementLevel.
//
//<=== Description <===


use grid_base_defs;
use level_base_defs;
use level_bcs;
use grid_ctu_defs;


//===> BaseLevel.stepCTU method ===>
//=================================>
def BaseLevel.stepCTU(lev_sol:  LevelSolution,
                      lev_bc:   LevelBC,
                      velocity: dimension*real,
                      dt:       real
                     ){

  //==== Fill ghost cells using boundary condition ====
  lev_bc.ghostFill(lev_sol);


  //==== Now fill ghost cells overlapped by neighbor grids ====
  coforall grid in child_grids {
    grid.copyFromNeighbors(lev_sol.child_sols(grid));    
  }


  //==== Take a time step on each grid ====
  coforall grid in child_grids {
    grid.stepCTU(lev_sol.child_sols(grid),
                 velocity,
                 dt);
  }


  //==== Update time ====
  lev_sol.time += dt;


}
//<=================================
//<=== BaseLevel.stepCTU method <===