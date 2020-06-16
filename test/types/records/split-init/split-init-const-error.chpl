proc setArgToFive(out arg: int) {
  arg = 5;
}
proc main() {
  const x:int = 1;
  setArgToFive(x); // initializes x
  writeln(x);
}
