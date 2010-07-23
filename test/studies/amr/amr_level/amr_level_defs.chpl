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
  var parent: AMRLevel;


  //===> initialize() method ===>
  def initialize() {

    //===> Set fields via parent data and index bounds ===>
    forall d in dimensions {
      low_coord(d)  = parent.low_coord(d) + low_index(d) * parent.dx(d)/2.0;
      high_coord(d) = parent.low_coord(d) + high_index(d) * parent.dx(d)/2.0;
      n_cells(d)    = (high_index(d) - low_index(d)) / 2;
    }

    n_ghost_cells = parent.n_child_ghost_cells;
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
  var low_coord, high_coord: dimension*real,
      n_cells:               dimension*int,
      n_child_ghost_cells:   dimension*int,
      dx:                    dimension*real;

  var grids: domain(LevelGrid);

  //===> initialize() method ===>
  def initialize() {
    dx = (high_coord - low_coord) / n_cells;
  }
  //<=== initialize() method <===

}
//<=====================================
//<=== Definition of class AMRLevel <===





//===> Adding a grid to an AMRLevel ===>
//=====================================>
def AMRLevel.add_grid(low_index:  dimension*int,
		      high_index: dimension*int) {
  

  var new_grid = new LevelGrid(parent     = this,
		               low_index  = low_index,
		               high_index = high_index);

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
    assert(q.grid = grid, 
	   "error: copy_from_neighbors\n"
	   + "GridFunction must share parent grid with BoundaryManager.");

    for n in neighbors {
      //==== Locate sibling function on neighbor ====
      q_nbr = q.level_function.grid_functions(n);

      //==== Copy values from shared cells ====
      q.value(shared_cells(n)) = q_nbr.value(shared_cells(n));
    }

  }
  //<=== copy_from_neighbors method <===
  //<===================================


}
//<=======================================
//<=== LevelGridBoundaryManager class <===


def main {
  
  var low_coord, high_coord: dimension*real,
    n_cells, n_ghost: dimension*int;

  [d in dimensions] {
    low_coord(d)  = 0.0;
    high_coord(d) = 1.0;
    n_cells(d)  = 20 * 2**d;
    n_ghost(d)  = 2;
  }

  var level = new AMRLevel(low_coord, high_coord, n_cells, n_ghost);


  var random_real: [1..2] real;
  var random_int:  [1..2] int;
  var low_index, high_index: dimension*int;



  //===> Create random grids on level ===>
  var n_grids = 10;

  for n in [1..n_grids] {

    //===> Generate random grid indices ===>
    for d in dimensions {
      //==== Generate random ints for grid index bounds ===
      fillRandom(random_real);
      for i in [1..2] do 
	random_int(i) = 2*round(random_real(i) / level.dx(d)):int;

      //==== Space out the ints if they're equal ====
      if random_int(1) == random_int(2) {
	if random_int(1) == 0 then
	  random_int(2) += 2;
	else
	  random_int(1) -= 2;
      }

      //==== Assign low_index and high_index ====
      if random_int(1) < random_int(2) {
	low_index(d)  = random_int(1);
	high_index(d) = random_int(2);
      }
      else {
	low_index(d)  = random_int(2);
	high_index(d) = random_int(1);
      }
      //<=== Generate random grid indices <===

    }

    //==== Add grid to level's list ====
    level.add_grid(low_index, high_index);

  }
  //<=== Create random grids on level



  var grid_num = 0;
  for grid in level.grids {
    grid_num += 1;
    writeln("");
    writeln("Grid number ", grid_num);
    writeln("==============");
    writeln("low_coord     = ", grid.low_coord);
    writeln("high_coord    = ", grid.high_coord);
    writeln("n_cells       = ", grid.n_cells);
    writeln("n_ghost_cells = ", grid.n_ghost_cells);
  }


}