use Color;

proc testRange(r) {
  writeln("---");
  // copy
  var r2 = r;
  writeln(r2);
  writeln(r2.type:string);
  writeln();

  // serial loop
  for c in r do
    writeln(c);
  writeln();

  writeln("size = ", r.size);
  writeln("first = ", r.first);
  writeln("last = ", r.last);
  writeln("low = ", r.low);
  writeln("high = ", r.high);
  writeln("alignedLow = ", r.alignedLow);
  writeln("alignedHigh = ", r.alignedHigh);
  writeln("isEmpty = ", r.isEmpty());
  writeln("hasFirst = ", r.hasFirst());
  writeln("hasLast = ", r.hasLast());
  writeln("isNaturallyAligned = ", r.isNaturallyAligned());
  writeln("isAmbiguous = ", r.isAmbiguous());
  writeln("contains(blue) = ", r.contains(color.blue));
  writeln("contains(red) = ", r.contains(color.red));
  writeln("ident(color.red..color.violet) = ", ident(r, color.red..color.violet));
  writeln("ident(color.yellow..color.indigo) = ", ident(r, color.yellow..color.indigo));
  writeln("indexOrder(color.yellow) = ", r.indexOrder(color.yellow));
  writeln("indexOrder(color.green) = ", r.indexOrder(color.green));
  if (r.size > 1) then
    writeln("orderToIndex(1) = ", r.orderToIndex(1));
  if (r.last != color.violet) then
    writeln("r.translate(1) = ", r.translate(1));
  if (r.size > 1) {
    writeln("interior(1) = ", r.interior(1));
    writeln("interior(-1) = ", r.interior(-1));
  }
  if (r.size > 1 && r.last != color.violet) {
    writeln("r.exterior(1) = ", r.exterior(1));
    writeln("r.exterior(-1) = ", r.exterior(-1));
    writeln("r.expand(1) = ", r.expand(1));
    writeln("r.offset(1) = ", r.offset(1));
  }


  writeln();
}


proc main() {
  testRange(color.red..color.violet);
  for c in color.red..color.violet do
    writeln(c);
  writeln();

  testRange(color.yellow..color.indigo);
  for c in color.yellow..color.indigo do
    writeln(c);
  writeln();

  testRange(color.green..color.green);
  for c in color.green..color.green do
    writeln(c);
  writeln();

  testRange(color.blue..color.green);
  for c in color.blue..color.green do
    writeln(c);
  writeln();
}
