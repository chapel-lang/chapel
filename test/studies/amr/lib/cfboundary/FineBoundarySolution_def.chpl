
use CFDataTransfer;
use LevelSolution_def;


//|\"""""""""""""""""""""""""""""""""""|\
//| >    FineBoundarySolution class    | >
//|/___________________________________|/
class FineBoundarySolution {
  
  const cf_boundary:     CFBoundary;
    
  var old_data:     FineBoundaryArray;
  var current_data: FineBoundaryArray;
  var old_time:     real;
  var current_time: real;
  
  
  //|\''''''''''''''''''''|\
  //| >    constructor    | >
  //|/....................|/
  def FineBoundarySolution(cf_boundary: CFBoundary)
 {
    this.cf_boundary = cf_boundary;
    
    old_data     = new FineBoundaryArray(cf_boundary);
    current_data = new FineBoundaryArray(cf_boundary);
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
//< |    FineBoundarySolution class    < |
// \|___________________________________\|



//|\"""""""""""""""""""""""""""""""""""""""""|\
//| >    FineBoundarySolution.fill_Linear    | >
//|/_________________________________________|/
def FineBoundarySolution.fill_Linear(
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
//< |    FineBoundarySolution.fill_Linear    < |
// \|_________________________________________\|




//|\"""""""""""""""""""""""""""""""""""""""""|\
//| >    LevelArray.getFineBoundaryValues    | >
//|/_________________________________________|/
def LevelArray.getFineBoundaryValues(
  fine_boundary_solution: FineBoundarySolution,
  time:                   real)
{
  //==== Pull times from fine_boundary_solution ====
  const t1 = fine_boundary_solution.old_time;
  const t2 = fine_boundary_solution.current_time;
  
  //==== Safety check the level ====
  assert(this.level == fine_boundary_solution.cf_boundary.fine_level);

  //==== Safety check the requested time ====
  assert(time > t1 - 1.0E-8  &&  time < t2 + 1.0E-8,
	 "Warning: LevelArray.getFineBoundaryValues\n" +
	 "Requesting fine data at time " + format("%8.4E",time) + "\n" +
	 "fine_boundary_solution.old_time =     " + format("%8.4E", t1) + "\n" +
	 "fine_boundary_solution.current_time = " + format("%8.4E", t2));

  //==== Interpolation parameters ====
  const c2 = (time - t1) / (t2 - t1);
  const c1 = 1 - c2;

  
  //===> Fill boundary one grid at a time ===>
  for grid in this.level.grids {

    //==== Handle one block of data at a time ====
    for (array1, array2) in fine_boundary_solution.array_pairs(grid)
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
//< |    LevelArray.getFineBoundaryValues    < |
// \|_________________________________________\|









// //|\""""""""""""""""""""""""""""""""""""""""""""|\
// //| >    CFBoundarySolution.fillFineBoundary    | >
// //|/____________________________________________|/
// def CFBoundarySolution.fillFineBoundary(
//   q: LevelArray,
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
//   q: GridArray,
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