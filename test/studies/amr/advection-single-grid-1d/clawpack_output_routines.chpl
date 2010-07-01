module clawpack_output_routines {

  use grid1d_definitions;

  var meqn: int = 1,
    ngrids: int = 1,
    maux: int = 0,
    ndim: int = 1,
    grid_number: int = 1,
    AMR_level: int = 1;

  var efmt: string = "%16.8e";
  var ifmt: string = "%16i";
  var sfmt: string = "%16s";


  def write_output(frame_number: int, time: real, q: [] real, grid: GridData){

    var frame_string: string = format("%04i", frame_number);
    var name_of_time_file: string = "_output/fort.t" + frame_string;
    var name_of_solution_file: string = "_output/fort.q" + frame_string;

    var outfile = new file(name_of_time_file, FileAccessMode.write);

    outfile.open();
    outfile.writeln( format(efmt, time),   format(sfmt, "time"));
    outfile.writeln( format(ifmt, meqn),   format(sfmt, "meqn"));
    outfile.writeln( format(ifmt, ngrids), format(sfmt, "ngrids"));
    outfile.writeln( format(ifmt, maux),   format(sfmt, "maux"));
    outfile.writeln( format(ifmt, ndim),   format(sfmt, "ndim"));
    outfile.close();

    delete outfile;

    outfile = new file(name_of_solution_file, FileAccessMode.write);
    
    outfile.open();
    outfile.writeln( format(ifmt, grid_number),    format(sfmt, "grid_number"));
    outfile.writeln( format(ifmt, AMR_level),      format(sfmt, "AMR_level"));
    outfile.writeln( format(ifmt, grid.num_cells), format(sfmt, "mx"));
    outfile.writeln( format(efmt, grid.x_low),     format(sfmt, "xlow"));
    outfile.writeln( format(efmt, grid.dx),        format(sfmt, "dx"));
    outfile.writeln("");

    for cell in grid.physical_cells do
      outfile.writeln(format(efmt, q(cell)));
    
    outfile.close();

  }
}