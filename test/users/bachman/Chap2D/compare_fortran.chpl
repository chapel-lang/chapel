use IO;
use parameters;
use domains;
use arrays;

proc load_1layer_test(inout arr : [?D] real) {

var tmp_arr : [D] real;

var f = open("./FORTRAN/test_grid.dat", iomode.r);
var r = f.reader(kind=ionative);
for j in 0..#ny {
  for i in 0..#nx {
    var tmp : real;
    r.readBinary(tmp);
    tmp_arr[j,i] = tmp;
  }
}
r.close();

arr = tmp_arr;

}

proc print_array_2D(arr : [?dom]) {

var D = dom.localSubdomain();

for i in D.dim(0) {
  writeln(arr.localSlice(D)[i,..] : real);
}
}

proc print_array_2D_i(arr : [?dom]) {

var D = dom.localSubdomain();

for i in D.dim(0) {
  var tmp = arr.localSlice(D)[i,..];
  tmp = -1i * tmp;
  writeln(tmp : real);
  writeln();
}

}

