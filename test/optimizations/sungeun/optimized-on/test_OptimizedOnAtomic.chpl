
on Locales[numLocales-1] {
  var y:atomic int;

  on Locales[0] {

    on Locales[numLocales-1] {
      atomic_fetch_add_explicit_int_least64_t(y._v, 1, memoryOrder.seqCst);
    }

    var x = 2;

    on Locales[numLocales-1] {
      atomic_fetch_add_explicit_int_least64_t(y._v, x, memoryOrder.seqCst);
    }


    on Locales[numLocales-1] {
      atomic_fetch_sub_explicit_int_least64_t(y._v, 1, memoryOrder.seqCst);
    }

    writeln(y.read());
  }
}
