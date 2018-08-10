use NetCDF.C_NetCDF;

// This code is based on the example pres_temp_4D_wr.c from:
// https://www.unidata.ucar.edu/software/netcdf/examples/programs/

/* This is the name of the data file we will read. */
param filename = "pres_temp_4D.nc";

/* We are reading 4D data, a 2 x 6 x 12 lvl-lat-lon grid, with 2
   timesteps of data. */
param ndims = 4,
      nlat = 6,
      nlon = 12,
      latName = "latitude",
      lonName = "longitude",
      nrec = 2,
      nlvl = 2;

/* Names of things. */
param presName = "pressure",
      tempName = "temperature";

/* These are used to calculate the values we expect to find. */
const samplePressure = 900.0,
      sampleTemp = 9.0,
      startLat = 25.0,
      startLon = -125.0;

proc cdfError(e) {
  if e != NC_NOERR {
    writeln("Error: ", nc_strerror(e): string);
    exit(2);
  }
}

proc main {
  var ncid, pres_varid, temp_varid: c_int;
  var lat_varid, lon_varid: c_int;

  /* The start and count arrays will tell the netCDF library where to
     read our data. */
  var start, count: [0..#ndims] size_t;

  /* Program variables to hold the data we will read. We will only
     need enough space to hold one timestep of data; one record. */
  var pres_in, temp_in: [0..#nlvl, 0..#nlat, 0..#nlon] real(32);

  /* These program variables hold the latitudes and longitudes. */
  var lats: [0..#nlat] real(32),
      lons: [0..#nlon] real(32);

  /* Loop indexes. */
  var lvl, lat, lon, rec, i = 0;

  /* Open the file. */
  cdfError(nc_open(filename, NC_NOWRITE, ncid));

  /* Get the varids of the latitude and longitude coordinate
   * variables. */
  cdfError(nc_inq_varid(ncid, latName, lat_varid));
  cdfError(nc_inq_varid(ncid, lonName, lon_varid));

  /* Read the coordinate variable data. */
  cdfError(nc_get_var_float(ncid, lat_varid, lats[0]));
  cdfError(nc_get_var_float(ncid, lon_varid, lons[0]));

  /* Check the coordinate variable data. */
  for lat in 0..#nlat do
    if lats[lat] != startLat + 5.0*lat {
      writeln("Found a mismatched lattitued at #", lat);
      return 2;
    }
  for lon in 0..#nlon do
    if lons[lon] != startLon + 5.0*lon {
      writeln("Found a mismatched longitude at #", lon);
      return 2;
    }

  /* Get the varids of the pressure and temperature netCDF
   * variables. */
  cdfError(nc_inq_varid(ncid, presName, pres_varid));
  cdfError(nc_inq_varid(ncid, tempName, temp_varid));

  /* Read the data. Since we know the contents of the file we know
   * that the data arrays in this program are the correct size to
   * hold one timestep. */
  count[0] = 1;
  count[1] = nlvl;
  count[2] = nlat;
  count[3] = nlon;
  start[1] = 0;
  start[2] = 0;
  start[3] = 0;

  /* Read and check one record at a time. */
  for rec in 0..#nrec {
    start[0] = rec: size_t;
    extern proc nc_get_vara_float_WAR(ncid: c_int, pres_varid: c_int, ref start: size_t, ref count: size_t, ref pres_out: real(32)): c_int;
    cdfError(nc_get_vara_float_WAR(ncid, pres_varid, start[0],
                                   count[0], pres_in[0, 0, 0]));
    cdfError(nc_get_vara_float_WAR(ncid, temp_varid, start[0],
                                   count[0], temp_in[0, 0, 0]));

    /* Check the data. */
    i = 0;
    for lvl in 0..#nlvl do
      for lat in 0..#nlat do
        for lon in 0..#nlon {
          if pres_in[lvl, lat, lon] != samplePressure + i {
            writeln("Found a bad pressure at ", (lvl, lat, lon));
            return 2;
          }
          if temp_in[lvl, lat, lon] != sampleTemp + i {
            writeln("Found a bad temperature at ", (lvl, lat, lon));
            return 2;
          }
          i += 1;
        }
  } /* next record */

  /* Close the file. */
  cdfError(nc_close(ncid));

  writeln("*** SUCCESS reading example file pres_temp_4D.nc!");
  return 0;
}

