use NetCDF.C_NetCDF;

// This code is based on the example simple_xy_wr.c from:
// https://www.unidata.ucar.edu/software/netcdf/examples/programs/

config const filename = "simple_xy.nc";

param NDIMS = 2, NX = 6, NY = 12;

param ERRCODE = 2;

proc cdfError(e) {
  if e != NC_NOERR {
    writeln("Error: ", nc_strerror(e): string);
    exit(ERRCODE);
  }
}

proc main {
  var data: [1..NX, 1..NY] c_int;
  var ncid, x_dimid, y_dimid, varid: c_int;
  var dimids: [0..#NDIMS] c_int;

  for x in 0..#NX do
    for y in 0..#NY do
      data[x+1,y+1] = (x*NY + y): c_int;

  /* Create the file. The NC_CLOBBER parameter tells netCDF to
   * overwrite this file, if it already exists. */
  cdfError(nc_create(filename.c_str(), NC_CLOBBER, ncid));

  /* Define the dimensions. NetCDF will hand back an ID for each. */
  cdfError(nc_def_dim(ncid, c"x", NX, x_dimid));
  cdfError(nc_def_dim(ncid, "y", NY, y_dimid));

  /* The dimids array is used to pass the IDs of the dimensions of
   * the variable. */
  dimids[0] = x_dimid;
  dimids[1] = y_dimid;

  /* Define the variable. The type of the variable in this case is
   * NC_INT (4-byte integer). */
  cdfError(nc_def_var(ncid, c"data", NC_INT, NDIMS, dimids[0], varid));

  /* End define mode. This tells netCDF we are done defining
   * metadata. */
  cdfError(nc_enddef(ncid));

  /* Write the pretend data to the file. Although netCDF supports
   * reading and writing subsets of data, in this case we write all
   * the data in one operation. */
  cdfError(nc_put_var_int(ncid, varid, data[1,1]));

  /* Close the file. This frees up any internal netCDF resources
   * associated with the file, and flushes any buffers. */
  cdfError(nc_close(ncid));

  writeln("*** SUCCESS writing example file ", filename, "!");
}
