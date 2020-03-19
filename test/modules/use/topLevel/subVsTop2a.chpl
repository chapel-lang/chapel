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
    use this.M;
    use this.N only;
    writeln(N.x);
  }
}
