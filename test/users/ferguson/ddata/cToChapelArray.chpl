use SysCTypes;

  /*forall i in 1..4 {
    writeln("This should be a chunk: {", "\n a", "\n b", "\n}");
  }

  record MyThing {
    proc writeThis(w:Writer) throws {
      w.writeln("This should be a chunk: {");
      w.writeln(" a");
      w.writeln(" b");
      w.writeln("}");
    }
  }

  forall i in 1..4 {
    var t:MyThing;
    write(t);
  }
*/

param size = 10;

pragma "local args"
export proc chapelProc( array: _ddata(c_int) )
{
  writeln("in chapelProc");
  for i in 0..#size {
    writeln("a[", i, "] is ", array[i]);
  }
  writeln("done chapelProc");
}

extern proc cProc( arg:_ddata(c_int) );

var a:_ddata(c_int);

a = _ddata_allocate(c_int, size);

for i in 0..#size {
  a[i] = i:c_int;
}

writeln("before chapelProc, a[1] is ", a[1]);
chapelProc(a);
writeln("after chapelProc, a[1] is ", a[1]);
writeln();

writeln("Hello from Chapel, calling C");
writeln("before cProc, a[1] is ", a[1]);
cProc(a);
writeln("after cProc, a[1] is ", a[1]);
writeln("Hello from Chapel, done");

_ddata_free(a, size);

