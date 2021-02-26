proc h(arg) {
  h(arg);
  arg = 11;
  h(arg);
}

proc g(arg) {
  h(arg);
}

proc f(arg:string) {
  g(arg);
}

f("hi");
