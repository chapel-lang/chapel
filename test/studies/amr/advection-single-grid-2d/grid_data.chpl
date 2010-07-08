module grid_class {
  
  //===> Description ===>
  //
  // Class definition for a rectangular grid of arbitrary dimension.
  //
  //<=== Description <===


  //---- Spatial dimension must be known at compile time.  It seems
  // appropriate to set this external to the grid, as it's inherent
  // to *any* spatial object in the same context. -----------------
  //---------------------------------------------------------------
  param dimension  = 1;
  const dimensions = [1..dimension];


  //============================================>
  //===> Definition of class RectangularGrid ===>
  //============================================>
  class RectangularGrid {
    
    const dimensions: domain(1) = [1..dimension];
    
    var lower_corner, upper_corner: dimension*real,
        num_cells, num_ghost_cells: dimension*int;

    var dx: dimension*real;
            
    var all_cells:      domain(dimension),
        physical_cells: subdomain(all_cells);
    
    var interfaces: [dimensions] domain(dimension);
    
    var lower_ghost_cells: [dimensions] domain(dimension),
        upper_ghost_cells: [dimensions] domain(dimension);
    
    


    //===> RectangularGrid constructor ===>
    //====================================>
    def RectangularGrid(lower_corner_input:    dimension*real,
                        upper_corner_input:    dimension*real,
                        num_cells_input:       dimension*int,
                        num_ghost_cells_input: dimension*int)
    {
      //---- Used for modifying domains; needed for
      // dimensionally generic code. --------------
      var size: dimension*int;

      //---- Core parameters ----
      lower_corner    = lower_corner_input;
      upper_corner    = upper_corner_input;
      num_cells       = num_cells_input;
      num_ghost_cells = num_ghost_cells_input;
      dx              = (upper_corner - lower_corner) / num_cells;
      
      //---- Cell domains ----
      physical_cells = physical_cells.exterior(num_cells);
      all_cells      = physical_cells.expand(num_ghost_cells);
             

      //---- Orientation-dependent domains ----    
      for orientation in dimensions do {
        //---- Cell-to-cell interfaces ----
        [d in dimensions] size(d) = num_cells(d) + 2*num_ghost_cells(d);
        size(orientation) -= 1;
        interfaces(orientation) = all_cells.interior(size);
        
        //---- Ghost cells ----
	// Note that all ghost cell domains contain the corners.
        [d in dimensions] size(d) = 0;
        size(orientation) = -num_ghost_cells(orientation);
        lower_ghost_cells(orientation) = all_cells.interior(size);
        size(orientation) = num_ghost_cells(orientation);
        upper_ghost_cells(orientation) = all_cells.interior(size);

      }                            
    }
    //<====================================
    //<=== RectangularGrid constructor <===




    //===> Generate coordinates of cell centers ===>
    //=============================================>
    def cell_center_coordinates (cells: domain(dimension)) {

      var coordinates: [cells] dimension*real;

      if dimension == 1 then {
        forall (cell,d) in [cells,dimensions] do
          coordinates(cell)(d) = lower_corner(d) + (cell:real - 0.5)*dx(d);
      }
      else {
        forall (cell,d) in [cells,dimensions] do
          coordinates(cell)(d) = lower_corner(d) + (cell(d):real - 0.5)*dx(d);
      }

      return coordinates;
    }
    //<=============================================
    //<=== Generate coordinates of cell centers <===



    //===> Locating interfaces that neighbor a cell, and vice versa ===>
    //=================================================================>
    def lower_interface(cell: dimension*int, d: int) {
      return cell;
    }

    def upper_interface(cell: dimension*int, d: int) {
      var interface = cell;
      interface(d) += 1;
      return interface;
    }

    def lower_cell(interface: dimension*int, d: int) {
      var cell: dimension*int = interface;
      cell(d) -= 1;
      return cell;
    }

    def upper_cell(interface: dimension*int, d: int) {
      return interface;
    }
    //<=================================================================
    //<=== Locating interfaces that neighbor a cell, and vice versa <===




    //===> Output a GridFunction in Clawpack format ===>
    //=================================================>
    def clawpack_output(q: GridFunction, frame_number: int) {


      //---- Make sure that q lives on this grid ----
      assert(q.parent_grid == this);


      //---- Parameters needed by the output file ----
      var meqn:        int = 1,
       	  ngrids:      int = 1,
          maux:        int = 0,
          grid_number: int = 1,
          AMR_level:   int = 1;


      //---- Formatting parameters ----
      var efmt:  string = "%16.8e",
          ifmt:  string = "%16i",
          sfmt:  string = "%20s",
          linelabel: string;


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
      outfile.writeln( format(ifmt, dimension),   format(sfmt, "ndim"));
      outfile.close();
      delete outfile;


      //---> Write solution file --->
      //---------------------------->
      outfile = new file(name_of_solution_file, FileAccessMode.write);
      outfile.open();
      outfile.writeln( format(ifmt, grid_number), format(sfmt, "grid_number"));
      outfile.writeln( format(ifmt, AMR_level),   format(sfmt, "AMR_level"));


      //---- Write num_cells ----
      for d in dimensions do {
        linelabel = "num_cells(" + format("%1i",d) + ")";
        outfile.writeln( format(ifmt, num_cells(d)),  format(sfmt, linelabel));
      }


      //---- Write lower_corner ----
      for d in dimensions do {
        linelabel = "lower_corner(" + format("%1i",d) + ")";
        outfile.writeln( format(efmt, lower_corner(d)),  format(sfmt, linelabel));
      }


      //---- Write dx ----
      for d in dimensions do {
        linelabel = "dx(" + format("%1i",d) + ")";
        outfile.writeln( format(efmt, dx(d)),  format(sfmt, linelabel));
      }
      outfile.writeln("");


      //---- Write solution values ----
      var physical_cells_transposed: domain(dimension),
          shape: dimension*int;
      
      [d in dimensions] shape(d) = num_cells(1 + dimension - d);
      physical_cells_transposed = physical_cells_transposed.exterior(shape);

      if dimension == 1 then {
        for cell in physical_cells do
    	  outfile.writeln(format(efmt, q.value(cell)));
      }
      else {
        var cell: dimension*int;
        for cell_transposed in physical_cells_transposed do {
          [d in dimensions] cell(d) = cell_transposed(1 + dimension - d);
          outfile.writeln(format(efmt, q.value(cell)));
      	}
      }


      //---- Finish up ----
      outfile.close();
      delete outfile;
      //<----------------------------
      //<--- Write solution file <---

    }
    //<=================================================
    //<=== Output a GridFunction in Clawpack format <===




    //===> Evaluating an analytical function on the grid ===>
    //======================================================>
    def evaluate(f) {
      var q = new GridFunction(this);
      var coordinates = cell_center_coordinates(physical_cells);
      [cell in physical_cells] q.value(cell) = f(coordinates(cell));
      return q;
    }
    //<======================================================
    //<=== Evaluating an analytical function on the grid <===




    //===> Upwind update of a GridFunction on this grid ===>
    //=====================================================>
    def constant_advection_upwind(q:              GridFunction,
				  time_requested: real,
				  velocity:       dimension*real) {

      //---- Make sure q can validly be updated ----
      assert(q.parent_grid == this  &&  q.time <= time_requested);


      //---- Initialize ----
      var cfl: [dimensions] real = dx / velocity,
          dt_target:        real = 0.45 * max reduce(cfl, dimensions),
          dt_used:          real,
	  flux_lower:       real,
	  flux_upper:       real;

      var val_old = q.value;


      //===> Time-stepping loop ===>
      while (q.time < time_requested) do {

        //---- Adjust the time step to hit time_requested if necessary ----
        if (q.time + dt_target > time_requested) then
	  dt_used = time_requested - q.time;
	else
	  dt_used = dt_target;

	
	//---- Fill in ghost cells ----
	for d in dimensions do {
	  q.value(lower_ghost_cells(d)) = q.value(periodic_image_of_lower_ghost_cells(d));
	  q.value(upper_ghost_cells(d)) = q.value(periodic_image_of_upper_ghost_cells(d));
	}


	//---- Update solution on each cell ----
	for d in dimensions {
	  for cell in physical_cells {
	    if velocity(d) < 0.0 then {
	      flux_lower = velocity(d) * val_old(cell);
	      flux_upper = velocity(d) * val_old(upper_cell(cell,d));
	    }
	    else {
	      flux_lower = velocity(d) * val_old(lower_cell(cell,d));
	      flux_upper = velocity(d) * val_old(cell);
	    }

	    q.value(cell) -= dt_used/dx(d) * (flux_upper - flux_lower);
	  }
	}


	//---- Update time ----
	q.time += dt_used;

      }
      //<=== Time-stepping loop <===


    }
    //<=== Upwind update of a GridFunction on this grid <===
    //<=====================================================


  }
  //<============================================
  //<=== Definition of class RectangularGrid <===
  //<============================================
  
  
  
  
  //=========================================>
  //===> Definition of class GridFunction ===>
  //=========================================>
  class GridFunction {
    var parent_grid: RectangularGrid;
    var value: [parent_grid.all_cells] real;
    var time: real;
  }
  //<=========================================
  //<=== Definition of class GridFunction ====
  //<=========================================



} // end module grid_class
