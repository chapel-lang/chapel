config const start = 1;
config const end = 10;

on here.getChild(1) {
  var a, b: [start..end] int;
  var value = 20;

  // one array
  forall  elem in a { elem += 10;          } writeln(a);
  forall  elem in a { elem += value;       } writeln(a);
  foreach elem in a { elem += 10;          } writeln(a);
  foreach elem in a { elem += value;       } writeln(a);
}
