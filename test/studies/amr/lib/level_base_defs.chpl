//===> Description ===>
//
// Definitions for the BaseLevel class.
//
//<=== Description <===

use grid_base_defs;



//=======================================================================>
//==================== FUNDAMENTAL LEVEL DEFINITIONS  ===================>
//=======================================================================>


//===> LevelGrid derived class ===>
//================================>
class LevelGrid: RectangularGrid {

  var parent: BaseLevel;

  var neighbors:    domain(LevelGrid);
  var shared_cells: [neighbors] subdomain(ext_cells);

  var ghost_is_shared: GhostData(bool);


  //===> initialize method ===>
  //==========================>
  def initialize() {

    //===> Set fields via parent data and index bounds ===>
    forall d in dimensions {
      x_low(d)  = parent.x_low(d) + i_low(d)  * parent.dx(d)/2.0;
      x_high(d) = parent.x_low(d) + i_high(d) * parent.dx(d)/2.0;
      n_cells(d)= (i_high(d) - i_low(d)) / 2;
    }

    n_ghost_cells = parent.n_child_ghost_cells;
    //<=== Set fields via parent data and index bounds <===


    //==== Shared flags on ghost cells ====
    ghost_is_shared = new GhostData(bool);


    sanityChecks();

    setDerivedFields();

  }
  //<==========================
  //<=== initialize method <===



  //===> setNeighborData method ===>
  //=================================>
  //-------------------------------------------------------------
  // Locates neighboring grids, and stores the domains that this 
  // grid's ghost cells share with each neighbor's interior.
  //-------------------------------------------------------------
  def setNeighborData() {
  
    //==== To store intersection on each check ====
    var intersection: ext_cells.type;
  
    //===> Check each sibling for neighbors ===>
    for sib in parent.child_grids {

      intersection = intersectDomains(ext_cells, sib.cells);

      //==== If the intersection is nonempty, update neighbor data ====
      if intersection.dim(1).length > 0 {
	neighbors.add(sib);
	shared_cells(sib) = intersection;
      }

    }
    //<=== Check each sibling for neighbors <===



/*     //===> Flag shared cells on each ghost domain ===> */
/*     for nbr in neighbors { */

/*       for d in dimensions { */

/* 	//==== Low ghost cells ==== */
/* 	intersection = intersectDomains(low_ghost_cells(d), shared_cells(nbr)); */
/* 	for cell in intersection do */
/* 	  ghost_is_shared.low(d).value(cell) = true; */

/* 	//==== High ghost cells ==== */
/* 	intersection = intersectDomains(high_ghost_cells(d), shared_cells(nbr)); */
/* 	for cell in intersection do */
/* 	  ghost_is_shared.high(d).value(cell) = true; */

/*       } */
/*     } */
/*     //<=== Flag shared cells on each ghost domain <=== */
    
    
  }
  //<=== setNeighborData method <===
  //<===============================
  
  
  
  //===> copyFromNeighbors method ===>
  //=================================>
  //-------------------------------------------------------------
  // Copies data from q's siblings on neighboring grids into q's
  // ghost values.
  //-------------------------------------------------------------
  def copyFromNeighbors(q: LevelGridSolution) {
  
    //==== Make q lives on this grid ====
    assert(q.grid == this);
  
    for nbr in neighbors {
      //==== Locate sibling solution on neighbor grid ====
      var q_sib = q.parent.child_sols(nbr);
  
      //==== Copy values from shared cells ====
      q.value(shared_cells(nbr)) = q_sib.value(shared_cells(nbr));
    }
  
  }
  //<=== copyFromNeighbors method <===
  //<=================================


}
//<================================
//<=== LevelGrid derived class <===




//===> Definition of class BaseLevel ===>
//============================================>
class BaseLevel {

  var fixed: bool = false;
  
  var x_low, x_high:       dimension*real,
      n_cells:             dimension*int,
      n_child_ghost_cells: dimension*int,
      dx:                  dimension*real;

  var child_grids: domain(LevelGrid);  // associative domain of LevelGrids

  //===> initialize() method ===>
  def initialize() {
    dx = (x_high - x_low) / n_cells;
  }
  //<=== initialize() method <===

}
//<============================================
//<=== Definition of class BaseLevel <===



//===> BaseLevel.addGrid method ===>
//=======================================>
//--------------------------------------------------------
// This version is based on indices, and probably best to
// use in practice, as integer arithmetic is cleaner than
// real arithmetic.
//--------------------------------------------------------
def BaseLevel.addGrid(i_low:  dimension*int,
                      i_high: dimension*int
                     ) {
  
  assert(fixed == false);

  var new_grid = new LevelGrid(parent  = this,
                               i_low   = i_low,
                               i_high  = i_high);

  child_grids.add(new_grid);
}


