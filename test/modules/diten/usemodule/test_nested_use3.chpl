module OuterModule {
  module outermost {
    module middlemost {
      module innermost {
        proc f() {
          writeln("hello from innermost");
        }
      }
    }
  }

  proc main() {
    use outermost.notmiddlemost.innermost;
  }
}
