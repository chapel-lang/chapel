class C {
  proc init(arg:int) {
  }
}

var A:[1..2] int;
var B = new borrowed C(A);

writeln(B);
