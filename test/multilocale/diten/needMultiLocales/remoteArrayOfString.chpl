proc main {
  var a: [1..3] string;
  a(1) = "one";
  a(2) = "two";
  a(3) = "three";
  on Locales(1) {
    var tmp: string;
    tmp = a(1);
    a(1) = a(3);
    a(3) = tmp;
  }
  writeln(a);
}
