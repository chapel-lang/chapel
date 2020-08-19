proc h(arg) {
  arg = 11;
}

proc g(arg) {
  h(arg);
}

proc f(arg:string) {
  g(arg);
}

f("hi");
