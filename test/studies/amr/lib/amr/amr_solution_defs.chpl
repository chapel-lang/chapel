use amr_base_defs;
use amr_array_defs;
use level_solution_defs;


//|~~~~~~~~~~~~~~~~~~~~~~~~\
//|    AMRSolution class    >
//|________________________/
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


  def initialize() {
    for level in hierarchy.levels do
      level_solutions(level) = new LevelSolution(level = level);
  }

}
// /~~~~~~~~~~~~~~~~~~~~~~~|
//<    AMRSolution class   |
// \_______________________|




//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
//|    AMRSolution.setToFunction method    >
//|_______________________________________/
def AMRSolution.setToFunction(
  initial_condition: func(dimension*real, real),
  time_in:           real
) {

  for level in hierarchy.levels do
    level_solutions(level).setToFunction(initial_condition, time_in);

}
// /~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
//<    AMRSolution.setToFunction method    |
// \_______________________________________|





//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
//|    AMRSolution.clawOutput method    >
//|____________________________________/
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

}
// /~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
//<    AMRSolution.clawOutput method    |
// \____________________________________|






/* def initial_condition ( x: dimension*real ) { */
/*   var f: real = 1.0; */
/*   for d in dimensions do */
/*     f *= exp(-10 * (x(d) + 0.0)**2); */
/*   return f; */
/* } */

/* def main { */

/*   var hierarchy = hierarchyFromInputFile("space.txt"); */
/*   var amr_solution = new AMRSolution(hierarchy = hierarchy); */
  
/*   amr_solution.setToFunction(initial_condition, 0.3); */
/*   amr_solution.clawOutput(0); */

/* } */