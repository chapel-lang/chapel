//===> Description ===>
//
// Definitions for the AMRLevel class.
//
//<=== Description <===

use grid_base_defs;


//===> Definition of class LevelGrid ===>
//======================================>
class LevelGrid: RectangularGrid {
  var level: AMRLevel;


  //===> initialize() method ===>
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
  //<=== initialize() method <===

}
//<======================================
//<=== Definition of class LevelGrid <===




//===> Definition of class AMRLevel ===>
//=====================================>
class AMRLevel {
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
//<=====================================
//<=== Definition of class AMRLevel <===





//===> Adding a grid to an AMRLevel ===>
//=====================================>
def AMRLevel.addGrid(i_low:  dimension*int,
                      i_high: dimension*int) {
  
  var new_grid = new LevelGrid(level  = this,
                               i_low  = i_low,
                               i_high = i_high);

  grids.add(new_grid);
}


def AMRLevel.addGrid(x_low_grid:  dimension*real,
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
//<=====================================
//<=== Adding a grid to an AMRLevel <===






//========================================================>
//==================== LEVEL FUNCTIONS ===================>
//========================================================>


//===> LevelSolution class ===>
//============================>
class LevelSolution {
  var level:          AMRLevel;                    // parent level
  var grid_functions: [level.grids] GridSolution;  // child functions
  var time:           real;                        // duh
}
//<============================
//<=== LevelSolution class <===



//===> initializeLevelSolution method ===>
//=======================================>
def AMRLevel.initializeLevelSolution(initial_condition) {
  var grid_functions: [grids] GridSolution;
  
  for grid in grids {
    grid_functions(grid) = grid.initializeGridSolution(initial_condition);
  }
  
  return new LevelSolution(level          = this,
                           grid_functions = grid_functions,
                           time           = 0.0);
}
//<=== initializeLevelSoution method <===
//<======================================

//<========================================================
//<=================== LEVEL FUNCTIONS ====================
//<========================================================






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
//   //===> copy_from_neighbors method ===>
//   //===================================>
//   def copy_from_neighbors(q: GridSolution) {
// 
//     //==== Make sure action is valid ====
//     assert(q.grid == grid, 
//     "error: copy_from_neighbors\n"
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
//   //<=== copy_from_neighbors method <===
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






//=======================================================>
//==================== OUTPUT METHODS ===================>
//=======================================================>


//===> claw_output method ===>
//===========================>
//-----------------------------------------------------------------------
// Writes both a time file and a solution file for a given frame number.
//-----------------------------------------------------------------------
def AMRLevel.clawOutput(q:            LevelSolution,
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
//<=== claw_output method <===
//<===========================




//===> writeSolution method ===>
//=============================>
//---------------------------------------------------------
// Writes the data for a LevelSolution living on this level.
//---------------------------------------------------------
def AMRLevel.writeSolution(q:           LevelSolution,
                           AMR_level:   int,
                           outfile:     file
                          ){

  var grid_number = 0;
  for grid in grids {
    grid_number += 1;
    grid.writeSolution(q.grid_functions(grid), grid_number, 1, outfile);
    outfile.writeln("  ");
  }
  
}
//<=== writeSolution method <===
//<=============================


//<=======================================================
//<=================== OUTPUT METHODS ====================
//<=======================================================