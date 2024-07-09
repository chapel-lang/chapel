use BlockDist;

proc blockDist.printBB() {
  // The use of 'this.' in the following is a workaround for #22656/#5979
  writeln("boundingBox = ", this.boundingBox);
}

proc DefaultAssociativeDom.printTableSize() {
  writeln("tableSize = ", table.tableSize);
}

proc main() {
  var DR = {1..20};
  var BD = DR dmapped new blockDist(DR);
  writeln(BD.distribution.type:string, ".printBB()");
  BD.distribution.printBB();
  writeln();

  var DA = {1, 3, 5, 6, 7, 42};
  writeln(DA.type:string, ".printTableSize()");
  DA.printTableSize();
  writeln();
}
