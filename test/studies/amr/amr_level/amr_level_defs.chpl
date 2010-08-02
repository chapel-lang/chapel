//===> Description ===>
//
// Definitions for the RefinementLevel class.
//
//<=== Description <===

use grid_base_defs;



//=======================================================================>
//==================== FUNDAMENTAL LEVEL DEFINITIONS  ===================>
//=======================================================================>


//===> LevelGrid derived class ===>
//================================>
class LevelGrid: RectangularGrid {

  var level: RefinementLevel;

  var neighbors:    domain(LevelGrid); //subdomain(level.grids);
  var shared_cells: [neighbors] subdomain(ext_cells);



  //===> initialize method ===>
  //==========================>
  def initialize() {

    //===> Set fields via parent data and index bounds ===>
    forall d in dimensions {
      x_low(d)  = level.x_low(d) + i_low(d)  * level.dx(d)/2.0;
      x_high(d) = level.x_low(d) + i_high(d) * level.dx(d)/2.0;
      n_cells(d)= (i_high(d) - i_low(d)) / 2;
    }

    n_ghost_cells = level.n_child_ghost_cells;
    //<=== Set fields via parent data and index bounds <===


    sanityChecks();

    setDerivedFields();

  }
  //<==========================
  //<=== initialize method <===



  //===> locateNeighbors method ===>
  //================================>
  //-------------------------------------------------------------------
  // Locates neighboring grids, and stores the domains that his grid's 
  // ghost cells share with its neighbor's interior.
  //-------------------------------------------------------------------
  def locateNeighbors() {
  
    //==== Stores overlap in each dimension as a range ====
    var overlap:   dimension*range(stridable=true);
    var intersect: bool;
  
    //===> Loop over siblings ===>
    for sib in level.grids {
  
      //==== Assume sib intersects this grid ====
      intersect = true;
  
  
      //==== Intersect ext_cells with sib.cells ====
      forall d in dimensions {
        overlap(d) = max(ext_cells.dim(d).low,  sib.cells.dim(d).low)  
                     ..
                     min(ext_cells.dim(d).high, sib.cells.dim(d).high)
                     by ext_cells.dim(d).stride;
        if overlap(d).length == 0 then intersect = false;
      }
  
      
      //==== If sib==grid, then the intersection is false ====
      if sib == this then intersect = false;
  
  
      //==== If sib intersects grid, store the overlap ====
      if intersect then {
        neighbors.add(sib);
        shared_cells(sib) = [(...overlap)];
      }
    }
    //<=== Loop over siblings <===
    
  }
  //<=== locateNeighbors method <===
  //<================================
  
  
  
  //===> copyFromNeighbors method ===>
  //=================================>
  //-------------------------------------------------------------
  // Copies data from q's siblings on neighboring grids into q's
  // ghost values.
  //-------------------------------------------------------------
  def copyFromNeighbors(q: GridSolution) {
  
    //==== Make q lives on this grid ====
    assert(q.grid == this);
  
    for nbr in neighbors {
      //==== Locate sibling function on neighbor ====
      var q_sib = q.level_function.grid_functions(nbr);
  
      //==== Copy values from shared cells ====
      q.value(shared_cells(nbr)) = q_sib.value(shared_cells(nbr));
    }
  
  }
  //<=== copyFromNeighbors method <===
  //<=================================


}
//<================================
//<=== LevelGrid derived class <===




//===> Definition of class RefinementLevel ===>
//============================================>
class RefinementLevel {
  
  var x_low, x_high:       dimension*real,
      n_cells:             dimension*int,
      n_child_ghost_cells: dimension*int,
      dx:                  dimension*real;

  var grids: domain(LevelGrid);  // associative domain of LevelGrids

  //===> initialize() method ===>
  def initialize() {
    dx = (x_high - x_low) / n_cells;
  }
  //<=== initialize() method <===

}
//<============================================
//<=== Definition of class RefinementLevel <===



//===> RefinementLevel.addGrid method ===>
//=======================================>
//--------------------------------------------------------
// This version is based on indices, and probably best to
// use in practice, as integer arithmetic is cleaner than
// real arithmetic.
//--------------------------------------------------------
def RefinementLevel.addGrid(i_low:  dimension*int,
                            i_high: dimension*int
                           ) {
  
  var new_grid = new LevelGrid(level  = this,
                               i_low  = i_low,
                               i_high = i_high);

  grids.add(new_grid);
}


//----------------------------------------------------------
// This version takes in real bounds, and snaps them to the
// level's discretization.  Mostly for testing purposes.
//----------------------------------------------------------
def RefinementLevel.addGrid(x_low_grid:  dimension*real,
                  x_high_grid: dimension*real
                 ){


  var i_low, i_high: dimension*int;

  //==== Find nearest interfaces to provided coordinates ====
  for d in dimensions {
    i_low(d)  = 2 * round((x_low_grid(d)  - x_low(d)) / dx(d)) : int;
    i_high(d) = 2 * round((x_high_grid(d) - x_low(d)) / dx(d)) : int;
  }
  
  writeln(i_low);
  writeln(i_high);

  //==== Add grid ====
  var new_grid = new LevelGrid(level  = this,
                               i_low  = i_low,
                               i_high = i_high);
  grids.add(new_grid);
}
//<=======================================
//<=== RefinementLevel.addGrid method <===


//<======================================================================
//<=================== FUNDAMENTAL LEVEL DEFINITIONS ====================
//<======================================================================






