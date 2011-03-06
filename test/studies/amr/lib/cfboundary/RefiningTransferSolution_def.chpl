
use CFDataTransfer;
use LevelSolution_def;


//|\"""""""""""""""""""""""""""""""""""""""|\
//| >    RefiningTransferSolution class    | >
//|/_______________________________________|/

class RefiningTransferSolution {
  
  const cf_boundary:     CFBoundary;
    
  var old_data:     RefiningTransferVariable;
  var current_data: RefiningTransferVariable;
  var old_time:     real;
  var current_time: real;
  
  
  //|\''''''''''''''''|\
  //| >    clear()    | >
  //|/................|/
  def clear() {
    old_data.clear();
    delete old_data;
    current_data.clear();
    delete current_data;
  }
  // /|''''''''''''''''/|
  //< |    clear()    < |
  // \|................\|
  
  
  //|\''''''''''''''''''''|\
  //| >    constructor    | >
  //|/....................|/
  def RefiningTransferSolution(cf_boundary: CFBoundary)
 {
    this.cf_boundary = cf_boundary;
    
    old_data     = new RefiningTransferVariable(cf_boundary);
    current_data = new RefiningTransferVariable(cf_boundary);
  }
  // /|''''''''''''''''''''/|
  //< |    constructor    < |
  // \|....................\|
  
  
  //|\'''''''''''''''''''''''|\
  //| >    these iterator    | >
  //|/.......................|/
  //------------------------------------------------------------
  // Iterate over the ArraySets corresponding to a single Grid.
  //------------------------------------------------------------
  def array_pairs (grid: Grid) 
  {
    for (old_array,current_array) in (old_data(grid),current_data(grid)) do
      yield (old_array,current_array);
  }
  // /|'''''''''''''''''''''''/|
  //< |    these iterator    < |
  // \|.......................\|
  
}
// /|"""""""""""""""""""""""""""""""""""""""/|
//< |    RefiningTransferSolution class    < |
// \|_______________________________________\|





//|\"""""""""""""""""""""""""""""""""""""""""""""|\
//| >    RefiningTransferSolution.fill_Linear    | >
//|/_____________________________________________|/

def RefiningTransferSolution.fill_Linear(
  coarse_solution: LevelSolution)
{
  //==== Safety check ====
  assert(coarse_solution.level == cf_boundary.coarse_level);

  //==== Get data ====
  old_data.getCoarseValues_Linear(coarse_solution.old_data);
  current_data.getCoarseValues_Linear(coarse_solution.current_data);

  //==== Get times ====
  old_time = coarse_solution.old_time;
  current_time = coarse_solution.current_time;  
}
// /|"""""""""""""""""""""""""""""""""""""""""""""/|
//< |    RefiningTransferSolution.fill_Linear    < |
// \|_____________________________________________\|






//|\""""""""""""""""""""""""""""""""""""""""""""|\
//| >    LevelVariable.getFineBoundaryValues    | >
//|/____________________________________________|/

def LevelVariable.getFineBoundaryValues (
  refining_transfer_solution: RefiningTransferSolution,
  time:                       real )
{
  //==== Pull times from coarse_overlap_solution ====
  const t1 = refining_transfer_solution.old_time;
  const t2 = refining_transfer_solution.current_time;
  
  //==== Safety check the level ====
  assert(this.level == refining_transfer_solution.cf_boundary.fine_level);

  //==== Safety check the requested time ====
  assert(time > t1 - 1.0E-8  &&  time < t2 + 1.0E-8,
	 "Warning: LevelVariable.getFineBoundaryValues\n" +
	 "Requesting fine data at time " + format("%8.4E",time) + "\n" +
	 "coarse_overlap_solution.old_time =     " + format("%8.4E", t1) + "\n" +
	 "coarse_overlap_solution.current_time = " + format("%8.4E", t2));

  //==== Interpolation parameters ====
  const c2 = (time - t1) / (t2 - t1);
  const c1 = 1 - c2;

  
  //===> Fill boundary one grid at a time ===>
  for grid in this.level.grids {

    //==== Handle one block of data at a time ====
    for (array1, array2) in refining_transfer_solution.array_pairs(grid)
    {
      //==== Safety check; this iteration seems a bit fragile ====
      assert(array1.domain == array2.domain);

      //==== Interpolate ====
      this(grid).value(array1.domain) = c1 * array1 + c2 * array2;
    }
  }
  //<=== Fill boundary one grid at a time <===
  
}
// /|""""""""""""""""""""""""""""""""""""""""""""/|
//< |    LevelVariable.getFineBoundaryValues    < |
// \|____________________________________________\|









