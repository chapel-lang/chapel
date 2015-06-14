extern {
  #define MYFUN myfun_impl
  static int myfun_impl(void) { return 20; }
}

writeln(MYFUN());

