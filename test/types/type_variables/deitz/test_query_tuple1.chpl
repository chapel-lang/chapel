proc f(x: (?t, ?tt)) {
  var y: t;
  var z: tt;
  writeln(x);
  writeln((y,z));
}

f((3,3.14));
