module OuterModule {
  var x = 1;

  module M {
    var y = 2;
  }

  use M;

  writeln((M.x, M.y));
}
