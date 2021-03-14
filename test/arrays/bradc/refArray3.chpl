proc main {
  var A: [1..3] real;
  const C: [1..3] real;

  const ref AR = A;
  const ref CR = C;

  AR[2] = 2.3;
  CR[2] = 3.4;

  writeln(AR);
  writeln(CR);
}
