use AMRHierarchy_def;
use LevelArray_def;


//===> AMRArrayClass ===>
//======================>
class AMRArray {

  const hierarchy: AMRHierarchy;

  var level_arrays: [hierarchy.levels] LevelArray;

}
//<=== AMRArrayClass <===
//<======================




//===> AMRArray.clawOutput method ===>
//===================================>
//------------------------------------------------------------------------
// Writes full Clawpack output at a given time.  Most of the work is done
// by the "write" method, which handles output of the spatial data.
//------------------------------------------------------------------------
def AMRArray.clawOutput(
  time: real,
  frame_number: int)
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
  write(solution_file);

}
//<=== AMRArray.clawOutput method <===
//<===================================





//|"""""""""""""""""""""""""""""\
//|===> AMRArray.write method ===>
//|_____________________________/
//----------------------------------------------------------------
// Proceeds down the indexed_levels, calling the LevelArray.write
// method on each corresponding LevelArray.
//----------------------------------------------------------------
def AMRArray.write(outfile: file){

  var base_grid_number = 1;

  for level in hierarchy.ordered_levels {
    level_arrays(level).write(hierarchy.level_numbers(level),
			      base_grid_number, outfile);
    base_grid_number += level.grids.numIndices;
  }

}
// /"""""""""""""""""""""""""""""/
//<=== AMRArray.write method <==<
// \_____________________________\





def main {

  def initial_condition ( x: dimension*real ) {
    var f: real = 1.0;
    for d in dimensions do
      f *= exp(-10 * (x(d) + 0.0)**2);
    return f;
  }


  var hierarchy = readHierarchy("input_hierarchy.txt");
  var amr_array = new AMRArray(hierarchy = hierarchy);
  var level_array: LevelArray;

  for level in hierarchy.levels {
    level_array = new LevelArray(level = level);
    level_array.setToFunction(initial_condition);
    amr_array.level_arrays(level) = level_array;
  }

  amr_array.clawOutput(0.0, 0);


}