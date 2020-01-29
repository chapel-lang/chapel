var str = "a global string";

class Unallocated {
  var a, b: int;
  proc method() {
    a = 1;
    return str;
  }
}
var ARR: [LocaleSpace] unmanaged Unallocated;
proc main {
  var aaa = ARR[0]; // nil of non-nilablel type, due to a typechecking loophole
  writeln(aaa.method());
}
