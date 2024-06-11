module linkToPrev {
  record foo {
    proc func1() { }

    // Seems to only be a problem for record's referring to functions at the
    // same level and when they are both in another type
    /* Returned by :proc:`func1` */
    record innerFoo { }
  }
}
