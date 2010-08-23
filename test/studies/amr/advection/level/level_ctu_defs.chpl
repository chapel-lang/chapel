//===> Description ===>
//
// Definitions for an advection/CTU solver on a
// BaseLevel.
//
//<=== Description <===


use level_base_defs;
use level_solution_defs;
use level_bc_defs;



//===> BaseLevel.stepAdvectionCTU method ===>
//==========================================>
def BaseLevel.stepAdvectionCTU(
  level_sol:  LevelSolution,
  bc:         LevelBC,
  velocity:   dimension*real,
  dt:         real)
{

  //==== Safety check ====
  assert(level_sol.level == this);


  //==== Assign names to solution components ====
  var q_current = level_sol.space_data(2);
  var t_current = level_sol.time(2);
  var q_new     = level_sol.space_data(1);
  var t_new     = t_current + dt;


  //==== Fill ghost cells of q_current ====
  bc.fillGhostCells(q_current, t_current);


  //==== Step each grid ====
  coforall grid in child_grids do
    grid.stepAdvectionCTU(q_new(grid), q_current(grid), velocity, dt);


  //==== Update solution structure ====
  level_sol.space_data(1) = q_current;
  level_sol.space_data(2) = q_new;
  level_sol.time(1) = t_current;
  level_sol.time(2) = t_new;


}
//<=== BaseLevel.stepAdvectionCTU method <===
//<==========================================




//===> BaseGrid.stepAdvectionCTU method ===>
//==========================================>
def BaseGrid.stepAdvectionCTU(
  q_new:     GridArray,
  q_current: GridArray,
  velocity:  dimension*real,
  dt:        real)
{

  //==== Safety check ====
  assert(q_new.grid == this);
  assert(q_current.grid == this);



  var alignments: domain(dimension);
  alignments = alignments.expand(1);
  
  //===> Calculate new value ===>
  forall precell in cells {

    //==== 1D/tuple fix ====
    var cell = tuplify(precell);


    q_new.value(cell) = 0.0;
    var volume_fraction: real;
    var upwind_cell: dimension*int;


    //===> Update for each alignment ===>
    for prealignment in alignments {

      //==== 1D/tuple fix ====
      var alignment = tuplify(prealignment);

        
      volume_fraction = 1.0;
      for d in dimensions {
        //-------------------------------------------------------
        // For each alignment, set the volume fraction and index
        // of the upwind cell. 
        //-------------------------------------------------------
        if alignment(d) == 0 then {
          volume_fraction *= abs(velocity(d))*dt / dx(d);
        if velocity(d) < 0.0 then
          upwind_cell(d) = cell(d)+2;
        else // the case velocity(d)==0 can refer to any cell
          upwind_cell(d) = cell(d)-2;
	      }
	      else {
          volume_fraction *= 1.0 - abs(velocity(d))*dt / dx(d);
          upwind_cell(d) = cell(d);
        }
          
      }

          
      //==== Update cell value ====
      q_new.value(cell) += volume_fraction * q_current.value(upwind_cell);
  
    }
    //<=== Update for each alignment <===
    
  }
  //<=== Calculate new value <===


}
//<=== LevelGrid.stepAdvectionCTU method <===
//<==========================================






//===> ZeroInflowAdvectionLevelBC derived class ===>
//=================================================>
class ZeroInflowAdvectionLevelBC: LevelBC {


  def fillBoundaryGhosts(q: LevelArray, t: real){
    fillBoundaryGhostsHomogeneous(q);
  }


  def fillBoundaryGhostsHomogeneous(q: LevelArray){

    coforall grid in level.child_grids {

      forall cell in level.boundary_ghosts(grid) do
	q(grid).value(cell) = 0.0;

    }
  }  
 
}
//<=== ZeroInflowAdvectionLevelBC derived class <===
//<=================================================
