class C {
  type t;
  proc init(arg:int) {
    t = int;
  }
}

var A:[1..2] int;
var B = new unmanaged C(A);

writeln(B);
