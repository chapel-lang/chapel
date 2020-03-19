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
    use this.M only N as N2;
    use this.N only;
    writeln(N.x);
    writeln(N2.x);
  }
}
