use notsupportedCommon;

proc test(dom) {
  writeln(dom.type:string);
  writeln(dom.low);
  writeln(dom.high);
  writeln(dom.stride);
  writeln(dom.alignment);
  writeln(dom.first);
  writeln(dom.last);
  writeln(dom.alignedLow);
  writeln(dom.alignedHigh);
  writeln();
}

test(dfltAssociativeBase);
