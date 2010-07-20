//===> Description ===>
//
// Definitions for the AMRLevel class.
//
//<=== Description <===

use grid_base_defs;
use linked_list_defs;
use Random;



/* //===> Definition of class LevelGrid ===> */
/* //======================================> */
/* class LevelGrid: RectangularGrid { */
/*   var parent: AMRLevel; */
/* } */

/* def LevelGrid.LevelGrid(parent_input:          AMRLevel, */
/*                         low_index_input:       dimension*int, */
/*                         high_index_input:      dimension*int) { */

/*   //---- Check that index bounds are valid ---- */
/*   [d in dimensions] { */
/*     assert(low_index_input(d)%2 == 0,  */
/* 	   "Error: low_index of LevelGrid must specify a vertex."); */
/*     assert(high_index_input(d)%2 == 0, */
/* 	   "Error: high_index of LevelGrid must specify a vertex."); */
/*   } */

/*   //---- Assign input arguments ---- */
/*   parent          = parent_input; */
/*   low_index       = low_index_input; */
/*   high_index      = high_index_input; */

/*   //---- Calculation of remaining fields ---- */
/*   num_ghost_cells = parent.n_child_ghost_cells; */
/*   dx              = parent.dx; */
/*   forall d in dimensions { */
/*     num_cells(d)  = (high_index(d) - low_index(d)) / 2.0;  */
/*     low_coord(d)  = parent.low_coord(d) + dx(d)/2.0*low_index(d); */
/*     high_coord(d) = parent.low_coord(d) + dx(d)/2.0*high_index(d); */
/*   }  */
  
/* } */
/* //<====================================== */
/* //<=== Definition of class LevelGrid <=== */




//===> Definition of class AMRLevel ===>
//=====================================>
class AMRLevel {
  var low_coord, high_coord: dimension*real,
      num_cells:             dimension*int,
      n_child_ghost_cells:   dimension*int,
      dx:                    dimension*real;

  var grid_list:    LinkedList(RectangularGrid);
  var grid_numbers: domain(1);
  var grid_array:   [grid_numbers] RectangularGrid;

}

def AMRLevel.AMRLevel(low_coord_input:  dimension*real,
		      high_coord_input: dimension*real,
		      num_cells_input:  dimension*int,
		      n_ghost_input:    dimension*int) {

  low_coord           = low_coord_input;
  high_coord          = high_coord_input;
  num_cells           = num_cells_input;
  n_child_ghost_cells = n_ghost_input;

  dx = (high_coord - low_coord) / num_cells;
  
  grid_list = new LinkedList(RectangularGrid);
}
//<=====================================
//<=== Definition of class AMRLevel <===





//===> Adding a grid to an AMRLevel ===>
//=====================================>
def AMRLevel.add_grid(low_index:  dimension*int,
		      high_index: dimension*int) {

  var low_coord_grid:  dimension*real,
      high_coord_grid: dimension*real,
      n_cells_grid:    dimension*int;

  [d in dimensions] {
    low_coord_grid(d)  = low_coord(d) + low_index(d) *dx(d)/2;
    high_coord_grid(d) = low_coord(d) + high_index(d)*dx(d)/2;
    n_cells_grid(d)    = (high_index(d) - low_index(d)) / 2;
  }
  


  var grid = new RectangularGrid(low_coord_grid,
				 high_coord_grid,
				 low_index,
				 n_cells_grid,
				 n_child_ghost_cells);

  grid_list.add_node(grid);
}
//<=====================================
//<=== Adding a grid to an AMRLevel <===







def main {
  
  var low_coord, high_coord: dimension*real,
    num_cells, num_ghost: dimension*int;

  [d in dimensions] {
    low_coord(d)  = 0.0;
    high_coord(d) = 1.0;
    num_cells(d)  = 20 * 2**d;
    num_ghost(d)  = 2;
  }

  var level = new AMRLevel(low_coord, high_coord, num_cells, num_ghost);


  var random_real: [1..2] real;
  var random_int:  [1..2] int;
  var low_index, high_index: dimension*int;

  for d in dimensions {
    //---- Generate random ints for grid index bounds ----
    fillRandom(random_real);
    for i in [1..2] do 
      random_int(i) = 2*round(random_real(i) / level.dx(d)):int;

    //---- Space out the ints if they're equal ----
    if random_int(1) == random_int(2) {
      if random_int(1) == 0 then
	random_int(2) += 2;
      else
	random_int(1) -= 2;
    }

    //---- Assign low_index and high_index ----
    if random_int(1) < random_int(2) {
      low_index(d)  = random_int(1);
      high_index(d) = random_int(2);
    }
    else {
      low_index(d)  = random_int(2);
      high_index(d) = random_int(1);
    }

  }

  level.add_grid(low_index, high_index);


  var node = level.grid_list.head;

  while node != nil {
      writeln(node.data);
      node = node.next;
  }

}