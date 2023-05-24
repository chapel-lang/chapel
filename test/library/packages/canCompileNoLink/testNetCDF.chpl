use NetCDF;
use NetCDF.C_NetCDF;

var f = ncopen(c_ptrToConst_helper("ppt2020_08_20.nc"), NC_WRITE);
var status: int = nc_close(f);

