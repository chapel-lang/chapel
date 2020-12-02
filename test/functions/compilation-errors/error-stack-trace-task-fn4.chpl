

proc baz(arg) {
  arg = 1;
}

proc bar(arg) {
  cobegin {
    {baz(arg);}
    {}
  }
}
proc foo(arg) {
  bar(arg);
}

foo("hi");
