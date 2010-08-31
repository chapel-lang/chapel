use level_base_defs;
use grid_array_defs;


//=====================================================>
//==================== LEVEL ARRAYS ===================>
//=====================================================>


//===> LevelArray class ===>
//=========================>
class LevelArray {
  const level: BaseLevel;

  var grid_arrays: [level.grids] GridArray;

  def initialize() {

    for grid in level.grids do
      grid_arrays(grid) = new GridArray(grid = grid);
                                              
  }

  def this(grid: BaseGrid){
    return grid_arrays(grid);
  }
}
//<=== LevelArray class <===
//<=========================




//===> LevelArray.setToFunction method ===>
//========================================>
//-----------------------------------------------------------------
// Sets LevelArray.value to the analytical function f evaluated on
// the level.
//-----------------------------------------------------------------
def LevelArray.setToFunction(
  f: func(dimension*real, real)
){

  coforall grid in level.grids {
    grid_arrays(grid).setToFunction(f);
  }

}
//<=== BaseLevel.setLevelArray method ====
//<=======================================




//===> LevelArray.fillSharedGhosts method ===>
//===========================================>
def LevelArray.fillSharedGhosts() {

  coforall grid in level.grids {    
    coforall sib in level.grids {

      var shared_domain = level.shared_ghosts(grid)(sib);
      grid_arrays(grid).value(shared_domain) 
	= grid_arrays(sib).value(shared_domain);

    }
  }

}
//<=== LevelArray.fillSharedGhosts method <===
//<===========================================



//<=====================================================
//<=================== LEVEL ARRAYS ====================
//<=====================================================







//=======================================================>
//==================== OUTPUT METHODS ===================>
//=======================================================>


//===> LevelArray.clawOutput method ===>
//=====================================>
//-----------------------------------------------------------------------
// Writes both a time file and a solution file for a given frame number.
//-----------------------------------------------------------------------
def LevelArray.clawOutput(
  time:         real,
  frame_number: int
){

  //==== Names of output files ====
  var frame_string:      string = format("%04i", frame_number),
      time_filename:     string = "_output/fort.t" + frame_string,
      solution_filename: string = "_output/fort.q" + frame_string;


  //==== Time file ====
  var n_grids = level.grids.numIndices;

  var outfile = new file(time_filename, FileAccessMode.write);
  outfile.open();  
  writeTimeFile(time, 1, n_grids, 0, outfile);
  outfile.close();
  delete outfile;
  
  
  //==== Solution file ====
  outfile = new file(solution_filename, FileAccessMode.write);
  outfile.open();
  write(1, outfile);  // AMR_level=1 for single-level output
  outfile.close();
  delete outfile;

}
//<=== clawOutput method <===
//<==========================




//===> LevelArray.write method ===>
//================================>
def LevelArray.write(
  AMR_level: int,
  outfile:   file
){
  var grid_number = 0;
  for grid in level.grids {
    grid_number += 1;
    grid_arrays(grid).write(grid_number, 1, outfile);
    outfile.writeln("  ");
  }

}
//<=== LevelArray.write method <===
//<================================


//<=======================================================
//<=================== OUTPUT METHODS ====================
//<=======================================================
