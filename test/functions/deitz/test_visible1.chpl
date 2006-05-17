class C {
  var x : int;
}

var c = C();

fun foo() {
  fun bar(c : C) {
    writeln("in bar");
  }
  writeln("in foo");
}

bar(c);
