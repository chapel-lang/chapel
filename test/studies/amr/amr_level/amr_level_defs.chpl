//===> Description ===>
//
// Definitions for the AMRLevel class.
//
//<=== Description <===

use grid_base_defs;
use linked_list_defs;




//===> Definition of class LevelGrid ===>
//======================================>
class LevelGrid: RectangularGrid {
  var parent: AMRLevel;
}

def LevelGrid.LevelGrid(parent_input:          AMRLevel,
                        low_index_input:       dimension*int,
                        high_index_input:      dimension*int) {

  //---- Check that index bounds are valid ----
  [d in dimensions] {
    assert(low_index_input(d)%2 == 0, 
	   "Error: low_index of LevelGrid must specify a vertex.");
    assert(high_index_input(d)%2 == 0,
	   "Error: high_index of LevelGrid must specify a vertex.");
  }

  //---- Assign input arguments ----
  parent          = parent_input;
  low_index       = low_index_input;
  high_index      = high_index_input;

  //---- Calculation of remaining fields ----
  num_ghost_cells = parent.n_ghost_of_children;
  dx = parent.dx;
  forall d in dimensions {
    num_cells(d)  = (high_index(d) - low_index(d)) / 2.0; 
    low_coord(d)  = parent.low_coord(d) + dx(d)/2.0*low_index(d);
    high_coord(d) = parent.low_coord(d) + dx(d)/2.0*high_index(d);
  } 
  
}
//<======================================
//<=== Definition of class LevelGrid <===




//===> Definition of class AMRLevel ===>
//=====================================>
class AMRLevel {
  var low_coord, high_coord: dimension*real,
      num_cells:             dimension*int,
      n_ghost_of_children:   dimension*int,
      dx:                    dimension*real;

  var grid_list:    LinkedList(LevelGrid);
  var grid_numbers: domain(1);
  var grid_array:   [grid_numbers] LevelGrid;

}

def AMRLevel.AMRLevel(low_coord_input:  dimension*real,
		      high_coord_input: dimension*real,
		      num_cells_input:  dimension*int,
		      n_ghost_input:    dimension*int) {

  low_coord           = low_coord_input;
  high_coord          = high_coord_input;
  num_cells           = num_cells_input;
  n_ghost_of_children = n_ghost_input;

  dx = (high_coord - low_coord) / num_cells;
  
  grid_list = new LinkedList(LevelGrid);
}
//<=====================================
//<=== Definition of class AMRLevel <===





//===> Adding a grid to an AMRLevel ===>
//=====================================>
def AMRLevel.add_grid(low_index:  dimension*int,
		      high_index: dimension*int) {


  var grid = new LevelGrid(low_index, high_index);
  grid_list.add_node(grid);
}
//<=====================================
//<=== Adding a grid to an AMRLevel <===







def main {
  
  var low_coord, high_coord: dimension*real,
    num_cells, num_ghost: dimension*int;

  [d in dimensions] {
    low_coord(d)  = 0.0;
    high_coord(d) = d*1.0;
    num_cells(d)  = 20;
    num_ghost(d)  = 2;
  }

  var level = new AMRLevel(low_coord, high_coord, num_cells, num_ghost);

  writeln(level);

}