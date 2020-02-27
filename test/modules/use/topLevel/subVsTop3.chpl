module OuterModule {
  module M {
    module N {
      var x = 100;
    }
  }

  module N {
    var x = 42;
  }

  {
    use M only N as N2;
    use N only;
    writeln(N.x);
    writeln(N2.x);
  }
}
