class Lower {
  proc something() {
    writeln("in foo");
  }
}

class Upper {
  forwarding var low : owned Lower = new owned Lower();
}

var u = new owned Upper();

u.something();
