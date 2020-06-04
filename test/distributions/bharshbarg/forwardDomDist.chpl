use BlockDist;

proc Block.printBB() {
  writeln("boundingBox = ", boundingBox);
}

proc DefaultAssociativeDom.printTableSize() {
  writeln("tableSize = ", table.tableSize);
}

proc main() {
  var DR = {1..20};
  var BD = DR dmapped Block(DR);
  writeln(BD.dist.type:string, ".printBB()");
  BD.dist.printBB();
  writeln();

  var DA = {1, 3, 5, 6, 7, 42};
  writeln(DA.type:string, ".printTableSize()");
  DA.printTableSize();
  writeln();
}
