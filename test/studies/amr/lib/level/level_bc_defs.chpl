use level_base_defs;

//===> LevelBC class ===>
//======================>
class LevelBC {
  
  var level: BaseLevel;

  //===> ghostFill method ===>
  //=========================>
  def ghostFill(grid_array: LevelGridArray, t: real) {

    var grid = grid_array.level_grid;
    grid.fillSharedGhosts(grid_array);
    boundaryFill(grid_array, t);

  }
  //<=== ghostFill method <===
  //<=========================
  
  
  //===> homogeneousGhostFill method ===>
  //====================================>
  def homogeneousGhostFill(grid_array: LevelGridArray) {
    
    var grid = grid_array.level_grid;
    grid.fillSharedGhosts(grid_array);
    homogeneousBoundaryFill(grid_array);

  }
  //<=== homogeneousGhostFill method <===
  //<====================================
  


 
  //===> ghostFill method on LevelGridArrays ===>
  //============================================>
  def boundaryFill(grid_array: LevelGridArray, t: real) {}
  //<=== ghostFill method on LevelGridArrays <===
  //<============================================




  //===> homogeneousGhostFill method on LevelGridArrays ===>
  //=======================================================>
  def homogeneousBoundaryFill(grid_array: LevelGridArray) {}
  //<=== homogeneousGhostFill method on LevelGridArrays <===
  //<=======================================================


}
//<=== LevelBC class <===
//<======================




//===> ZeroInflowLevelBC derived class ===>
//========================================>
class ZeroInflowLevelBC: LevelBC {


  def boundaryFill(grid_array: LevelGridArray, t: real){
    homogeneousGhostFill(grid_array);
  }


  def homogeneousBoundaryFill(grid_array: LevelGridArray){

    var grid = grid_array.level_grid;

    for cell in grid.boundary_ghosts {
      grid_array.value(cell) = 0.0;
    }
  }  
 
}
//<=== ZeroInflowLevelBC derived class <===
//<========================================