module SubModule {
  import super.EXTRA_CHECKS;

  proc foo() { writeln("in SubModule.foo, EXTRA_CHECKS=", EXTRA_CHECKS); }
}
