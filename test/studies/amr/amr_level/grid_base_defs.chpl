//===> Description ===>
//
// Basic class and method definitions for a rectangular grid of
// generic dimension (though it must be a compile-time constant.)
//
// This version is based on an underlying mesh of width dx/2, which
// contains all cell centers, interface centers, and vertices.  As
// a result, the domin of cell centers is strided by 2, corresponding
// to jumps of dx.
//
// Currently, only dimensions 1, 2, and 3 are supported, as a few
// parts of the code must be instantiated (look for "select" statements)
// until more general constructs are available.
//
//<=== Description <===


config param dimension = 2;
const dimensions = [1..dimension];


//===> Definition of class RectangularGrid ===>
//============================================>
class RectangularGrid {
  
  const dimensions: domain(1) = [1..dimension];
  
  var low_coord, high_coord: dimension*real,
      low_index, high_index: dimension*int,
      num_cells, num_ghost_cells: dimension*int;

  var dx: dimension*real;
          
  var all_cells:      domain(dimension, stridable=true),
      physical_cells: subdomain(all_cells);
  
  var interfaces: [dimensions] domain(dimension, stridable=true);
  
  var lower_ghost_cells: [dimensions] subdomain(all_cells),
      upper_ghost_cells: [dimensions] subdomain(all_cells);
  
  var periodic_image_of_lower_ghost_cells: [dimensions] subdomain(all_cells),
      periodic_image_of_upper_ghost_cells: [dimensions] subdomain(all_cells);
}
//<=== Definition of class RectangularGrid <===
//<============================================




//===> RectangularGrid constructor ===>
//====================================>
def RectangularGrid.RectangularGrid(low_coord_input:       dimension*real,
                                    high_coord_input:      dimension*real,
                                    low_index_input:       dimension*int,
                                    num_cells_input:       dimension*int,
                                    num_ghost_cells_input: dimension*int)
{
  //---- Used for modifying domains; needed for
  // dimensionally generic code. --------------
  var size: dimension*int;

  //---- Core parameters ----
  low_coord       = low_coord_input;
  high_coord      = high_coord_input;
  for d in dimensions do assert(low_coord(d) < high_coord(d), 
                                "grid_base_defs.chpl: Error: low_coord exceeds high_coord");
  
  low_index       = low_index_input;
  num_cells       = num_cells_input;
  num_ghost_cells = num_ghost_cells_input;

  [d in dimensions] high_index(d) = low_index(d) + 2*num_cells(d);
  dx = (high_coord - low_coord) / num_cells;


  
  //---- Cell domains ----
  select dimension {
    when 1 do physical_cells = [1..2*num_cells(1)-1 by 2];
    when 2 do physical_cells = [1..2*num_cells(1)-1 by 2, 1..2*num_cells(2)-1 by 2];
    when 3 do physical_cells = [1..2*num_cells(1)-1 by 2, 1..2*num_cells(2)-1 by 2, 1..2*num_cells(3) by 2];
    otherwise assert(0, "dimension>3 not currently allowed");
  }
  
  [d in dimensions] size(d) = 2*num_ghost_cells(d);
  all_cells = physical_cells.expand(size);
         

  //===> Orientation-dependent domains ===>
  for d in dimensions do {
    //---- Cell-to-cell interfaces ----
    [d_temp in dimensions] size(d_temp) = 0;
    size(d) = -1;
    interfaces(d) = all_cells.expand(size);
    
    //---- Ghost cells ----
    // Note that all ghost cell domains contain the corners.
    [d_temp in dimensions] size(d_temp) = 0;

    size(d) = -(2*num_ghost_cells(d) - 1);
    lower_ghost_cells(d) = all_cells.interior(size);

    size(d) = 2*num_ghost_cells(d) - 1;
    upper_ghost_cells(d) = all_cells.interior(size);
          
    size(d) = 2*num_cells(d);
    periodic_image_of_lower_ghost_cells(d) = lower_ghost_cells(d).translate(size);
    
    size(d) = -2*num_cells(d);
    periodic_image_of_upper_ghost_cells(d) = upper_ghost_cells(d).translate(size);

  }   
  //<=== Orientation-dependent domains <===   
  
}
//<====================================
//<=== RectangularGrid constructor <===




//===> Generate coordinates of cell centers ===>
//=============================================>
def RectangularGrid.cell_center_coordinates (cells) {

  var coordinates: [cells] dimension*real;

  if dimension == 1 then {
    forall (cell,d) in [cells,dimensions] do
      coordinates(cell)(d) = low_coord(d) + cell*dx(d)/2.0;
  }
  else {
    forall (cell,d) in [cells,dimensions] do
      coordinates(cell)(d) = low_coord(d) + cell(d)*dx(d)/2.0;
  }

  return coordinates;
}
//<=============================================
//<=== Generate coordinates of cell centers <===




//===> Locating interfaces that neighbor a cell, and vice versa ===>
//=================================================================>
def RectangularGrid.lower_interface(cell: dimension*int, d: int) {
  var interface = cell;
  interface(d) -= 1;
  return interface;
}

