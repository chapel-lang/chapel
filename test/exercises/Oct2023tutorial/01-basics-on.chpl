if (numLocales < 2) {
  warning("This program assumes 2 or more locales, please run with -nl 2");
  exit();
}

writeln("Hello from locale ", here.id);

var A: [1..2, 1..2] real;

on Locales[1] {
  var B: [1..2, 1..2] real;

  B = 2 * A;
  writeln(B);
}

