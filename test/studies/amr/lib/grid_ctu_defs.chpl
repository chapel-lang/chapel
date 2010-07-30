//===> Description ===>
//
// Initializes grid_base_defs and creates a method for a CTU
// (corner transport upwind) solver.
//
//<=== Description <===


use grid_base_defs;




//===> RectangularGrid.stepCTU method ===>
//=======================================>
def RectangularGrid.stepCTU(q:        GridSolution,
                            velocity: dimension*real,
                            dt:       real
                           ){


  //==== Initialize ====
  var new_value: [cells] real;


  //-----------------------------------------------------------
  //---- Domain of alignments is [0..1, 0..1, ..., 0..1].
  //---- In each dimension, alignment 1 indicates that it's
  //---- aligned with the cell, and alignment 0 indicates that
  //---- it's upwind.  (What is "it"?)
  //-----------------------------------------------------------
  var alignments: domain(dimension);
  alignments = alignments.expand(1);
  
  //===> Calculate new value ===>
  forall precell in cells {

    //==== 1D/tuple fix ====
    var cell = tuplify(precell);


    new_value(cell) = 0.0;
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
      new_value(cell) += volume_fraction * q.value(upwind_cell);
  
    }
    //<=== Update for each alignment <===
    
  }
  //<=== Calculate new value <===
    
  
  
  //==== Update solution ====
  q.value(cells) = new_value;
  q.time += dt;

}
//<=======================================
//<=== RectangularGrid.stepCTU method <===