def RectangularGrid.upper_interface(cell: dimension*int, d: int) {
  var interface = cell;
  interface(d) += 1;
  return interface;
}

def RectangularGrid.lower_cell(cell: dimension*int, d: int) {
  var cell_out: dimension*int = cell;
  cell_out(d) -= 2;
  return cell_out;
}

def RectangularGrid.upper_cell(cell: dimension*int, d: int) {
  var cell_out: dimension*int = cell;
  cell_out(d) += 2;
  return cell_out;
}
//<=================================================================
//<=== Locating interfaces that neighbor a cell, and vice versa <===




//===> Output a GridFunction in Clawpack format ===>
//=================================================>
def RectangularGrid.clawpack_output(q: GridFunction, frame_number: int) {


  //---- Make sure that q lives on this grid ----
  assert(q.parent_grid == this);


  //---- Parameters needed by the output file ----
  var meqn:        int = 1,
      ngrids:      int = 1,
      maux:        int = 0,
      grid_number: int = 1,
      AMR_level:   int = 1;


  //---- Formatting parameters ----
  var efmt:  string = "%26.16E",
      ifmt:  string = "%5i",
      sfmt:  string = "%20s",
      linelabel: string;


  //---- Names of output files ----
  var frame_string:          string = format("%04i", frame_number),
      name_of_time_file:     string = "_output/fort.t" + frame_string,
      name_of_solution_file: string = "_output/fort.q" + frame_string;


  //---- Write time file ----
  var outfile = new file(name_of_time_file, FileAccessMode.write);
  outfile.open();
  outfile.writeln( format(efmt, q.time),    "    time");
  outfile.writeln( format(ifmt, meqn),      "                 meqn");
  outfile.writeln( format(ifmt, ngrids),    "                 ngrids");
  outfile.writeln( format(ifmt, maux),      "                 naux");
  outfile.writeln( format(ifmt, dimension), "                 ndim");
  outfile.writeln("");
  outfile.writeln("");
  outfile.close();
  delete outfile;


  //---> Write solution file --->
  //---------------------------->
  outfile = new file(name_of_solution_file, FileAccessMode.write);
  outfile.open();
  outfile.writeln( format(ifmt, grid_number), "                 grid_number");
  outfile.writeln( format(ifmt, AMR_level),   "                 AMR_level");


  //---- Write num_cells ----
  for d in dimensions do {
    select d {
      when 1 do linelabel = "                 mx";
      when 2 do linelabel = "                 my";
      when 3 do linelabel = "                 mz";
      otherwise linelabel = "                 mx(" + format("%1i",d) + ")";
    }
    outfile.writeln( format(ifmt, num_cells(d)),  linelabel);
  }


  //---- Write low_coord ----
  for d in dimensions do {
    select d {
      when 1 do linelabel = "    xlow";
      when 2 do linelabel = "    ylow";
      when 3 do linelabel = "    zlow";
      otherwise linelabel = "    xlow(" + format("%1i",d) + ")";
    }
    outfile.writeln( format(efmt, low_coord(d)),  linelabel);
  }


  //---- Write dx ----
  for d in dimensions do {
    select d {
      when 1 do linelabel = "    dx";
      when 2 do linelabel = "    dy";
      when 3 do linelabel = "    dz";
      otherwise linelabel = "    dx(" + format("%1i",d) + ")";
    }
    outfile.writeln( format(efmt, dx(d)),  linelabel);
  }
  outfile.writeln("");


  //---- Write solution values ----
  var physical_cells_transposed: domain(dimension, stridable=true),
      shape: dimension*int;
  


  if dimension == 1 then {
    for cell in physical_cells do
   outfile.writeln(format(efmt, q.value(cell)));
  }
  else {

    select dimension {
      when 2 do physical_cells_transposed = [1..2*num_cells(2)-1 by 2, 1..2*num_cells(1)-1 by 2];
      when 3 do physical_cells_transposed = [1..2*num_cells(3)-1 by 2, 1..2*num_cells(2)-1 by 2, 1..2*num_cells(1) by 2];
      otherwise assert(0, "dimension <=3 not currently supported by clawpack_output");
    }


    var cell: dimension*int;
    for cell_transposed in physical_cells_transposed do {
      [d in dimensions] cell(d) = cell_transposed(1 + dimension - d);
      outfile.writeln(format(efmt, q.value(cell)));
      for d in dimensions do {
        if cell(d) == physical_cells.dim(d).high then
          outfile.writeln("  ");
        else
          break;
      }
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
def RectangularGrid.evaluate(f) {
  var q = new GridFunction(this);
  var coordinates = cell_center_coordinates(physical_cells);
  [cell in physical_cells] q.value(cell) = f(coordinates(cell));
  return q;
}
//<======================================================
//<=== Evaluating an analytical function on the grid <===






//===> Definition of class GridFunction ===>
//=========================================>
class GridFunction {
  var parent_grid: RectangularGrid;
  var value: [parent_grid.all_cells] real;
  var time: real;
}
//<=========================================
//<=== Definition of class GridFunction ====



