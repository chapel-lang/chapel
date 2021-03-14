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
    use N only;
    use M;
    writeln(N.x);
  }
}
