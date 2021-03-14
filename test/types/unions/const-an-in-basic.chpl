union Union {
  var s: string;
}

proc empty(u) { }

proc nop(in u) { }

proc test1() {
  var str = "string literal"*2;

  var aUnion: Union;
  aUnion.s = str;

  empty(aUnion);
  nop(aUnion);
}
test1();

proc test2() {
  var str = "string literal"*2;

  var aUnion: Union;
  aUnion.s = str;

  empty(aUnion);
  nop(aUnion);

  writeln(aUnion);
}
test2();
