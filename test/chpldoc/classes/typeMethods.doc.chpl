class Foo {
  /* A type method declared inside the class Foo */
  proc type biff() {

  }
}

/* A type method declared outside the class Foo */
proc type Foo.blah() {

}

record Bar {
  /* A type method declared inside the record Bar */
  proc type biff() {

  }
}

/* A type method declared outside the record Bar */
proc type Bar.blah() {

}
