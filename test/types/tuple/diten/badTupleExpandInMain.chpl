proc main() {
  var x3: 5*int = (1, 2, 3, 4, 5);
  var y3: 5*int = (...x3); // This type is wrong.
  writeln(y3.type:string);
  writeln(y3);
}
