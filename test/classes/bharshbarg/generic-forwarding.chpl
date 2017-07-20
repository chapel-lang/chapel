
class Lower {
  type eltType;
  proc something(foo) {
    writeln("foo = ", foo.type:string);
  }
}

class Upper {
  type eltType;
  forwarding var low : Lower(eltType) = new Lower(eltType);

  proc deinit() {
    delete low;
  }
}

var u = new Upper(int);

u.something(u);

delete u;
