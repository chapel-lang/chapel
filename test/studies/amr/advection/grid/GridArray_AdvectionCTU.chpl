use GridArray_def;


//|""""""""""""""""""""""""""""""""""\
//|===> GridArray.storeCTUOperator ===>
//|__________________________________/
def GridArray.storeCTUOperator(
  q_in:     GridArray,
  velocity: dimension*real,
  dt:       real
)
{
  //-----------------------------------------------------------
  //---- Domain of alignments is [0..1, 0..1, ..., 0..1].
  //---- In each dimension, alignment 1 indicates that it's
  //---- aligned with the cell, and alignment 0 indicates that
  //---- it's upwind.  (What is "it"?)
  //-----------------------------------------------------------
  var alignments = (0..1)**dimension;

  
  //===> Calculate new value ===>
  forall precell in grid.cells {

    //==== 1D/tuple fix ====
    var cell = tuplify(precell);


    value(cell) = 0.0;
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
          volume_fraction *= abs(velocity(d))*dt / grid.dx(d);
        if velocity(d) < 0.0 then
          upwind_cell(d) = cell(d)+2;
        else // the case velocity(d)==0 can refer to any cell
          upwind_cell(d) = cell(d)-2;
	      }
	      else {
          volume_fraction *= 1.0 - abs(velocity(d))*dt / grid.dx(d);
          upwind_cell(d) = cell(d);
        }
          
      }

          
      //==== Update cell value ====
      value(cell) += volume_fraction * q_in.value(upwind_cell);
  
    }
    //<=== Update for each alignment <===
    
  }
  //<=== Calculate new value <===
    
}
// /""""""""""""""""""""""""""""""""""/
//<=== GridArray.storeCTUOperator <==<
// \__________________________________\
