record foo {
  var dom : domain(1) = {1..10};
  var data : [dom] int;
}

proc bar(in x) {
  writeln("In bar()...");
  writeln("We're running on locale ", here.id);
  writeln("x is on locale ", x.locale.id);
  writeln("x.data[1] is on locale ", x.data[1].locale.id);
  writeln();
}

proc main() {
  var a : foo;

  on Locales[numLocales-1] {
    writeln("Before call to bar()...");
    writeln("a is on locale: ", a.locale.id);
    writeln("a.data[1] is on locale: ", a.data[1].locale.id);
    writeln();
    bar(a);
  }
}
