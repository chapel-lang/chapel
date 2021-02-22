proc f(out arg: [] int) {
  var A:[1..1] real;
  arg = A;
}

proc main() {
  var AA:[1..1] int;
  f(AA);
}
