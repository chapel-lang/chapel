module OuterModule {
  module N {
    var x = 1;
  }

  module M {
    use super.N;
    var y = 2;
  }

  use M;

  writeln((M.x, M.y));
}
