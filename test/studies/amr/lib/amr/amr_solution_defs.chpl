use amr_array_defs;
use cf_solution_interface_defs;


//|"""""""""""""""""""""""""\
//|===> AMRSolution class ===>
//|_________________________/
//------------------------------------------------------------------------
// An AMRSolution is treated as a union of LevelSolutions, each of which
// represents the PDE solution on a time interval.  Note that AMRArrays
// are *not* used except as temporary structures for output.  In general,
// the two layers of space data in each LevelSolution exist at different
// times, and within each level, their references will be swapped as
// computations proceed.
//------------------------------------------------------------------------
class AMRSolution {

  const hierarchy: AMRHierarchy;

  var level_solutions: [hierarchy.levels] LevelSolution;

  var coarse_interfaces: [hierarchy.levels] CFSolutionInterface;
  var fine_interfaces:   [hierarchy.levels] CFSolutionInterface;


  def coarserSolution(lev_sol: LevelSolution){
    var coarser_level = hierarchy.coarserLevel( lev_sol.level );

    var coarser_solution: LevelSolution = nil;
    if coarser_level then
      coarser_solution = level_solutions(coarser_level);

    return coarser_solution;
  }


  def finerSolution(lev_sol: LevelSolution){
    var finer_level = hierarchy.finerLevel( lev_sol.level );

    var finer_solution: LevelSolution = nil;
    if finer_level then
      finer_solution = level_solutions(finer_level);

    return finer_solution;
  }


  def initialize() {
    //==== Initialize level solutions ====
    for level in hierarchy.levels do
      level_solutions(level) = new LevelSolution(level = level);


    //==== Initialize coarse solution interfaces ====
    for level in hierarchy.fine_levels {
      var fine_solution   = level_solutions(level);
      var coarse_solution = coarserSolution(fine_solution);
      var level_interface = hierarchy.coarse_interfaces(level);

      coarse_interfaces(level) = new CFSolutionInterface(coarse_solution,
							 fine_solution,
							 level_interface);
    }

    //==== Match fine solution interfaces ====
    for level in hierarchy.coarse_levels do
      fine_interfaces(level) = coarse_interfaces(hierarchy.finerLevel(level));

  }

}
// /"""""""""""""""""""""""""/
//<=== AMRSolution class <==<
// \_________________________\




//|""""""""""""""""""""""""""""""""""""""""\
//|===> AMRSolution.setToFunction method ===>
//|________________________________________/
def AMRSolution.setToFunction(
  initial_condition: func(dimension*real, real),
  time_in:           real
) {

  for level in hierarchy.levels do
    level_solutions(level).setToFunction(initial_condition, time_in);

}
// /""""""""""""""""""""""""""""""""""""""""/
//<=== AMRSolution.setToFunction method <==<
// \________________________________________\





//|"""""""""""""""""""""""""""""""""""""\
//|===> AMRSolution.clawOutput method ===>
//|_____________________________________/
def AMRSolution.clawOutput(frame_number: int) {

  //==== Get final time from coarsest level ====
  var time = level_solutions(hierarchy.top_level).time(2);


  //==== Create AMRArray to link LevelArrays ====
  var amr_array = new AMRArray(hierarchy = hierarchy);
  for level in hierarchy.levels {

    //==== Check that time on this level agrees with coarsest time ====
    if abs(level_solutions(level).time(2) - time) > 1.0e-8 then
      writeln("Warning: Possible time inconsistency between levels upon output.");

    //==== Place latest space_data in amr_array ====
    amr_array.level_arrays(level) = level_solutions(level).space_data(2);
  }


  //==== Use AMRArray.clawOutput to actually produce data files ====
  amr_array.clawOutput(time, frame_number);

  //==== Clean up ====
  delete amr_array;

}
// /"""""""""""""""""""""""""""""""""""""/
//<=== AMRSolution.clawOutput method <==<
// \_____________________________________\








def main {

  def initial_condition ( x: dimension*real ) {
    var f: real = 1.0;
    for d in dimensions do
      f *= exp(-10 * (x(d) + 0.0)**2);
    return f;
  }


  var hierarchy    = readHierarchy("input_hierarchy.txt");
  var amr_solution = new AMRSolution(hierarchy = hierarchy);
  amr_solution.setToFunction(initial_condition, 0.0);

  amr_solution.clawOutput(0);


}