proc dothis(i:int) : int;
proc dothis(i:real) : real;

proc main() {
  writeln(dothis(100));
  writeln(dothis(100.0));
}
