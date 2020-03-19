module OuterModule {
  var x = 1;

  module M {
    var y = 2;
  }

  use this.M;

  writeln((M.x, M.y));
}