//========================================================>
//==================== LEVEL SOLUTIONS ===================>
//========================================================>


//===> LevelSolution class ===>
//============================>
class LevelSolution {
  var level:          RefinementLevel;             // parent level
  var grid_solutions: [level.grids] GridSolution;  // child functions
  var time:           real;                        // duh
}
//<============================
//<=== LevelSolution class <===



//===> initializSolution method ===>
//=================================>

//--------------------------------------------------------------------
// This version takes in initial_condition as a first-class function.
//--------------------------------------------------------------------
def RefinementLevel.initializeSolution(initial_condition: func(dimension*real,real)) {

  var grid_solutions: [grids] GridSolution;
  
  for grid in grids {
    grid_solutions(grid) = grid.initializeSolution(initial_condition);
  }
  
  return new LevelSolution(level          = this,
                           grid_solutions = grid_solutions,
                           time           = 0.0);
}


//--------------------------------------------------
// This version uses a TrueSolution data structure.
//--------------------------------------------------
def RefinementLevel.initializeSolution(true_solution: TrueSolution){

  var grid_solutions: [grids] GridSolution;

  for grid in grids {
    grid_solutions(grid) = grid.initializeSolution(true_solution);
  }

  return new LevelSolution(level          = this,
			   grid_solutions = grid_solutions,
			   time           = 0.0);
}

//<=== initializeSoution method <===
//<=================================


//<========================================================
//<=================== LEVEL SOLUTIONS ====================
//<========================================================






//============================================================>
//==================== BOUNDARY CONDITIONS ===================>
//============================================================>


//===> LevelBC class ===>
//======================>
class LevelBC {
  
  var level:    RefinementLevel;
  var grid_bcs: [level.grids] GridBC;

  //===> ghostFill method ===>
  //=========================>
  def ghostFill(q: LevelSolution) {
    
    //==== Make sure q can validly be filled ====
    assert(q.level == level);
    
    coforall grid in level.grids do
      grid_bcs(grid).ghostFill(q.grid_solutions(grid));
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
    
    for grid in level.grids {
      grid_bcs(grid) = new ZeroOrderExtrapGridBC(grid = grid);
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
def RefinementLevel.clawOutput(q:            LevelSolution,
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
  for grid in grids do n_grids += 1;

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
def RefinementLevel.writeSolution(q:           LevelSolution,
                                  AMR_level:   int,
                                  outfile:     file
                                 ){

  var grid_number = 0;
  for grid in grids {
    grid_number += 1;
    grid.writeSolution(q.grid_solutions(grid), grid_number, 1, outfile);
    outfile.writeln("  ");
  }
  
}
//<=== writeSolution method <===
//<=============================


//<=======================================================
//<=================== OUTPUT METHODS ====================
//<=======================================================






// //==========================================================>
// //==================== BOUNDARY MANAGERS ===================>
// //==========================================================>
// 
// 
// //===> LevelGridBoundaryManager class ===>
// //=======================================>
// class LevelGridBoundaryManager: BoundaryManager {
// 
//   var grid: LevelGrid;  // is it worth shadowing the RectangularGrid version?
//   
//   var neighbors:                subdomain(grid.level.grids);
//   var shared_cells: [neighbors] subdomain(grid.all_cells);
// 
// 
// 
//   //===> locate_neighbors method ===>
//   //================================>
//   def locate_neighbors() {
// 
//     //==== Stores overlap in each dimension as a range ====
//     var overlap:   dimension*range(stridable=true);
//     var intersect: bool;
// 
//     //===> Loop over siblings ===>
//     for g in grid.level.grids {
// 
//       //==== Assume g intersects grid ====
//       intersect = true;
// 
// 
//       //==== Intersect grid.ext_cells with g.cells ====
//       forall d in dimensions {
//         overlap(d) = max(grid.ext_cells.dim(d).low, g.cells.dim(d).low)
//                      .. min(grid.ext_cells.dim(d).high, g.cells.dim(d).high)
//                     by grid.ext_cells.dim(d).stride;
//         if overlap(d).length == 0 then intersect = false;
//       }
// 
//       
//       //==== If g==grid, then the intersection is false ====
//       if g == grid then intersect = false;
// 
// 
//       //==== If g intersects grid, store the overlap ====
//       if intersect then {
//         neighbors.add(g);
//         shared_cells(g) = [(...overlap)];
//       }
//     }
//     //<=== Loop over siblings <===
//     
//   }
//   //<=== locate_neighbors method <===
//   //<================================
// 
// 
// 
//   //===> copyFromNeighbors method ===>
//   //===================================>
//   def copyFromNeighbors(q: GridSolution) {
// 
//     //==== Make sure action is valid ====
//     assert(q.grid == grid, 
//     "error: copyFromNeighbors\n"
//     + "GridSolution must share parent grid with BoundaryManager.");
// 
// 
// 
//     for n in neighbors {
//       //==== Locate sibling function on neighbor ====
//       var q_nbr = q.level_function.grid_functions(n);
// 
//       //==== Copy values from shared cells ====
//       q.value(shared_cells(n)) = q_nbr.value(shared_cells(n));
//     }
// 
//   }
//   //<=== copyFromNeighbors method <===
//   //<===================================
// 
// 
// }
// //<=======================================
// //<=== LevelGridBoundaryManager class <===
// 
// 
// //<==========================================================
// //<=================== BOUNDARY MANAGERS ====================
// //<==========================================================
