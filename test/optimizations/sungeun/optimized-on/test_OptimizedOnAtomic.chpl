
on Locales[numLocales-1] {
  var y:atomic int;

  on Locales[0] {

    on Locales[numLocales-1] {
      atomic_fetch_add_explicit_int_least64_t(y._v, 1, memory_order_seq_cst);
    }

    var x = 2;

    on Locales[numLocales-1] {
      atomic_fetch_add_explicit_int_least64_t(y._v, x, memory_order_seq_cst);
    }


    on Locales[numLocales-1] {
      atomic_fetch_sub_explicit_int_least64_t(y._v, 1, memory_order_seq_cst);
    }

    writeln(y.read());
  }
}
