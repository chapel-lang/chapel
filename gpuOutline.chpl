record GPUAbleArray {
  type eltType;
  var size = 0: int;
  forwarding var data: _ddata(eltType);

  proc postinit() {
    var dummy = false;
    data = _ddata_allocate_noinit(eltType, size, dummy);
  }
}

var a = new GPUAbleArray(int, 10);
var b = new GPUAbleArray(int, 10);

for i in 0..9 do a[i] = 1;
for i in 0..9 do b[i] = 2;

forall i in 0..9 {
  a[i] = b[i];
}

for i in 0..9 do write(a[i], " ");
writeln();
