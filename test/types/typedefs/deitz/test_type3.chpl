proc main() {
  type a = b;
  type b = int;
  var c: a;
  writeln(c);
  type x = int;
  type y = x;
  var z: y;
  writeln(z);
}
