use level_base_defs;

//===> LevelBC class ===>
//======================>
class LevelBC {
  
  var level: BaseLevel;

  //===> fillGridGhosts method ===>
  //==============================>
  def fillGhostCells(grid_array: LevelGridArray, t: real) {

    var grid = (grid_array.grid : LevelGrid);

    grid.fillLevelGhosts(grid_array);
    fillBoundaryGhosts(grid_array, t);

  }
  //<=== fillGridGhosts method <===
  //<==============================
  
  
  //===> fillGridGhostsHomogeneous method ===>
  //=========================================>
  def fillGhostCellsHomogeneous(grid_array: LevelGridArray) {
    
    var grid = (grid_array.grid : LevelGrid);

    grid.fillLevelGhosts(grid_array);
    fillBoundaryGhostsHomogeneous(grid_array);

  }
  //<=== fillGridGhostsHomogeneous method <===
  //<=========================================
  

  //==== Dummy methods; provied in derived class ====
  def fillBoundaryGhosts(grid_array: LevelGridArray, t: real) {}
  def fillBoundaryGhostsHomogeneous(grid_array: LevelGridArray) {}

}
//<=== LevelBC class <===
//<======================