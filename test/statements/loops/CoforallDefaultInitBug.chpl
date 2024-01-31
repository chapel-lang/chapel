class MyClass {
  proc func() {
    return 2 + 2;
  }
}

// Split-init error should fire at this point. It would be nice if the error
// would mention a note about the use within the `coforall`, but that isn't
// necessary - and would require modification to map `m` to temporaries
// introduced for the `coforall` block function.
var m: MyClass;

coforall idx in 0..3 {
  m.func();
}
