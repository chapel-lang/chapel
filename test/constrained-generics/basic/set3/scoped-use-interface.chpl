module Other {
  class CC { }
  interface II { }
}

proc foo() {
  use Other;

  record R: II {}
  class C: CC {}
}
foo();
