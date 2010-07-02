module grid_data {

  //===> Description ===>
  //
  // Class definition for a 1-dimensional grid with ghost cells.
  //
  //<=== Description <===




  //=========================================>
  //===> Definition of class GridFunction ===>
  //=========================================>
  class GridFunction {
    var parent_grid: OneDimensionalGrid;
    var value: [parent_grid.all_cells] real;
    var time: real;
  }
  //<=========================================
  //<=== Definition of class GridFunction ====
  //<=========================================




  //===============================================>
  //===> Definition of class OneDimensionalGrid ===>
  //===============================================>
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


    //=======================================>
    //===> OneDimensionalGrid constructor ===>
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
    //<=== OneDimensionalGrid constructor <===
    //<=======================================



    //========================================================>
    //===> Generate coordinates of (physical) cell centers ===>
    def cell_centers () {

      var x: [physical_cells] real;

      for cell in physical_cells do 
	x(cell) = x_low + (cell:real-0.5)*dx;
      
      return x;
    }
    //<=== Generate coordinates of (physical) cell centers <===
    //<========================================================


    //=================================================================>
    //===> How to locate edges that neighbor a cell, and vice versa ===>
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
    //<=================================================================



    
    //=====================================================>    
    //===> Upwind update of a GridFunction on this grid ===>
    def constant_advection_upwind(q:              GridFunction, 
				  time_requested: real, 
				  velocity:       real) {

      assert(q.parent_grid == this && q.time <= time_requested);

      var dt_target: real = 0.9*dx / abs(velocity);
      var dt_used:   real;

      var flux: [edges] real;
      
      
      while (q.time < time_requested) do {
        
        //---- Adjust time step to hit output time if necessary ----
        if (q.time+dt_target > time_requested) then
          dt_used = time_requested - q.time;
        else
          dt_used = dt_target;


        //---- Fill in ghost cells  ----
        q.value(left_ghost_cells)  = q.value(periodic_image_of_left_ghost_cells);
        q.value(right_ghost_cells) = q.value(periodic_image_of_right_ghost_cells);


        //---- Define fluxes at each edge ----
        for edge in edges do
          flux(edge) = velocity * q.value( cell_left_of(edge) );


        //---- Update solution on each cell ----
        for cell in physical_cells do {
          q.value(cell) -= dt_used/dx * ( flux(edge_right_of(cell)) - flux(edge_left_of(cell)) );
        }


        //---- Update time ----
        q.time += dt_used;

      } // end while
      
    }    
    //<=== Upwind update of a GridFunction on this grid <===
    //<=====================================================



    //=================================================>
    //===> Output a GridFunction in Clawpack format ===>
    def output_in_clawpack_format(q: GridFunction,
				  frame_number: int) {

      //---- Make sure that q lives on this grid ----
      assert(q.parent_grid == this);

      
      //---- Parameters needed by the output file ----
      var meqn:        int = 1,
       	  ngrids:      int = 1,
	  maux:        int = 0,
	  ndim:        int = 1,
	  grid_number: int = 1,
	  AMR_level:   int = 1;


      //---- Formatting parameters ----
      var efmt: string = "%16.8e",
          ifmt: string = "%16i",
          sfmt: string = "%16s";


      //---- Names of output files ----
      var frame_string:          string = format("%04i", frame_number),
	  name_of_time_file:     string = "_output/fort.t" + frame_string,
          name_of_solution_file: string = "_output/fort.q" + frame_string;
          

      //---- Write time file ----
      var outfile = new file(name_of_time_file, FileAccessMode.write);
      outfile.open();
      outfile.writeln( format(efmt, q.time), format(sfmt, "time"));
      outfile.writeln( format(ifmt, meqn),   format(sfmt, "meqn"));
      outfile.writeln( format(ifmt, ngrids), format(sfmt, "ngrids"));
      outfile.writeln( format(ifmt, maux),   format(sfmt, "maux"));
      outfile.writeln( format(ifmt, ndim),   format(sfmt, "ndim"));
      outfile.close();
      delete outfile;


      //---- Write solution file ----
      outfile = new file(name_of_solution_file, FileAccessMode.write);
      outfile.open();
      outfile.writeln( format(ifmt, grid_number), format(sfmt, "grid_number"));
      outfile.writeln( format(ifmt, AMR_level),   format(sfmt, "AMR_level"));
      outfile.writeln( format(ifmt, num_cells),   format(sfmt, "mx"));
      outfile.writeln( format(efmt, x_low),       format(sfmt, "xlow"));
      outfile.writeln( format(efmt, dx),          format(sfmt, "dx"));
      outfile.writeln("");
      for cell in physical_cells do
        outfile.writeln(format(efmt, q.value(cell)));
      outfile.close();
      delete outfile;


    }
    //<=== Output a GridFunction in Clawpack format <===
    //<=================================================     
 

  }
  //<===============================================
  //<=== Definition of class OneDimensionalGrid <===
  //<===============================================
  
  

} // end module