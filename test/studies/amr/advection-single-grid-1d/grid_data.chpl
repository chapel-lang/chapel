module grid_data {

  //===> Description ===>
  //
  // Class definition for a 1-dimensional grid with ghost cells.
  //
  //<=== Description <===


  class GridFunction {
    var parent_grid: OneDimensionalGrid;
    var value: [parent_grid.all_cells] real;
    var time: real;
  }


  class OneDimensionalGrid {

    var x_low:           real;
    var x_high:          real;
    var num_cells:       int;
    var num_ghost_cells: int;

    var dx: real;

    var all_cells:                           domain(1)            = [1..0],
	      physical_cells:                      subdomain(all_cells) = [1..0],
        edges:                               subdomain(all_cells) = [1..0],
        left_ghost_cells:                    subdomain(all_cells) = [1..0],
        right_ghost_cells:                   subdomain(all_cells) = [1..0],
        periodic_image_of_left_ghost_cells:  subdomain(all_cells) = [1..0],
        periodic_image_of_right_ghost_cells: subdomain(all_cells) = [1..0];

    //===> Grid constructor ===>
    def OneDimensionalGrid(xlo, xhi, ncells, nghost) {

      //---- Core parameters ----
      x_low           = xlo;
      x_high          = xhi;
      num_cells       = ncells;
      num_ghost_cells = nghost;
      dx = (x_high - x_low) / num_cells:real;

      //---- Core domains ----
      physical_cells = [1 .. #num_cells];

      all_cells      = [physical_cells.low - num_ghost_cells
                        .. physical_cells.high + num_ghost_cells];

      edges          = [edge_right_of(all_cells.low) 
                        .. edge_left_of(all_cells.high)];

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


    //===> How to locate edges that neighbor a cell, and vice versa <===
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
    //<=== How to locate edges that neighbor a cell, and vice versa <===



    
    
    //===> Upwind update of a function on this grid ===>
    def upwind_constant_advection(q: GridFunction, time_requested: real, velocity: real) {

      assert(q.parent_grid == this && q.time <= time_requested);

      writeln("Hello from upwind_constant_advection!");

    }
      
/*      var dt_target: real = 0.9*grid.dx / abs(velocity);
      var dt_used:   real;
      var flux: [grid.edges] real;
      
      
      while (time < time_requested) do {
        
        //---- Adjust time step to hit output time if necessary ----
        if (time+dt_target > time_requested) then
          dt_used = time_requested - time;
        else
          dt_used = dt_target;

        //---- Define fluxes at each edge ----
        for edge in grid.edges do
          flux(edge) = velocity * q( grid.cell_left_of(edge) );

        //---- Update solution on each cell ----
        for cell in grid.physical_cells do {
          q(cell) = q(cell) - dt_used/grid.dx *
                    (  flux( grid.edge_right_of(cell) )  -  flux( grid.edge_left_of(cell) )  );
        }

        //---- Fill in ghost cells for use at next step ----
        q(grid.left_ghost_cells)  = q(grid.periodic_image_of_left_ghost_cells);
        q(grid.right_ghost_cells) = q(grid.periodic_image_of_right_ghost_cells);

        //---- Update time ----
        time += dt_used;
      } // end while
      
    }    
    //<=== Upwind update of a function on this grid <===
*/
      

  } // Grid definition
  
  

} // end module