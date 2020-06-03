module OuterModule {
  module outermost {
    use OuterModule.useMe;
    module middlemost {
      module innermost {
        proc f() {
          writeln("Here's f!");
          // Should have access to symbols in module useMe
          writeln(a);
          g();
        }
      }
    }
  }

  module useMe {
    var a = 3 + 4i;
    proc g() {
      writeln("Here's g!");
    }
  }

  proc main() {
    use outermost.middlemost.innermost;
    f();
  }
}
