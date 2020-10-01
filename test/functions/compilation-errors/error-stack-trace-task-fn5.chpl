

proc baz(arg) {
  arg = 1;
}

proc bar(arg) {
  coforall i in 1..1 {
    baz(arg);
  }
}

proc foo(arg) {
  bar(arg);
}

foo("hi");
