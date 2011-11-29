proc foo(a) {
  writeln("in foo");
  if (a == 0) {
    bar(1);
  } else {
    writeln("done with foo");
  }
}

proc bar(a) {
  writeln("in bar");
  if (a == 0) {
    foo(1);
  } else {
    writeln("done with bar");
  }
}

foo(0);
bar(0);

