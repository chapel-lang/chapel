extern {
  #define MYFUN(x) myfun_impl(x)
  static int myfun_impl(int arg) { return arg; }
}

writeln(MYFUN(1));

