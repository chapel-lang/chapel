use level_base_defs;
use level_array_defs;


//===> LevelBC class ===>
//======================>
class LevelBC {
  
  var level: BaseLevel;

  //===> fillGhostCells method ===>
  //==============================>
  def fillGhostCells(q: LevelArray, t: real) {

    level.fillSharedGhosts(q);
    fillBoundaryGhosts(q, t);

  }
  //<=== fillGhostCells method <===
  //<==============================
  
  
  //===> fillGhostCellsHomogeneous method ===>
  //=========================================>
  def fillGhostCellsHomogeneous(q: LevelArray) {
    
    level.fillSharedGhosts(q);
    fillBoundaryGhostsHomogeneous(q);

  }
  //<=== fillGhostCellsHomogeneous method <===
  //<=========================================
  

  //==== Dummy methods; provied in derived class ====
  def fillBoundaryGhosts(q: LevelArray, t: real) {}
  def fillBoundaryGhostsHomogeneous(q: LevelArray) {}

}
//<=== LevelBC class <===
//<======================