proc main() {
  on Locales[1] {
    var A:[1..10] int;
    writeln(A.targetLocales());
    on Locales[2] {
      writeln(A.targetLocales());
    }
  }
}
