function foo(a) {
  writeln("in foo");
  if (a == 0) then {
    bar(1)
  } else {
    writeln("done with foo");
  }
}

function bar(a) {
  writeln("in bar");
  if (a == 0) then {
    foo(1)
  } else {
    writeln("done with bar");
  }
}

foo(0);
bar(0);

