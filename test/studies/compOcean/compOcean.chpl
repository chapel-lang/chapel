use NetCDF.C_NetCDF;
require "compOcean.h";

// This is a Chapel port of an ocean modeling example in Python from
// Andrew Shao located at:
// https://github.com/ashao/chapel_cmip/blob/master/python-examples/enso_sst_covariance.ipynb

const dataPath = "./",
      filename = "tos_Omon_GFDL-ESM2G_historical_r1i1p1_200101-200512.nc",
      fullPath = dataPath + filename;

const nan = 0.0:real(32) / 0.0:real(32);

proc cdfError(e) {
  if e != NC_NOERR {
    writeln("Error: ", nc_strerror(e): string);
    exit(2);
  }
}

proc readData(param dims: int, dataname: string) {
  var ncid, varid, ndims, natts: c_int;
  var nctype: nc_type;

  var name: c_string;

  cdfError(nc_open((fullPath).c_str(), NC_NOWRITE, ncid));

  cdfError(nc_inq_varid(ncid, dataname.c_str(), varid));
  cdfError(nc_inq_varndims(ncid, varid, ndims));

  assert(ndims == dims);

  var dimids: [0..#ndims] c_int;
  var dimlens: [0..#ndims] size_t;

  cdfError(nc_inq_vardimid(ncid, varid, dimids[0]));
  extern proc nc_inq_dimlen_WAR(ncid:c_int, dimid: c_int, ref dimlens): c_int;
  var dimranges: dims*range;
  for i in 0..#ndims {
    cdfError(nc_inq_dimlen_WAR(ncid, dimids[i], dimlens[i]));
    dimranges(i+1) = 0..#dimlens[i];
  }

  var data: [(...dimranges)] real(32);
  var zero: dims*int;

  cdfError(nc_get_var_float(ncid, varid, data[(...zero)]));

  cdfError(nc_close(ncid));

  return data;
}

var tos = readData(3, "tos"),
    lat = readData(2, "lat"),
    lon = readData(2, "lon");

var (ntime, nlat, nlon) = tos.shape;

// set data that is out of the expected range to NaN
for t in tos do if t > 1000.0 then t = nan;

var monthlyAverage: [0..#12, 0..#nlat, 0..#nlon] real(64);
const nyears = ntime / 12;

for jtime in 0..#ntime {
  const currentMonth = jtime % 12;
  monthlyAverage[currentMonth, .., ..] += tos[jtime, .., ..];
}

monthlyAverage /= nyears: real(64);

var tosAnomaly: [0..#ntime, 0..#nlat, 0..#nlon] real(64);
for jtime in 0..#ntime {
  const currentMonth = jtime % 12;
  tosAnomaly[jtime, .., ..] = tos[jtime, .., ..] - monthlyAverage[currentMonth, .., ..];
}

const ninoMinlat = -5, ninoMaxlat = 5,
      ninoMinlon = -170, ninoMaxlon = -120;
var ninoIndex: [0..#ntime] real(64);
var totalArea = 0;

for jlat in 0..#nlat {
  if jlat % 10 == 0 then writeln(jlat);

  for jlon in 0..#nlon {
    const inregion = lat[jlat, jlon] >= ninoMinlat &&
                     lat[jlat, jlon] <= ninoMaxlat &&
                     lon[jlat, jlon] >= ninoMinlon &&
                     lon[jlat, jlon] <= ninoMaxlon;
    if inregion {
      totalArea += 1;
      for jtime in 0..#ntime {
        ninoIndex[jtime] += tosAnomaly[jtime, jlat, jlon];
      }
    }
  }
}

ninoIndex /= totalArea: real(64);

var nino_sst_covariance: [0..#nlat, 0..#nlon] real(64);
for jtime in 0..#ntime do
  for jlat in 0..#nlat do
    for jlon in 0..#nlon do
      nino_sst_covariance[jlat, jlon] += tosAnomaly[jtime, jlat, jlon] *
                                         ninoIndex[jtime];

nino_sst_covariance = nino_sst_covariance / (ntime - 1):real(64);

writeln(nino_sst_covariance);
//plt.pcolormesh(nino_sst_covariance); plt.colorbar(); plt.show();
