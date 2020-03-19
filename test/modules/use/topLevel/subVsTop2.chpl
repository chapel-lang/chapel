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
    use this.N only;
    use this.M;
    writeln(N.x);
  }
}
