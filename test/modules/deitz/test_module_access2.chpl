module OuterModule {
  module N {
    var x = 1;
  }

  module M {
    use N;
    var y = 2;
  }

  use M;

  writeln((M.x, M.y));
}
