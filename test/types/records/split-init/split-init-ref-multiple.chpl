config const setting = false;

proc main() {
  var oneInt = 1;
  var twoReal = 2.0;
  ref b;
  if setting {
    b = oneInt;
  } else {
    b = twoReal;
  }
  writeln(b);
}
