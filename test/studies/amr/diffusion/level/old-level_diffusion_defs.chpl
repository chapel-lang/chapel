//===> Description ===>
//
// Defines a time-stepper for diffusion via Backward Euler
// on a single Level.
//
//<=== Description <===


use level_base_defs;
use level_solution_defs;
use level_bc_defs;










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