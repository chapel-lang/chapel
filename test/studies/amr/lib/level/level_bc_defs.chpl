use level_base_defs;

//===> LevelBC class ===>
//======================>
class LevelBC {
  
  var level: BaseLevel;

  //===> ghostFill method ===>
  //=========================>
  def ghostFill(q: LevelSolution) {}
  //<=== ghostFill method <===
  //<=========================
  
  
  //===> homogeneousGhostFill method ===>
  //====================================>
  def homogeneousGhostFill(q: LevelSolution) {}
  //<=== homogeneousGhostFill method <===
  //<====================================
  
}
//<=== LevelBC class <===
//<======================




//===> ZeroInflowLevelBC derived class ===>
//========================================>
class ZeroInflowLevelBC: LevelBC {
  
  //===> ghostFill method ===>
  //=========================>
  def ghostFill(q: LevelSolution) {
                  
    homogeneousGhostFill(q);
  }
  //<=== ghostFill method <===
  //<=========================
  
  
  //===> homogeneousGhostFill method ===>
  //====================================>
  def homogeneousGhostFill(q: LevelSolution) {

    coforall grid in level.child_grids {
      var q_grid = q.child_sols(grid);
      
      for d in dimensions {
        forall cell in grid.low_ghost_cells(d) {
          if !grid.ghost_is_shared.low(d).value(cell) then
            q_grid.value(cell) = 0.0;
        }
          
        forall cell in grid.high_ghost_cells(d) {
          if !grid.ghost_is_shared.high(d).value(cell) then
            q_grid.value(cell) = 0.0;
        }
      }
    }
  }
  //<=== homogeneousGhostFill method <===
  //<====================================
 
}
//<=== ZeroInflowLevelBC derived class <===
//<========================================