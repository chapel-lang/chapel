proc foo(i) where bar(2).type == real {
  writeln("hello world");
}

proc bar(i) where foo(2).type == real {
  writeln("hello again");
}

foo(2);
