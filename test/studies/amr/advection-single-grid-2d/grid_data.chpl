module grid_data {
  
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
    
    var lower_corner: [dimensions] real,
        upper_corner: [dimensions] real,
        num_cells:    [dimensions] int,
        num_ghost_cells: int;

    var dx: [dimensions] real;
            
    var all_cells:      domain(dimension),
        physical_cells: subdomain(all_cells);
    
    var interfaces: [dimensions] domain(dimension);
    
    var lower_ghost_cells: [dimensions] domain(dimension),
        upper_ghost_cells: [dimensions] domain(dimension);
    
    


    //===> RectangularGrid constructor ===>
    //====================================>
    def RectangularGrid(lower_corner_input: dimension*real,
                           upper_corner_input: dimension*real,
                           num_cells_input:    dimension*int,
                           num_ghost_cells_input:        int)
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
      [d in dimensions] size(d) = num_cells(d);
      physical_cells = physical_cells.exterior(size);
      all_cells      = physical_cells.expand(num_ghost_cells);
             

      //---- Orientation-dependent domains ----    
      for orientation in dimensions do {
        //---- Cell-to-cell interfaces ----
        [d in dimensions] size(d) = num_cells(d) + 2*num_ghost_cells;
        size(orientation) -= 1;
        interfaces(orientation) = all_cells.interior(size);
        
        //---- Ghost cells ----
	// Note that all ghost cell domains contain the corners.
        [d in dimensions] size(d) = 0;
        size(orientation) = -num_ghost_cells;
        lower_ghost_cells(orientation) = all_cells.interior(size);
        size(orientation) = num_ghost_cells;
        upper_ghost_cells(orientation) = all_cells.interior(size);

      }                            
    }
    //<====================================
    //<=== RectangularGrid constructor <===




    //===> Generate coordinates of cell centers ===>
    //=============================================>
    def cell_center_coordinates (cells) 
    where cells.rank == 1 {

      var coordinates: [cells] real;
      [cell in cells] coordinates(cell) = lower_corner(1) + (cell:real - 0.5)*dx(1);
      return coordinates;
    }

    def cell_center_coordinates (cells)
    where cells.rank == 2 {

      var coordinates: [cells] dimension*real;
      forall (cell,d) in [cells,dimensions] do
        coordinates(cell)(d) = lower_corner(d) + (cell(d):real - 0.5)*dx(d);
      return coordinates;
    }
    //<=============================================
    //<=== Generate coordinates of cell centers <===



    //===> Locating interfaces that neighbor a cell, and vice versa ===>
    //=================================================================>
    def lower_interface(cell: dimension*int, d: int) {
      var interface: dimension*int = cell;
      return interface;
    }

    def upper_interface(cell: dimension*int, d: int) {
      var interface: dimension*int = cell;
      interface(d) += 1;
      return interface;
    }

    def lower_cell(interface: dimension*int, d: int) {
      var cell: dimension*int = interface;
      cell(d) -= 1;
      return cell;
    }

    def upper_cell(interface: dimension*int, d: int) {
      var cell: dimension*int = interface;
      return cell;
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
          sfmt:  string = "%16s",
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
  
  
  
  def main {

    var lower_corner, upper_corner: dimension*real,
      num_cells: dimension*int;

    for d in [1..dimension] {
      lower_corner(d) = 0.0;
      upper_corner(d) = 1.0;
      num_cells(d)    = 5; //10 * 2**(d-1);
    }

    var G = new RectangularGrid(lower_corner, upper_corner, num_cells, 2);


    writeln("");
    writeln("Core parameters:");
    writeln("----------------");
    writeln("lower_corner = ", G.lower_corner);
    writeln("upper_corner = ", G.upper_corner);
    writeln("num_cells    = ", G.num_cells);
    writeln("dx           = ", G.dx);


    writeln("");
    writeln("Cell domains:");
    writeln("-------------");
    writeln("physical_cells = ", G.physical_cells);
    writeln("all_cells      = ", G.all_cells);


    writeln("");
    writeln("Interface domains:");
    writeln("------------------");
    for d in dimensions do
      writeln("interfaces(", d, ") = ", G.interfaces(d));


    writeln("");
    writeln("Ghost cell domains:");
    writeln("-------------------");
    for d in dimensions do {
      writeln("lower_ghost_cells(", d, ") = ", G.lower_ghost_cells(d));
      writeln("upper_ghost_cells(", d, ") = ", G.upper_ghost_cells(d));
    }


    writeln("");
    writeln("Cell indices in column-major order:");
    writeln("-----------------------------------");
    var physical_cells_transposed: domain(dimension),
        shape: dimension*int;
      
    [d in dimensions] shape(d) = G.num_cells(1 + dimension - d);
    physical_cells_transposed = physical_cells_transposed.exterior(shape);

    if dimension == 1 then {
      for cell in G.physical_cells do
	writeln(cell);
    }
    else {
      var cell: dimension*int;
      for cell_transposed in physical_cells_transposed do {
	[d in dimensions] cell(d) = cell_transposed(1 + dimension - d);
	writeln(cell);
      }
    }


    writeln("");
    writeln("Testing function evaluation:");
    writeln("----------------------------");
    def fcn(x) where x.type==real {
      return 2.0*x;
    }


    var coordinates = G.cell_center_coordinates(G.physical_cells);
    var Q: [G.physical_cells] real;
    [cell in G.physical_cells] Q(cell) = fcn(coordinates(cell));
    writeln(Q);
    writeln("");

/*     var q = G.evaluate(initial_condition); */
/*     writeln(q.value); */

  } // end main
  

} // end module grid_data
