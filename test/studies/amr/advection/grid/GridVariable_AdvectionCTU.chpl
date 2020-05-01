public use GridVariable_def;


//|\""""""""""""""""""""""""""""""""""""""|\
//| >    GridVariable.storeCTUOperator    | >
//|/______________________________________|/

//----------------------------------------------------------------
// This method applies the CTU (corner transport upwind) operator 
// to GridVariable q_in, storing the result in this.value.
//
// The algorithm uses the data in q_in to construct a continuous
// function whose value is constant on each grid cell.  The
// data is then advected exactly by the specified velocity,
// and the continuous data is averaged over each cell.
//
// This implementation constructs the output value for each cell
// by iterating over all cells that lie upwind.  There are
// 2**dimension such cells, which are identified as being either
// 'aligned' or 'upwind' in each dimension relative to the output
// cell.
//----------------------------------------------------------------

proc GridVariable.storeCTUOperator(
  q_in:     unmanaged GridVariable,
  velocity: dimension*real,
  dt:       real)
{
  //----------------------------------------------------------
  // As we iterate over each cell of the output, the upwind
  // cells will be parameterized by their relative alignments
  // to the output cell.  In each dimension, we'll use 0 to
  // specifiy 'aligned', and 1 to specify 'upwind'.
  //----------------------------------------------------------
  var relative_alignments = (0..1)**dimension;

  
  //===> Calculate new value ===>
  forall precell in grid.cells {

    //==== 1D/tuple fix ====
    var cell = tuplify(precell);

    
    //==== Initialize ====
    value(cell) = 0.0;
    var volume_fraction: real;
    var upwind_cell: dimension*int;


    //===> Update for each alignment ===>
    for prealignment in relative_alignments {

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
          
          upwind_cell(d) = if velocity(d) < 0.0 then
                             cell(d)+2
                           else
                             cell(d)-2;
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
// /|""""""""""""""""""""""""""""""""""""""/|
//< |    GridVariable.storeCTUOperator    < |
// \|______________________________________\|
