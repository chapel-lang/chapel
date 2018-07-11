var str = "a global string";

class Unallocated {
  var a, b: int;
  proc method() {
    a = 1;
    return str;
  }
}

proc main {
  var aaa: unmanaged Unallocated;
  writeln(aaa.method());
}
