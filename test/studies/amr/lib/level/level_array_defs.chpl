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
  var _promotionType: real;

  def initialize() {

    for grid in level.grids do
      grid_arrays(grid) = new GridArray(grid = grid);
                                              
  }

  def this(grid: BaseGrid){
    return grid_arrays(grid);
  }


  //===> these() iterator ===>
  //---------------------------------------------------------------------
  // Seems like it would be ideal to chain this with the leader/follower
  // for GridArrays.
  //---------------------------------------------------------------------
  def these() {
    for grid in level.grids {
      for cell in grid.cells do
	yield grid_arrays(grid).value(cell);
    }
  }

  def these(param tag: iterator)
  where tag == iterator.leader {
/*     coforall grid in level.grids { */
/*       yield grid; */
/*     } */
    yield true;
  }

  def these(param tag: iterator, follower) var
  where tag == iterator.follower {
/*     for cell in follower.cells do */
/*       yield grid_arrays(follower).value(cell); */
    for grid in level.grids {
      for cell in grid.cells do
	yield grid_arrays(grid).value(cell);
    }

  }
  //<=== these() iterator <===

}
//<=== LevelArray class <===
//<=========================




//===> LevelArray assignment overloads ===>
//========================================>
def =(q: LevelArray, rvalue)
where rvalue.type != LevelArray && rvalue.type != real
{
  forall (x,y) in (q,rvalue) do
    x = y;
}

def =(q: LevelArray, rvalue: real) {
  forall x in q do
    x = rvalue;
}
//<=== LevelArray assignment overloads <===
//<========================================




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
  frame_number: int)
{

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
  write(1, 1, outfile);  // AMR_level=1 and base_grid_number=1 for single-level output
  outfile.close();
  delete outfile;

}
//<=== clawOutput method <===
//<==========================




//===> LevelArray.write method ===>
//================================>
def LevelArray.write(
  AMR_level:        int,
  base_grid_number: int,
  outfile:          file)
{

  var grid_number = base_grid_number;
  for grid in level.grids {
    grid_arrays(grid).write(grid_number, AMR_level, outfile);
    outfile.writeln("  ");
    grid_number += 1;
  }

}
//<=== LevelArray.write method <===
//<================================


//<=======================================================
//<=================== OUTPUT METHODS ====================
//<=======================================================
