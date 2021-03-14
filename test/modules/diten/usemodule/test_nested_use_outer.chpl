module OuterModule {
  module outermost {
    module middlemost {
      module innermost {
        proc f(a: int) {
          import OuterModule.outermost.c;
          import OuterModule.outermost.middlemost.b;
          writeln("innermost: a is ", a);
          writeln("innermost: b is ", b);
          writeln("innermost: c is ", c);
        }
      }
      var b = "b";
      proc f(a: int) {
        //        import super.c;
        import OuterModule.outermost.c;
        writeln("middlemost: a is ", a);
        writeln("middlemost: b is ", b);
        writeln("middlemost: c is ", c);
      }
    }
    var c = "c";
    proc f(a: int) {
      writeln("outermost: a is ", a);
      writeln("outermost: c is ", c);
    }
  }

  proc main() {
    use outermost.middlemost.innermost;
    f(3);
    foo(4);
  }

  proc foo(a: int) {
    use outermost;
    writeln("foo");
    f(a);
  }
}
