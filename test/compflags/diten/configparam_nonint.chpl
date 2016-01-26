config param areal: real = 3.14;
config param ac_string: c_string = "hello";

proc main() {
  writeln(areal);
  writeln(ac_string:string);
}
