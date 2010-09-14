//===> Description ===>
//
// Initializes grid_base_defs and creates a method for a CTU
// (corner transport upwind) solver.
//
//<=== Description <===


use grid_solution_defs;
use grid_bc_defs;



//===> Grid.stepAdvectionCTU method ===>
//=====================================>
//-------------------------------------------------------------
// The output of the oprator will be stored in q.old, and then
// q.old and q.current will be swapped.
//-------------------------------------------------------------
def Grid.stepAdvectionCTU(
  sol:      GridSolution,
  bc:       GridBC,
  velocity: dimension*real,
  dt:       real
){

  //==== Assign names to solution components ====
  var q_current = sol.space_data(2);
  var q_new     = sol.space_data(1); // overwriting old values
  var t_current = sol.time(2);
  var t_new     = sol.time(2) + dt;


  //==== Fill ghost cells ====
  bc.fillGhostCells(q_current, t_current);
  

  //-----------------------------------------------------------
  //---- Domain of alignments is [0..1, 0..1, ..., 0..1].
  //---- In each dimension, alignment 1 indicates that it's
  //---- aligned with the cell, and alignment 0 indicates that
  //---- it's upwind.  (What is "it"?)
  //-----------------------------------------------------------
  var ranges: dimension*range;
  for d in dimensions do ranges(d) = 0..1;
  var alignments: domain(dimension) = ranges;

  
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
    
  
  
  //==== Update solution structure ====
  sol.time(1) = t_current;
  sol.time(2) = t_new;
  sol.space_data(1) <=> sol.space_data(2);

}
//<=====================================
//<=== Grid.stepAdvectionCTU method <===






//===> ZeroInflowAdvectionGridBC class ===>
//========================================>
class ZeroInflowAdvectionGridBC: GridBC {
  
  
  //===> ghostFill method ===>
  //=========================>
  def fillGhostCells(q: GridArray, t: real) {
    //==== This type of BC is homogeneous ====
    fillGhostCellsHomogeneous(q);
  }
  //<=== ghostFill method <===
  //<=========================
  
  
  //===> homogeneousGhostFill method ===>
  //====================================>
  def fillGhostCellsHomogeneous(q: GridArray) {

    for loc in ghost_locations {
      forall cell in ghost_cells(loc) do
	q.value(cell) = 0.0;
    }
/*     for cell in grid.ghost_cells do */
/*       q.value(cell) = 0.0; */
    
  }
  //<=== homogeneousGhostFill method <===
  //<====================================
  
}
//<=== ZeroInflowAdvectionGridBC class <===
//<========================================