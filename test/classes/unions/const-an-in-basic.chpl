union Union {
  var s: string;
}

proc empty(u) { }

proc nop(in u) { }

proc main() {
  var str = "string literal"*2;

  var aUnion: Union;
  aUnion.s = str;

  empty(aUnion);
  nop(aUnion);
}
