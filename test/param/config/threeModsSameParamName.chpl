module OuterModule {
  module M1 {
    config param flag = false;
  }

  module M2 {
    config param flag = false;
  }

  module M3 {
    config param flag = false;
  }

  writeln((M1.flag, M2.flag, M3.flag));
}
