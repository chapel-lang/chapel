proc mult(param size) {
  const op = "*";
  var a = (1.0 + 1.0i): complex(size), b = (2.0 + 2.0i): complex(size);
  var c = a * b;

  writeln(size, " bit: (", a, ") ", op, " (", b, ") = ", c);
}

proc div(param size) {
  const op = "/";
  var a = (1.0 + 1.0i): complex(size), b = (2.0 + 2.0i): complex(size);
  var c = a / b;
  writeln(size, " bit: (", a, ") ", op, " (", b, ") = ", c);
}

proc add(param size) {
  const op = "+";
  var a = (1.0 + 1.0i): complex(size), b = (2.0 + 2.0i): complex(size);
  var c = a + b;
  writeln(size, " bit: (", a, ") ", op, " (", b, ") = ", c);
}

proc sub(param size) {
  const op = "-";
  var a = (1.0 + 1.0i): complex(size), b = (2.0 + 2.0i): complex(size);
  var c = a - b;
  writeln(size, " bit: (", a, ") ", op, " (", b, ") = ", c);
}

proc uplus(param size) {
  const op = "+";
  var a = (1.0 + 1.0i):complex(size);
  var b = +a;
  writeln(size, " bit: ", op, "(", a, ") = ", b);
}

proc uminus(param size) {
  const op = "-";
  var a = (1.0 + 1.0i):complex(size);
  var b = -a;
  writeln(size, " bit: ", op, "(", a, ") = ", b);
}

proc main {
  mult(128);
  mult(64);
  div(128);
  div(64);
  add(128);
  add(64);
  sub(128);
  sub(64);
  uplus(128);
  uplus(64);
  uminus(128);
  uminus(64);
}
