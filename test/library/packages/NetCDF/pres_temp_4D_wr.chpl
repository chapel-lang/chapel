use NetCDF.C_NetCDF;

// This code is based on the example pres_temp_4D_wr.c from:
// https://www.unidata.ucar.edu/software/netcdf/examples/programs/

/* This is the name of the data file we will create. */
param filename = "pres_temp_4D.nc";

/* We are writing 4D data, a 2 x 6 x 12 lvl-lat-lon grid, with 2
   timesteps of data. */
param ndims = 4,
      nlat = 6,
      nlon = 12,
      latName = "latitude",
      lonName = "longitude",
      nrec = 2,
      recName = "time",
      lvlName = "level",
      nlvl = 2;

/* Names of things. */
param presName = "pressure",
      tempName = "temperature",
      units = "units",
      degreesEast = "degrees_east",
      degreesNorth = "degrees_north";

/* These are used to construct some example data. */
const samplePressure = 900.0: real(32),
      sampleTemp = 9.0: real(32),
      startLat = 25.0: real(32),
      startLon = -125.0: real(32);

/* For the units attributes. */
param presUnits = "hPa",
      tempUnits = "celsius";

proc cdfError(e) {
  if e != NC_NOERR {
    writeln("Error: ", nc_strerror(e): string);
    exit(2);
  }
}

proc main {
  /* IDs for the netCDF file, dimensions, and variables. */
  var ncid, lon_dimid, lat_dimid, lvl_dimid, rec_dimid: c_int;
  var lat_varid, lon_varid, pres_varid, temp_varid: c_int;
  var dimids: [0..#ndims] c_int;

  /* The start and count arrays will tell the netCDF library where to
     write our data. */
  var start, count: [0..#ndims] size_t;

  /* Program variables to hold the data we will write out. We will only
     need enough space to hold one timestep of data; one record. */
  var pres_out: [0..#nlvl, 0..#nlat, 0..#nlon] real(32);
  var temp_out: [0..#nlvl, 0..#nlat, 0..#nlon] real(32);

  /* These program variables hold the latitudes and longitudes. */
  var lats: [0..#nlat] real(32),
      lons: [0..#nlon] real(32);

  /* Loop indexes. */
  var lvl, lat, lon, rec, i: c_int = 0;

  /* Error handling. */
  var retval: c_int;

  /* Create some pretend data. If this wasn't an example program, we
   * would have some real data to write, for example, model
   * output. */
  for lat in 0..#nlat do
    lats[lat] = (startLat + 5.0*lat): real(32);
  for lon in 0..#nlon do
    lons[lon] = (startLon + 5.0*lon): real(32);

  for lvl in 0..#nlvl do
    for lat in 0..#nlat do
      for lon in 0..#nlon {
        pres_out[lvl, lat, lon] = (samplePressure + i): real(32);
        temp_out[lvl, lat, lon] = (sampleTemp + i): real(32);
        i += 1;
      }

  /* Create the file. */
  cdfError(nc_create(filename, NC_CLOBBER, ncid));

  /* Define the dimensions. The record dimension is defined to have
   * unlimited length - it can grow as needed. In this example it is
   * the time dimension.*/
  cdfError(nc_def_dim(ncid, lvlName, nlvl, lvl_dimid));
  cdfError(nc_def_dim(ncid, latName, nlat, lat_dimid));
  cdfError(nc_def_dim(ncid, lonName, nlon, lon_dimid));
  cdfError(nc_def_dim(ncid, recName, NC_UNLIMITED: size_t, rec_dimid));

  /* Define the coordinate variables. We will only define coordinate
     variables for lat and lon.  Ordinarily we would need to provide
     an array of dimension IDs for each variable's dimensions, but
     since coordinate variables only have one dimension, we can
     simply provide the address of that dimension ID (&lat_dimid) and
     similarly for (&lon_dimid). */
  cdfError(nc_def_var(ncid, latName, NC_FLOAT, 1, lat_dimid,
           lat_varid));
  cdfError(nc_def_var(ncid, lonName, NC_FLOAT, 1, lon_dimid,
           lon_varid));

  /* Assign units attributes to coordinate variables. */
  cdfError(nc_put_att_text(ncid, lat_varid, units,
                           degreesNorth.numBytes, degreesNorth));
  cdfError(nc_put_att_text(ncid, lon_varid, units,
                           degreesEast.numBytes, degreesEast));

  /* The dimids array is used to pass the dimids of the dimensions of
     the netCDF variables. Both of the netCDF variables we are
     creating share the same four dimensions. In C, the
     unlimited dimension must come first on the list of dimids. */
  dimids[0] = rec_dimid;
  dimids[1] = lvl_dimid;
  dimids[2] = lat_dimid;
  dimids[3] = lon_dimid;

  /* Define the netCDF variables for the pressure and temperature
   * data. */
  cdfError(nc_def_var(ncid, presName, NC_FLOAT, ndims,
                      dimids[0], pres_varid));
  cdfError(nc_def_var(ncid, tempName, NC_FLOAT, ndims,
                      dimids[0], temp_varid));

  /* Assign units attributes to the netCDF variables. */
  cdfError(nc_put_att_text(ncid, pres_varid, units,
                           presUnits.numBytes, presUnits));
  cdfError(nc_put_att_text(ncid, temp_varid, units,
                           tempUnits.numBytes, tempUnits));

  /* End define mode. */
  cdfError(nc_enddef(ncid));

  /* Write the coordinate variable data. This will put the latitudes
     and longitudes of our data grid into the netCDF file. */
  cdfError(nc_put_var_float(ncid, lat_varid, lats[0]));
  cdfError(nc_put_var_float(ncid, lon_varid, lons[0]));

  /* These settings tell netcdf to write one timestep of data. (The
     setting of start[0] inside the loop below tells netCDF which
     timestep to write.) */
  count[0] = 1;
  count[1] = nlvl;
  count[2] = nlat;
  count[3] = nlon;
  start[1] = 0;
  start[2] = 0;
  start[3] = 0;

  /* Write the pretend data. This will write our surface pressure and
     surface temperature data. The arrays only hold one timestep worth
     of data. We will just rewrite the same data for each timestep. In
     a real application, the data would change between timesteps. */
  extern proc nc_put_vara_float_WAR(ncid: c_int, pres_varid: c_int, ref start: size_t, ref count: size_t, ref pres_out: real(32)): c_int;
  for rec in 0..#nrec {
    start[0] = rec: size_t;
    cdfError(nc_put_vara_float_WAR(ncid, pres_varid, start[0], count[0],
                                   pres_out[0, 0, 0]));
    cdfError(nc_put_vara_float_WAR(ncid, temp_varid, start[0], count[0],
                                   temp_out[0, 0, 0]));
  }

  /* Close the file. */
  cdfError(nc_close(ncid));

  writeln("*** SUCCESS writing example file ", filename);
  return 0;
}
