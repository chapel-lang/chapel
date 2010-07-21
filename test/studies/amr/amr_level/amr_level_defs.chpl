//===> Description ===>
//
// Definitions for the AMRLevel class.
//
//<=== Description <===

use grid_base_defs;
use linked_list_defs;
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

  var n_grids = 0;
  var grid_list:    LinkedList(LevelGrid);
  var grid_numbers: domain(1);
  var grid_array:   [grid_numbers] LevelGrid;


  //===> initialize() method ===>
  def initialize() {
    dx = (high_coord - low_coord) / n_cells;
    grid_list = new LinkedList(LevelGrid);
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

  grid_list.add_node(new_grid);
  n_grids += 1;
}
//<=====================================
//<=== Adding a grid to an AMRLevel <===




//===> Move grid_list into grid_array ===>
//=======================================>
def AMRLevel.move_list_to_array() {

  grid_numbers = [1..n_grids];
  for n in grid_numbers do
    grid_array(n) = grid_list.remove_from_head();

}
//<=======================================
//<=== Move grid_list into grid_array <===




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




/*   //===> Write out grid data ===> */
/*   var node = level.grid_list.head; */

/*   var grid_num = 1; */

/*   while node != nil { */
/*       writeln(""); */
/*       writeln("Grid number ", grid_num); */
/*       writeln("=============="); */
/*       writeln("low_coord     = ", node.data.low_coord); */
/*       writeln("high_coord    = ", node.data.high_coord); */
/*       writeln("n_cells       = ", node.data.n_cells); */
/*       writeln("n_ghost_cells = ", node.data.n_ghost_cells); */

/*       node = node.next; */
/*       grid_num += 1; */
/*   } */
/*   //<=== Write out grid data <=== */


  //===> Move grid_list to grid_array, and write ===>
  level.move_list_to_array();

  var grid: LevelGrid;

  for grid_num in level.grid_numbers {
    grid = level.grid_array(grid_num);
    writeln("");
    writeln("Grid number ", grid_num);
    writeln("==============");
    writeln("low_coord     = ", grid.low_coord);
    writeln("high_coord    = ", grid.high_coord);
    writeln("n_cells       = ", grid.n_cells);
    writeln("n_ghost_cells = ", grid.n_ghost_cells);
  }
  //<=== Move grid_list to grid_array, and write <===



}