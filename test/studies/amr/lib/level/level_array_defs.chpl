use level_base_defs;
use grid_array_defs;


//=====================================================>
//==================== LEVEL ARRAYS ===================>
//=====================================================>


//===> LevelArray class ===>
//=========================>
class LevelArray {
  const level: BaseLevel;

  var child_arrays: [level.child_grids] GridArray;

  def initialize() {

    for grid in level.child_grids do
      child_arrays(grid) = new GridArray(grid = grid);
                                              
  }

  def this(grid: BaseGrid){
    return child_arrays(grid);
  }
}
//<=== LevelArray class <===
//<=========================




//===> BaseLevel.setLevelArray method ===>
//=======================================>
def BaseLevel.setLevelArray(
  q: LevelArray,
  f: func(dimension*real, real)
){

  coforall grid in child_grids {
    grid.setGridArray(q(grid), f);
  }

}
//<=== BaseLevel.setLevelArray method ====
//<=======================================




//===> BaseLevel.fillSharedGhosts method ===>
//==========================================>
def BaseLevel.fillSharedGhosts(q: LevelArray) {

  coforall grid in child_grids {    
    coforall sib in child_grids {

      var shared_domain = shared_ghosts(grid)(sib);
      q(grid).value(shared_domain) = q(sib).value(shared_domain);

    }
  }

}
//<=== BaseLevel.fillSharedGhosts method <===
//<==========================================



//<=====================================================
//<=================== LEVEL ARRAYS ====================
//<=====================================================







//=======================================================>
//==================== OUTPUT METHODS ===================>
//=======================================================>


//===> clawOutput method ===>
//==========================>
//-----------------------------------------------------------------------
// Writes both a time file and a solution file for a given frame number.
//-----------------------------------------------------------------------
def BaseLevel.clawOutput(
  q:            LevelArray,
  time:         real,
  frame_number: int
){

  //==== Make sure q lives on this level ====
  assert(q.level == this);


  //==== Names of output files ====
  var frame_string:      string = format("%04i", frame_number),
      time_filename:     string = "_output/fort.t" + frame_string,
      solution_filename: string = "_output/fort.q" + frame_string;


  //==== Time file ====
  var n_grids = 0;
  for grid in child_grids do n_grids += 1;

  var outfile = new file(time_filename, FileAccessMode.write);
  outfile.open();  
  writeTimeFile(time, 1, n_grids, 0, outfile);
  outfile.close();
  delete outfile;
  
  
  //==== Solution file ====
  outfile = new file(solution_filename, FileAccessMode.write);
  outfile.open();
  writeLevelArray(q, 1, outfile);  // AMR_level=1 for single-level output
  outfile.close();
  delete outfile;

}
//<=== clawOutput method <===
//<==========================




//===> BaseLevel.writeLevelArray method ===>
//=========================================>
def BaseLevel.writeLevelArray(
  q:         LevelArray,
  AMR_level: int,
  outfile:   file
){
  var grid_number = 0;
  for grid in child_grids {
    grid_number += 1;
    grid.writeGridArray(q.child_arrays(grid), grid_number, 1, outfile);
    outfile.writeln("  ");
  }

}
//<=== BaseLevel.writeLevelArray method <===
//<=========================================


//<=======================================================
//<=================== OUTPUT METHODS ====================
//<=======================================================
