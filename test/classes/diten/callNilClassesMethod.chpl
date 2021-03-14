var str = "a global string";

class Unallocated {
  var a, b: int;
  proc method() {
    a = 1;
    return str;
  }
}
pragma "unsafe" var ARR: [LocaleSpace] unmanaged Unallocated;
proc main {
  var aaa = ARR[0]; // nil of non-nilablel type, due to pragma "unsafe"
  writeln(aaa.method());
}