//----------------------------------------------------------
// This version takes in real bounds, and snaps them to the
// level's discretization.  Mostly for testing purposes.
//----------------------------------------------------------
def BaseLevel.addGrid(x_low_grid:  dimension*real,
                      x_high_grid: dimension*real
                     ){

  //==== Make sure the level isn't fixed ====
  assert(fixed == false);


  //==== Find nearest interfaces to provided coordinates ====
  var i_low, i_high: dimension*int;
  for d in dimensions {
    i_low(d)  = 2 * round((x_low_grid(d)  - x_low(d)) / dx(d)) : int;
    i_high(d) = 2 * round((x_high_grid(d) - x_low(d)) / dx(d)) : int;
  }
  

  //==== Add grid ====
  var new_grid = new LevelGrid(parent  = this,
                               i_low   = i_low,
                               i_high  = i_high);
  child_grids.add(new_grid);
}
//<=================================
//<=== BaseLevel.addGrid method <===




//===> BaseLevel.fix method ===>
//=============================>
//----------------------------------------------------------------
// This method is meant to be called after all grids are added to
// the level.  Neighbor data is set on each grid, and other post-
// processing can be added here as needed.
//----------------------------------------------------------------
def BaseLevel.fix() {

  fixed = true;

  coforall grid in child_grids do
    grid.setNeighborData();

}
//<=============================
//<=== BaseLevel.fix method <===


//<===============================================================
//<=================== BASE LEVEL DEFINITIONS ====================
//<===============================================================






//========================================================>
//==================== LEVEL SOLUTIONS ===================>
//========================================================>


//===> LevelSolution class ===>
//============================>
class LevelSolution {
  var level:      BaseLevel;                              // parent level
  var child_sols: [level.child_grids] LevelGridSolution;  // child functions
  var time:       real;                                   // duh
}
//<============================
//<=== LevelSolution class <===




//===> LevelGridSolution derived class ===>
//========================================>
class LevelGridSolution: GridSolution {

  var parent: LevelSolution;

}
//<========================================
//<=== LevelGridSolution derived class <===




//===> BaseLevel.initializSolution method ===>
//===========================================>
//--------------------------------------------------------------------
// The argument 'initializer' can be either a first-class function
// or a TrueSolution.
//--------------------------------------------------------------------
def BaseLevel.initializeSolution(q: LevelSolution,
				 initializer
				){

  //==== Check that q lives on this level ====
  assert(q.level == this);


  //==== Form LevelGridSolutions ====
  coforall grid in child_grids {

    write("Creating new GridSolution...");
    var grid_sol = new LevelGridSolution(grid   = grid, 
					 parent = q);
		write("done.\n");

    grid.initializeSolution(grid_sol, initializer);

    q.child_sols(grid) = grid_sol;
  }
  

}
//<=== BaseLevel.initializeSoution method <===
//<===========================================


//<========================================================
//<=================== LEVEL SOLUTIONS ====================
//<========================================================






//============================================================>
//==================== BOUNDARY CONDITIONS ===================>
//============================================================>


//===> LevelBC class ===>
//======================>
class LevelBC {
  
  var level:     BaseLevel;
  var child_bcs: [level.child_grids] GridBC;

  //===> ghostFill method ===>
  //=========================>
  def ghostFill(q: LevelSolution) {
    
    //==== Make sure q can validly be filled ====
    assert(q.level == level);
    
    coforall grid in level.child_grids do
      child_bcs(grid).ghostFill(q.child_sols(grid));
  }
  //<=== ghostFill method <===
  //<=========================
}
//<======================
//<=== LevelBC class <===


//===> ZeroOrderExtrapLevelBC derived class ===>
//=============================================>
class ZeroOrderExtrapLevelBC: LevelBC {
  
  //===> initialize method ===>
  //==========================>
  def initialize() {
    
    for grid in level.child_grids {
      child_bcs(grid) = new ZeroOrderExtrapGridBC(grid = grid);
    }
    
  }
  //<=== initialize method <===
  //<==========================
  
}
//<=============================================
//<=== ZeroOrderExtrapLevelBC derived class <===


//<============================================================
//<=================== BOUNDARY CONDITIONS ====================
//<============================================================






//=======================================================>
//==================== OUTPUT METHODS ===================>
//=======================================================>


//===> clawOutput method ===>
//==========================>
//-----------------------------------------------------------------------
// Writes both a time file and a solution file for a given frame number.
//-----------------------------------------------------------------------
def BaseLevel.clawOutput(q:            LevelSolution,
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
  writeTimeFile(q.time, 1, n_grids, 0, outfile);
  outfile.close();
  delete outfile;
  
  
  //==== Solution file ====
  outfile = new file(solution_filename, FileAccessMode.write);
  outfile.open();
  writeSolution(q, 1, outfile);  // AMR_level=1 for single-level output
  outfile.close();
  delete outfile;

}
//<=== clawOutput method <===
//<==========================




//===> writeSolution method ===>
//=============================>
//---------------------------------------------------------
// Writes the data for a LevelSolution living on this level.
//---------------------------------------------------------
def BaseLevel.writeSolution(q:           LevelSolution,
                            AMR_level:   int,
                            outfile:     file
                           ){

  var grid_number = 0;
  for grid in child_grids {
    grid_number += 1;
    grid.writeSolution(q.child_sols(grid), grid_number, 1, outfile);
    outfile.writeln("  ");
  }
  
}
//<=== writeSolution method <===
//<=============================


//<=======================================================
//<=================== OUTPUT METHODS ====================
//<=======================================================
