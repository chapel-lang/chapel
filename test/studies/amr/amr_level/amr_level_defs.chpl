//===> Description ===>
//
// Definitions for the AMRLevel class.
//
//<=== Description <===

use grid_base_defs;
use Random;



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


    sanity_checks();

    set_derived_fields();

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
def AMRLevel.add_grid(i_low:  dimension*int,
		      i_high: dimension*int) {
  

  var new_grid = new LevelGrid(level     = this,
		               i_low  = i_low,
		               i_high = i_high);

  grids.add(new_grid);
}
//<=====================================
//<=== Adding a grid to an AMRLevel <===




//===> LevelFunction class ===>
//============================>
class LevelFunction {
  var level:          AMRLevel;                    // parent level
  var grid_functions: [level.grids] GridFunction;  // child functions
  var time:           real;                        // duh
}
//<============================
//<=== LevelFunction class <===




//===> LevelGridBoundaryManager class ===>
//=======================================>
class LevelGridBoundaryManager: BoundaryManager {

  var grid: LevelGrid;  // is it worth shadowing the RectangularGrid version?
  
  var neighbors:                subdomain(grid.level.grids);
  var shared_cells: [neighbors] subdomain(grid.all_cells);



  //===> locate_neighbors method ===>
  //================================>
  def locate_neighbors() {

    //==== Stores overlap in each dimension as a range ====
    var overlap:   dimension*range(stridable=true);
    var intersect: bool;

    //===> Loop over siblings ===>
    for g in grid.level.grids {

      //==== Assume g intersects grid ====
      intersect = true;


      //==== Intersect grid.ext_cells with g.cells ====
      [d in dimensions] {
	overlap(d) = max(grid.ext_cells.dim(d).low, g.cells.dim(d).low)
                     .. min(grid.ext_cells.dim(d).high, g.cells.dim(d).high)
	             by grid.ext_cells.dim(d).stride;
	if overlap(d).length == 0 then intersect = false;
      }

      
      //==== If g==grid, then the intersection is false ====
      if g == grid then intersect = false;


      //==== If g intersects grid, store the overlap ====
      if intersect then {
	neighbors.add(g);
	shared_cells(g) = [(...overlap)];
      }
    }
    //<=== Loop over siblings <===
    
  }
  //<=== locate_neighbors method <===
  //<================================



  //===> copy_from_neighbors method ===>
  //===================================>
  def copy_from_neighbors(q: GridFunction) {

    //==== Make sure action is valid ====
    assert(q.grid == grid, 
	   "error: copy_from_neighbors\n"
	   + "GridFunction must share parent grid with BoundaryManager.");

    for n in neighbors {
      //==== Locate sibling function on neighbor ====
      var q_nbr = q.level_function.grid_functions(n);

      //==== Copy values from shared cells ====
      q.value(shared_cells(n)) = q_nbr.value(shared_cells(n));
    }

  }
  //<=== copy_from_neighbors method <===
  //<===================================


}
//<=======================================
//<=== LevelGridBoundaryManager class <===






