use GPU;

config const n = 10;

coforall l in Locales do on l.gpus[0] {
  var A: [1..n] int;
  foreach a in A {
    assertOnGpu();
    a += a.locale.id;
    // a *= here.id;  // still doesn't work, locale code requires virtual method
                      // call, which we can't do on GPU. Should we do something
                      // special for "here.id"
  }
  writeln(A);
}
