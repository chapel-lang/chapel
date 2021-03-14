record R {
  var x : int;
  proc init() {
    writeln("R.init()");
    this.x = 0;
  }
  proc init=(other:R) {
    this.x = other.x + 1;
    writeln("R.init=(R): ", this.x);
  }
  proc deinit() {
    writeln("R.deinit(): ", this.x);
  }
}

class C {
  var x : R;

  proc init() {
    writeln("start: C.init()");
    this.x = new R();
    writeln("end: C.init()");
  }
}

// We should see a matching number of calls to R.init and R.deinit, with one
// of the R.init calls being a copy init.

var c = new unmanaged C();
const x : R = c.x;
writeln("temp = ", x);
delete c;
