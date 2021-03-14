use NetCDF.C_NetCDF;

// This code is based on the example simple_xy_rd.c from:
// https://www.unidata.ucar.edu/software/netcdf/examples/programs/

config const filename = "simple_xy.nc";

param NX = 6, NY = 12;

param ERRCODE = 2;

proc cdfError(e) {
  if e != NC_NOERR {
    writeln("Error: ", nc_strerror(e): string);
    exit(ERRCODE);
  }
}

proc main {
  var ncid, varid: c_int;
  var data: [1..NX, 1..NY] c_int;

  /* Open the file. NC_NOWRITE tells netCDF we want read-only access
   * to the file.*/
  cdfError(nc_open(filename.c_str(), NC_NOWRITE, ncid));

  /* Get the varid of the data variable, based on its name. */  
  cdfError(nc_inq_varid(ncid, c"data", varid));

  /* Read the data. */
  cdfError(nc_get_var_int(ncid, varid, data[1,1]));

  /* Check the data. */
  for x in 0..#NX do
    for y in 0..#NY do
      if data[x+1, y+1] != x*NY + y {
        writeln("Error, incorrect data found at (", x+1, ", ", y+1, "): ",
                data[x+1, y+1], " != ", x*NY + y);
        exit(ERRCODE);
      }

  cdfError(nc_close(ncid));

  writeln("*** SUCCESS reading example file ", filename, "!");
}
