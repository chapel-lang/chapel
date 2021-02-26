proc f(arg) {
  g(arg);
}

proc g(arg) {
  h(arg);
}

proc h(arg) {
  h(arg);
  a(arg);
  h(arg);
}

proc a(arg) {
  b(arg);
}

proc b(arg) {
  c(arg);
}

proc c(arg) {
  a(arg);
  badfn(arg);
  a(arg);
}

proc badfn(arg) {
  arg = 11;
}

f("hi");
