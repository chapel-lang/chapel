config const setting = false;

proc main() {
  type t;

  if setting {
    t = int;
  } else {
    t = real;
  }
  writeln(t:string);
}
