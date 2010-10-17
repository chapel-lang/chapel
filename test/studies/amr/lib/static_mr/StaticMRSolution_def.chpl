
use StaticMRHierarchy_def;
use FineBoundarySolution_def;


//|\"""""""""""""""""""""""""""""""|\
//| >    StaticMRSolution class    | >
//|/_______________________________|/
//-------------------------------------------------------------------
// An StaticMRSolution is treated as a union of LevelSolutions, each
// of which represents the PDE solution on a time interval.
//-------------------------------------------------------------------
class StaticMRSolution {

  const hierarchy: StaticMRHierarchy;

  var level_solutions:         [hierarchy.level_indices] LevelSolution;
  var fine_boundary_solutions: [hierarchy.level_indices] FineBoundarySolution;

  def time { return level_solutions(1).current_time; }


  //|\''''''''''''''''''''|\
  //| >    constructor    | >
  //|/....................|/
  //----------------------------------------------------------------------------
  // Have to use initialize() method to allocate fields based on the hierarchy.
  // Intended signature:
  //     StaticMRSolution(hierarchy: StaticMRHierarchy)
  //----------------------------------------------------------------------------
  def initialize() {
    for i in hierarchy.level_indices {

      //==== Initialize LevelSolution ====
      level_solutions(i) = new LevelSolution(hierarchy.levels(i));

      //==== If level is fine, make a FineBoundarySolution ===
      if i>1 then
        fine_boundary_solutions(i) = new FineBoundarySolution(hierarchy.fine_boundaries(i-1));

    }

  }
  // /|''''''''''''''''''''/|
  //< |    constructor    < |
  // \|....................\|

}
// /|"""""""""""""""""""""""""""""""/|
//< |    StaticMRSolution class    < |
// \|_______________________________\|




//|\"""""""""""""""""""""""""""""""""""""""|\
//| >    StaticMRSolution.setToFunction    | >
//|/_______________________________________|/
def StaticMRSolution.setToFunction(
  initial_condition: func(dimension*real, real),
  time_in:           real
) {

  for i in hierarchy.level_indices do
    level_solutions(i).setToFunction(initial_condition, time_in);

}
// /|"""""""""""""""""""""""""""""""""""""""/|
//< |    StaticMRSolution.setToFunction    < |
// \|_______________________________________\|





//|\""""""""""""""""""""""""""""""""""""""""|\
//| >    StaticMRSolution output methods    | >
//|/________________________________________|/
def StaticMRSolution.clawOutput(frame_number: int)
{

  //==== Names of output files ====
  var frame_string:       string = format("%04i", frame_number);
  var time_file_name:     string = "_output/fort.t" + frame_string;
  var solution_file_name: string = "_output/fort.q" + frame_string;


  //==== Time file ====
  var n_grids: int = 0;
  for level in hierarchy.levels do
    n_grids += level.grids.numIndices;

  const time_file = new file(time_file_name, FileAccessMode.write);
  time_file.open();
  writeTimeFile(time, 1, n_grids, 1, time_file);
  time_file.close();


  //==== Solution file ====
  const solution_file = new file(solution_file_name, FileAccessMode.write);
  solution_file.open();
  this.write(solution_file);

}



//----------------------------------------------------------------
// Proceeds down the indexed_levels, calling the LevelArray.write
// method on each corresponding LevelArray.
//----------------------------------------------------------------
def StaticMRSolution.write(outfile: file){

  var base_grid_number = 1;

  for i in hierarchy.level_indices {
    level_solutions(i).current_data.write(i, base_grid_number, outfile);
    base_grid_number += hierarchy.levels(i).grids.numIndices;
  }

}
// /|""""""""""""""""""""""""""""""""""""""""/|
//< |    StaticMRSolution output methods    < |
// \|________________________________________\|








def main {

  def initial_condition ( x: dimension*real ) {
    var f: real = 1.0;
    for d in dimensions do
      f *= exp(-10 * (x(d) + 0.0)**2);
    return f;
  }


  var hierarchy    = readHierarchy("input_hierarchy.txt");
  var smr_solution = new StaticMRSolution(hierarchy);
  smr_solution.setToFunction(initial_condition, 0.0);

  smr_solution.clawOutput(0);


}