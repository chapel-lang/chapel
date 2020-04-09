
class Lower {
  type eltType;
  proc something(foo) {
    writeln("foo = ", foo.type:string);
  }
}

class Upper {
  type eltType;
  forwarding var low : unmanaged Lower(eltType) = new unmanaged Lower(eltType);

  proc deinit() {
    delete low;
  }
}

var u = new unmanaged Upper(int);

u.something(u);

delete u;
