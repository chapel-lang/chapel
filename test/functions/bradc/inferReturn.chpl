proc returnInt() {
  return 2;
}


proc returnFloat() {
  return 2.3;
}


proc returnString() {
  return "hi there";
}


proc main() {
  writeln("returnInt() returns: ", returnInt());
  writeln("returnFloat() returns: ", returnFloat());
  writeln("returnString() returns: ", returnString());
}
