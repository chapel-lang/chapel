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
    use M;
    use N only;
    writeln(N.x);
  }
}
