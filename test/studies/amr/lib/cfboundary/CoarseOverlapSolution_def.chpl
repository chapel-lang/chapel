
use CFDataTransfer;
use LevelSolution_def;


//|\"""""""""""""""""""""""""""""""""""|\
//| >    CoarseOverlapSolution class    | >
//|/___________________________________|/
class CoarseOverlapSolution {
  
  const cf_boundary:     CFBoundary;
    
  var old_data:     CoarseOverlapArray;
  var current_data: CoarseOverlapArray;
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
  def CoarseOverlapSolution(cf_boundary: CFBoundary)
 {
    this.cf_boundary = cf_boundary;
    
    old_data     = new CoarseOverlapArray(cf_boundary);
    current_data = new CoarseOverlapArray(cf_boundary);
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
  def array_pairs(grid: Grid) {
    for (old_array,current_array) in (old_data(grid),current_data(grid)) do
      yield (old_array,current_array);
  }
  // /|'''''''''''''''''''''''/|
  //< |    these iterator    < |
  // \|.......................\|
  
}
// /|"""""""""""""""""""""""""""""""""""/|
//< |    CoarseOverlapSolution class    < |
// \|___________________________________\|



//|\"""""""""""""""""""""""""""""""""""""""""|\
//| >    CoarseOverlapSolution.fill_Linear    | >
//|/_________________________________________|/
def CoarseOverlapSolution.fill_Linear(
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
// /|"""""""""""""""""""""""""""""""""""""""""/|
//< |    CoarseOverlapSolution.fill_Linear    < |
// \|_________________________________________\|




//|\"""""""""""""""""""""""""""""""""""""""""|\
//| >    LevelVariable.getFineBoundaryValues    | >
//|/_________________________________________|/
def LevelVariable.getFineBoundaryValues(
  coarse_overlap_solution: CoarseOverlapSolution,
  time:                   real)
{
  //==== Pull times from coarse_overlap_solution ====
  const t1 = coarse_overlap_solution.old_time;
  const t2 = coarse_overlap_solution.current_time;
  
  //==== Safety check the level ====
  assert(this.level == coarse_overlap_solution.cf_boundary.fine_level);

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
    for (array1, array2) in coarse_overlap_solution.array_pairs(grid)
    {
      //==== Safety check; this iteration seems a bit fragile ====
      assert(array1.Domain == array2.Domain);

      //==== Interpolate ====
      this(grid).value(array1.Domain) = c1 * array1.value + c2 * array2.value;
    }
  }
  //<=== Fill boundary one grid at a time <===
  
}
// /|"""""""""""""""""""""""""""""""""""""""""/|
//< |    LevelVariable.getFineBoundaryValues    < |
// \|_________________________________________\|









// //|\""""""""""""""""""""""""""""""""""""""""""""|\
// //| >    CFBoundarySolution.fillFineBoundary    | >
// //|/____________________________________________|/
// def CFBoundarySolution.fillFineBoundary(
//   q: LevelVariable,
//   t: real)
// {
//   //==== Safety check the fine level ====
//   assert(q.level == cf_boundary.fine_level);
// 
// 
//   //==== Safety check the requested time ====
//   assert(t > old_time-1.0E-8 && t < current_time+1.0E-8,
//   "Warning: CFBoundarySolution.fillFineArrayBoundary\n" +
//   "Requesting fine data at time " + format("%8.4E",t) + "\n" +
//   "old_time =     " + format("%8.4E", old_time) + "\n" +
//   "current_time = " + format("%8.4E", current_time));
// 
// 
//   //==== Provide data for each grid array ====
//   for grid in q.level.grids do
//     fillFineBoundary(q(grid), t);
// 
// }
// 
// 
// def CFSolutionInterface.fillFineBoundary(
//   q: GridVariable,
//   t: real)
// {
// 
//   //==== Interpolation parameters ====
//   var a2 = (t - old_time) / (current_time - old_time);
//   var a1 = 1 - a2;
// 
// 
//   //==== Interpolate one block at a time ====
//   for (old_array, curent_array) in (old_data.array_sets(q.grid), current_data.array_sets(q.grid)) 
//   {
//     //==== Safety check; this iteration seems slightly fragile ====
//     assert(old_array.dom == current_array.dom);
// 
//     q.value(old_array.dom) = a1 * old_ghost_array.value 
//                             + a2 * current_ghost_array.value;
//   }
// 
// }
// // /|""""""""""""""""""""""""""""""""""""""""""""/|
// //< |    CFBoundarySolution.fillFineBoundary    < |
// // \|____________________________________________\|





// //|\"""""""""""""""""""""""""""""""""""""""""""""""""|\
// //| >    CFBoundarySolution.correctCoarseSolution    | >
// //|/_________________________________________________|/
// def CFBoundarySolution.correctCoarseSolution()
// {
//   assert( abs(coarse_solution.current_time - fine_solution.current_time) < 1.0e-8,
//     "Warning: correctCoarseInterior_Linear\n" +
//     "  Time of fine solution may not agree with time of coarse solution");
// 
//   coarse_solution.current_data.interpolateFromFine_Linear(fine_solution.current_data);
// }
// // /|"""""""""""""""""""""""""""""""""""""""""""""""""/|
// //< |    CFBoundarySolution.correctCoarseSolution    < |
// // \|_________________________________________________\|