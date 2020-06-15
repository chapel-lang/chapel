proc setArgToFive(out arg: int) {
  arg = 5;
}
proc main() {
  const x:int;
  setArgToFive(x); // initializes x
  writeln(x);
}
