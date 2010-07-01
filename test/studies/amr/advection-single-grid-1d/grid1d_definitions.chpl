module grid1d_definitions {

  //===> Description ===>
  //
  // Class definition for a 1-dimensional grid with ghost cells.
  //
  //<=== Description <===


  class GridData {

    var x_low:           real;
    var x_high:          real;
    var num_cells:       int;
    var num_ghost_cells: int;

    var dx: real = 0.0;

    var all_cells:                           domain(1)            = [1..0],
	physical_cells:                      subdomain(all_cells) = [1..0],
        edges:                               subdomain(all_cells) = [1..0],
        left_ghost_cells:                    subdomain(all_cells) = [1..0],
        right_ghost_cells:                   subdomain(all_cells) = [1..0],
        periodic_image_of_left_ghost_cells:  subdomain(all_cells) = [1..0],
        periodic_image_of_right_ghost_cells: subdomain(all_cells) = [1..0];

    def edge_left_of(cell: int) {
      return cell;
    }

    def edge_right_of(cell: int){
      return cell+1;
    }

    def cell_left_of(edge: int){
      return edge-1;
    }

    def cell_right_of(edge: int){
      return edge;
    }



  //===> GridData constructor ===>
    def GridData(xlo, xhi, ncells, nghost) {

    //---- Core parameters ----
      x_low           = xlo;
      x_high          = xhi;
      num_cells       = ncells;
      num_ghost_cells = nghost;
      dx = (x_high - x_low) / num_cells:real;

    //---- Core domains ----
      physical_cells = [1 .. #num_cells];

      all_cells      = [physical_cells.low  - num_ghost_cells
			.. physical_cells.high + num_ghost_cells];

      edges          = [edge_right_of(all_cells.low) .. edge_left_of(all_cells.high)];


    //---- Domains for ghost cells and their periodic images ----
      left_ghost_cells  = [physical_cells.low - num_ghost_cells 
			  .. #num_ghost_cells];

      right_ghost_cells = [physical_cells.high + 1 
			   .. #num_ghost_cells];

      periodic_image_of_right_ghost_cells = [physical_cells.low 
					     .. #num_ghost_cells];

      periodic_image_of_left_ghost_cells  = [physical_cells.high - num_ghost_cells + 1 
					     .. #num_ghost_cells];

    }
  //<=== GridData constructor <===

  }

}