proc f(out arg: [1..2] int) {
  var A:[1..1] int;
  arg = A;
}

proc main() {
  var AA:[1..1] int;
  f(AA);
}
