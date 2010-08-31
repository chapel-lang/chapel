use level_base_defs;
use level_array_defs;


//===> LevelBC class ===>
//======================>
class LevelBC {
  
  var level: BaseLevel;

  //===> fillGhostCells method ===>
  //==============================>
  def fillGhostCells(q: LevelArray, t: real) {

    q.fillSharedGhosts();
    fillBoundaryGhosts(q, t);

  }
  //<=== fillGhostCells method <===
  //<==============================
  
  
  //===> fillGhostCellsHomogeneous method ===>
  //=========================================>
  def fillGhostCellsHomogeneous(q: LevelArray) {
    
    q.fillSharedGhosts();
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