use NetCDF.C_NetCDF, Buffers;

// This code is based on the example sfc_pres_temp_rd.c from:
// https://www.unidata.ucar.edu/software/netcdf/examples/programs/

/* This is the name of the data file we will read. */
param filename = "sfc_pres_temp.nc";

/* We are reading 2D data, a 6 x 12 lat-lon grid. */
param ndims = 2,
      nlat = 6,
      nlon = 12;

param latName = "latitude",
      lonName = "longitude",
      presName = "pressure",
      tempName = "temperature";

/* These are used to calculate the values we expect to find. */
const samplePressure = 900.0,
      sampleTemp = 9.0,
      startLat = 25.0,
      startLon = -125.0;

/* For the units attributes. */
param units = "units",
      presUnits = "hPa",
      tempUnits = "celsius",
      latUnits = "degrees_north",
      lonUnits = "degrees_east",
      maxAttLen = 80;

/* Handle errors by printing an error message and exiting with a
 * non-zero status. */
proc cdfError(e) {
  if e != NC_NOERR {
    writeln("Error: ", nc_strerror(e): string);
    exit(2);
  }
}

proc main {
  var ncid, pres_varid, temp_varid: c_int;
  var lat_varid, lon_varid: c_int;

  /* We will read surface temperature and pressure fields. */
  var presIn:[0..#nlat, 0..#nlon] real(32);
  var tempIn:[0..#nlat, 0..#nlon] real(32);

  /* For the lat lon coordinate variables. */
  var latsIn: [0..#nlat] real(32),
      lonsIn: [0..#nlon] real(32);

  /* To check the units attributes. */
  var presUnitsIn = new byteBuffer(maxAttLen),
      tempUnitsIn = new byteBuffer(maxAttLen);
  var latUnitsIn = new byteBuffer(maxAttLen),
      lonUnitsIn = new byteBuffer(maxAttLen);

  /* We will learn about the data file and store results in these
     program variables. */
  var ndims_in, nvars_in, ngatts_in, unlimdimid_in: c_int;

  /* Open the file. */
  cdfError(nc_open(filename, NC_NOWRITE, ncid));

  /* There are a number of inquiry functions in netCDF which can be
     used to learn about an unknown netCDF file. NC_INQ tells how
     many netCDF variables, dimensions, and global attributes are in
     the file; also the dimension id of the unlimited dimension, if
     there is one. */
  cdfError(nc_inq(ncid, ndims_in, nvars_in, ngatts_in, unlimdimid_in));

  /* In this case we know that there are 2 netCDF dimensions, 4
     netCDF variables, no global attributes, and no unlimited
     dimension. */
  if ndims_in != 2 || nvars_in != 4 || ngatts_in != 0 || unlimdimid_in != -1 {
    writeln("one of the inputs was not as expected");
    return 2;
  }

  /* Get the varids of the latitude and longitude coordinate
     variables. */
  cdfError(nc_inq_varid(ncid, latName, lat_varid));
  cdfError(nc_inq_varid(ncid, lonName, lon_varid));

  /* Read the coordinate variable data. */
  cdfError(nc_get_var_float(ncid, lat_varid, latsIn[0]));
  cdfError(nc_get_var_float(ncid, lon_varid, lonsIn[0]));

  /* Check the coordinate variable data. */
  for lat in 0..#nlat do
    if latsIn[lat] != startLat + 5.0*lat {
      writeln("Found a mismatched lattitued at #", lat);
      return 2;
    }
  for lon in 0..#nlon do
    if lonsIn[lon] != startLon + 5.0*lon {
      writeln("Found a mismatched longitude at #", lon);
      return 2;
    }

  /* Get the varids of the pressure and temperature netCDF
     variables. */
  cdfError(nc_inq_varid(ncid, presName, pres_varid));
  cdfError(nc_inq_varid(ncid, tempName, temp_varid));

  /* Read the data. Since we know the contents of the file we know
     that the data arrays in this program are the correct size to
     hold all the data. */
  cdfError(nc_get_var_float(ncid, pres_varid, presIn[0, 0]));
  cdfError(nc_get_var_float(ncid, temp_varid, tempIn[0, 0]));

  /* Check the data. */
  for lat in 0..#nlat do
    for lon in 0..#nlon do
      if presIn[lat, lon] != samplePressure + (lon * nlat + lat) ||
         tempIn[lat, lon] != sampleTemp + .25 * (lon * nlat + lat) {
        writeln("Found a bad pressure or temperature at ", (lat, lon));
        return 2;
      }

  extern proc nc_get_att_text_void_ptr(ncid: c_int, varid: c_int, field: c_string, p: c_void_ptr): c_int;

  /* Each of the netCDF variables has a "units" attribute. Let's read
     them and check them. */
  cdfError(nc_get_att_text_void_ptr(ncid, lat_varid, units, latUnitsIn.ptr()));
  if latUnitsIn.ptr(): c_string: string != latUnits {
    writeln("Bad units for latUnits");
    return 2;
  }

  cdfError(nc_get_att_text_void_ptr(ncid, lon_varid, units, lonUnitsIn.ptr()));
  if lonUnitsIn.ptr(): c_string: string != lonUnits {
    writeln("Bad units for lonUnits");
    return 2;
  }

  cdfError(nc_get_att_text_void_ptr(ncid, pres_varid, units, presUnitsIn.ptr()));
  if presUnitsIn.ptr(): c_string: string != presUnits {
    writeln("Bad units for presUnits");
    return 2;
  }
  cdfError(nc_get_att_text_void_ptr(ncid, temp_varid, units, tempUnitsIn.ptr()));
  if tempUnitsIn.ptr(): c_string: string != tempUnits {
    writeln("Bad units for tempUnits");
    return 2;
  }

  /* Close the file. */
  cdfError(nc_close(ncid));

  writeln("*** SUCCESS reading example file sfc_pres_temp.nc!");
  return 0;
}
