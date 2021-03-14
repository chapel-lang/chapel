class Lower {
  proc something() {
    writeln("in foo");
  }
}

class Upper {
  var low = new shared Lower();
  forwarding low;
}

var u = new shared Upper();

u.something();
