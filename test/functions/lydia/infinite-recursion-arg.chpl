proc infinite1(x: int, y = infinite2(x)) {
  return y;
}

proc infinite2(x: int) {
  return infinite1(x);
}

writeln(infinite1(2));
