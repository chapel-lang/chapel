module DoubleNested {
  module DoubleNested {
    include module SubModule;
  }
  proc main() {
    import DoubleNested.DoubleNested.SubModule as T;
    writeln(T.subVar);
    writeln(T.subFn());
  }
}
