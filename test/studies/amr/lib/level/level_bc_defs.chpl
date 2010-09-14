use level_base_defs;
use level_array_defs;


//===> LevelBC class ===>
//======================>
class LevelBC {
  
  const level: Level;

  //===> fillGhostCells method ===>
  //==============================>
  def fillGhostCells(q: LevelArray, t: real) {

    fillBoundaryGhosts(q, t);
    q.fillSharedGhosts();

  }
  //<=== fillGhostCells method <===
  //<==============================
  
  
  //===> fillGhostCellsHomogeneous method ===>
  //=========================================>
  def fillGhostCellsHomogeneous(q: LevelArray) {

    fillBoundaryGhostsHomogeneous(q);    
    q.fillSharedGhosts();

  }
  //<=== fillGhostCellsHomogeneous method <===
  //<=========================================
  

  //==== Dummy methods; provided in derived class ====
  def fillBoundaryGhosts(q: LevelArray, t: real) {}
  def fillBoundaryGhostsHomogeneous(q: LevelArray) {}

}
//<=== LevelBC class <===
//<======================