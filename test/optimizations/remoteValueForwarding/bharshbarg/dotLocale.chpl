
config const n = 10;

proc helper(const ref arg) {
  writeln(arg);
  writeln(arg.locale);
}

proc main() {
  var x = if n > 10 then 5 else 1;

  // We could RVF 'x' if not for the use of ".locale" in the on-statement
  on Locales[numLocales-1] {
    writeln(x);
    writeln(x.locale);
  }

  var y = if n < 10 then 5 else 1;

  on Locales[numLocales-1] {
    helper(y);
  }
}
