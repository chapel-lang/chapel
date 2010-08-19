use level_base_defs;

//===> LevelBC class ===>
//======================>
class LevelBC {
  
  var level: BaseLevel;

  //===> fillGridGhosts method ===>
  //==============================>
  def fillGhostCells(grid_array: LevelGridArray, t: real) {

    var grid = grid_array.level_grid;
    grid.fillSharedGhosts(grid_array);
    fillBoundaryGhosts(grid_array, t);

  }
  //<=== fillGridGhosts method <===
  //<==============================
  
  
  //===> fillGridGhostsHomogeneous method ===>
  //=========================================>
  def fillGhostCellsHomogeneous(grid_array: LevelGridArray) {
    
    var grid = grid_array.level_grid;
    grid.fillSharedGhosts(grid_array);
    fillBoundaryGhostsHomogeneous(grid_array);

  }
  //<=== fillGridGhostsHomogeneous method <===
  //<=========================================
  


 
  //===> fillBoundaryGhosts method ===>
  //==================================>
  def fillBoundaryGhosts(grid_array: LevelGridArray, t: real) {}
  //<=== fillBoundaryGhosts method <===
  //<==================================




  //===> fillBoundaryGhostsHomogeneous method ===>
  //=============================================>
  def fillBoundaryGhostsHomogeneous(grid_array: LevelGridArray) {}
  //<=== fillBoundaryGhostsHomogeneous method <===
  //<=============================================


}
//<=== LevelBC class <===
//<======================




//===> ZeroInflowAdvectionLevelBC derived class ===>
//=================================================>
class ZeroInflowAdvectionLevelBC: LevelBC {


  def fillBoundaryGhosts(grid_array: LevelGridArray, t: real){
    fillBoundaryGhostsHomogeneous(grid_array);
  }


  def fillBoundaryGhostsHomogeneous(grid_array: LevelGridArray){

    var grid = grid_array.level_grid;

    for cell in grid.boundary_ghosts {
      grid_array.value(cell) = 0.0;
    }
  }  
 
}
//<=== ZeroInflowLevelBC derived class <===
//<========================================



//===> ZeroFluxDiffusionLevelBC derived class ===>
//===============================================>
class ZeroFluxDiffusionLevelBC: LevelBC {
  
  def fillBoundaryGhosts(grid_array: LevelGridArray, t: real) {
    fillBoundaryGhostsHomogeneous(grid_array);
  }
  
  def fillBoundaryGhostsHomogeneous(grid_array: LevelGridArray) {
    
    var grid = grid_array.level_grid;
    
    for cell in grid.boundary_ghosts {
      var target_cell = cell;
      for d in dimensions {
        target_cell(d) = min(target_cell(d), grid.cells.dim(d).high);
        target_cell(d) = max(target_cell(d), grid.cells.dim(d).low);        
      }
      grid_array.value(cell) = grid_array.value(target_cell);
    }
    
  }
  
}
//<=== ZeroFluxDiffusionLevelBC derived class <===
//<===============================================