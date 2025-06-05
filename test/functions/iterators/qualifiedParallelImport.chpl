
module A {
  iter foo(): int { yield 1; }
  iter foo(param tag): int where tag == iterKind.standalone { yield 1; }
}
module B {
  proc main() {
    import A;
    forall i in A.foo() { writeln(i); }
  }
}
