use NetCDF.C_NetCDF;

// This code is based on the example sfc_pres_temp_wr.c from:
// https://www.unidata.ucar.edu/software/netcdf/examples/programs/

config const filename = "sfc_pres_temp.nc";

param ndims = 2,
      nlat = 6,
      nlon = 12;

param latName = "latitude",
      lonName = "longitude",
      presName = "pressure",
      tempName = "temperature",
      units = "units",
      degEast = "degrees_east",
      degNorth = "degrees_north";

const samplePressure = 900.0,
      sampleTemp = 9.0,
      startLat = 25.0,
      startLon = -125.0;

param ERRCODE = 2;

proc cdfError(e) {
  if e != NC_NOERR {
    writeln("Error: ", nc_strerror(e): string);
    exit(ERRCODE);
  }
}

proc main {
  var dimids: [0..#ndims] c_int;

  var presOut, tempOut: [0..#nlat, 0..#nlon] real(32);

  var lats: [0..#nlat] real(32),
      lons: [0..#nlon] real(32);

  const presUnits = "hPa", tempUnits = "celsius";

  var ncid, lonDimId, latDimId, presVarId,
      tempVarId, latVarId, lonVarId: c_int;

  /* Create some pretend data. If this wasn't an example program, we
     would have some real data to write, for example, model
     output. */
  for lat in 0..#nlat do lats[lat] = (startLat + 5.0 * lat): real(32);
  for lon in 0..#nlon do lons[lon] = (startLon + 5.0 * lon): real(32);

  for lat in 0..#nlat {
    for lon in 0..#nlon {
      presOut[lat, lon] = (samplePressure + (lon * nlat + lat)): real(32);
      tempOut[lat, lon] = (sampleTemp + 0.25 * (lon * nlat + lat)): real(32);
    }
  }

  /* Create the file. */
  cdfError(nc_create(filename.c_str(), NC_CLOBBER, ncid));

  /* Define the dimensions. */
  cdfError(nc_def_dim(ncid, latName, nlat, latDimId));
  cdfError(nc_def_dim(ncid, lonName, nlon, lonDimId));

  /* Define coordinate netCDF variables.  They will hold the
     coordinate information, that is, the latitudes and longitudes. A
     varid is returned for each. */
  cdfError(nc_def_var(ncid, latName, NC_FLOAT, 1, latDimId, latVarId));
  cdfError(nc_def_var(ncid, lonName, NC_FLOAT, 1, lonDimId, lonVarId));

  /* Define units attributes for coordinate vars. This attaches a
     text attribute to each of the coordinate variables, containing
     the units. Note that we are not writing a trailing NULL, just
     "units", because the reading program may be fortran which does
     not use null-terminated strings. In general it is up to the
     reading C program to ensure that it puts null-terminators on
     strings where necessary. */
  cdfError(nc_put_att_text(ncid, latVarId, units,
                           degNorth.numBytes, degNorth.c_str()));
  cdfError(nc_put_att_text(ncid, lonVarId, units,
                           degEast.numBytes, degEast.c_str()));

  /* Define the netCDF variables. The dimids array is used to pass
     the dimids of the dimensions of the variables. */
  dimids[0] = latDimId;
  dimids[1] = lonDimId;

  cdfError(nc_def_var(ncid, presName, NC_FLOAT,
                      ndims, dimids[0], presVarId));
  cdfError(nc_def_var(ncid, tempName, NC_FLOAT,
                      ndims, dimids[0], tempVarId));

  /* Define units attributes for vars. */
  cdfError(nc_put_att_text(ncid, presVarId, units.c_str(),
                           presUnits.numBytes:size_t, presUnits.c_str()));
  cdfError(nc_put_att_text(ncid, tempVarId, units.c_str(),
                           tempUnits.numBytes:size_t, tempUnits.c_str()));

  /* End define mode. */
  cdfError(nc_enddef(ncid));

  /* Write the coordinate variable data. This will put the latitudes
     and longitudes of our data grid into the netCDF file. */
  cdfError(nc_put_var_float(ncid, latVarId, lats[0]));
  cdfError(nc_put_var_float(ncid, lonVarId, lons[0]));

  /* Write the pretend data. This will write our surface pressure and
     surface temperature data. The arrays of data are the same size
     as the netCDF variables we have defined. */
  cdfError(nc_put_var_float(ncid, presVarId, presOut[0,0]));
  cdfError(nc_put_var_float(ncid, tempVarId, tempOut[0,0]));

  /* Close the file. */
  cdfError(nc_close(ncid));

  writeln("*** SUCCESS writing example file sfc_pres_temp.nc!");
}
