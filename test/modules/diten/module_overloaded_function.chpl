module OuterModule {
  module color {
    var r, g, b: int;

    proc do_something() {
      r = 1; g = 2; b = 3;
    }
    proc do_something(i: int){
      r = i; g = i; b = i;
    }
  }

  proc main() {
    use color;

    g = 1;

    writeln(color.r, " ", color.g, " ", color.b);
    do_something();
    writeln(color.r, " ", color.g, " ", color.b);
    do_something(3);
    writeln(color.r, " ", color.g, " ", color.b);
  }
}
