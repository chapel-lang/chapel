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

dfltRectangularSparseSub.bulkAdd([1,2]);
DistributedSparseSub.bulkAdd([1,2]);

test(dfltRectangularBase);
//test(dfltAssociativeBase);
//TODO implement BlockDom.first et al:
// test(DistributedBase);
test(dfltRectangularSubdomain);
//test(dfltAssociativeSubdomain);
//test(DistributedSubdomain);
test(dfltRectangularSparseSub);
//test(dfltAssociativeSparseSub);
test(DistributedSparseSub);

writeln("done");
