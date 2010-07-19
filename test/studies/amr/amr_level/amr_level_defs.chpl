//===> Description ===>
//
// Definitions for the AMRLevel class.
//
//<=== Description <===

use grid_base_defs;

param max_grids = 200;


//===> Definition of class AMRLevel ===>
//=====================================>
class AMRLevel {
  var low_coord, high_coord: dimension*real,
      num_cells:             dimension*int,
      num_ghost_cells:       dimension*int,
      dx:                    dimension*real;
      
}
//<=====================================
//<=== Definition of class AMRLevel <===




//===> Definition of class LevelGrid ===>
//======================================>
class LevelGrid: RectangularGrid {
  var parent: AMRLevel;
}

def LevelGrid.LevelGrid(parent_input:          AMRLevel,
                        low_index_input:       dimension*int,
                        high_index_input:      dimension*int,
                        num_ghost_cells_input: dimension*int) {

  parent     = parent_input;
  low_index  = low_index_input;
  high_index = high_index_input;
  num_ghost_cells = num_ghost_cells_input;

  dx = parent.dx;
  forall d in dimensions {
    num_cells(d)  = (high_index(d) - low_index(d)) / 2.0; 
    low_coord(d)  = parent.low_coord(d) + dx(d)/2.0*low_index(d);
    high_coord(d) = parent.low_coord(d) + dx(d)/2.0*high_index(d);
  } 
  
}
//<=== Definition of class LevelGrid <===
//<======================================




// //===> Definition of class GridList ===>
// //=====================================>
// class Node {
//   type element_type;
//   var data: element_type;
//   var next: Node(element_type);
// }
// 
// class GridList {
//   var first: Node(Rec)
// }
// 
// 
// //<=====================================
// //<=== Definition of class GridList <===
