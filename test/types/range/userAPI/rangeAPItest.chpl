config param testError = 0, testDisplayRepresentation = false;

proc testRangeAPI(lbl, r: range(?), idx, subr) {
  writeln(lbl);
  writeln("------------");
  writeln(r);
  writeln("idxType          = ", r.idxType:string);
  writeln("stridable        = ", r.stridable);
  writeln("boundedType      = ", r.boundedType);
  writeln("isRangeType()    = ", isRangeType(r.type));
  writeln("isBoundedRange() = ", isBoundedRange(r));
  writeln("hasLowBound()    = ", r.hasLowBound());
  writeln("hasHighBound()   = ", r.hasHighBound());
  writeln("stride           = ", r.stride);
  writeln("alignment        = ", r.alignment);
  writeln("aligned          = ", r.aligned);
  writeln("first            = ", r.first);
  writeln("last             = ", r.last);
  writeln("low              = ", r.low);
  writeln("high             = ", r.high);
  writeln("alignedLow       = ", r.alignedLow);
  writeln("alignedHigh      = ", r.alignedHigh);
  writeln("isEmpty()        = ", r.isEmpty());
  if (isBoundedRange(r)) {
    writeln("size             = ", r.size);

  }
  writeln("hasFirst()       = ", r.hasFirst());
  writeln("hasLast()        = ", r.hasLast());
  writeln("isNat.Algned()   = ", r.isNaturallyAligned());
  writeln("isAmbiguous      = ", r.isAmbiguous());
  writeln("contains(", idx, ") = ", r.contains(idx));
  writeln("contains(", subr, ") = ", r.contains(subr));
  writeln("ident(self)      = ", ident(r, r));
  writeln("ident(other)     = ", ident(r, subr));
  writeln("boundsCheck(", idx, ") = ", r.boundsCheck(idx));
  writeln("boundsCheck(", subr, ") = ", r.boundsCheck(subr));
  writeln("indexOrder(", idx, ") = ", r.indexOrder(idx));
  if (r.hasFirst()) then
    writeln("orderToIndex(3)  = ", r.orderToIndex(3));
  if (isBoundedRange(r)) {
    writeln("expand(2)        = ", r.expand(2));
    writeln("offset(1)        = ", r.offset(1));
  }
  writeln("translate(2)     = ", r.translate(2));
  writeln("translate(-2)    = ", r.translate(-2));
  if (isBoundedRange(r)) {
    writeln("exterior(2)      = ", r.exterior(2));
    writeln("exterior(-2)     = ", r.exterior(-2));
    writeln("interior(2)      = ", r.interior(2));
    writeln("interior(-2)     = ", r.interior(-2));
  }
          
  writeln("serial iteration = ");
  if (isBoundedRange(r)) {
    for i in r do
      write(i, " ");
  } else {
    if r.hasFirst() {
      var count = 1;
      for i in r {
        count += 1;
        if (count == 5) then
          break;
        writeln(i, " ");
      }
      writeln("...");
    } else {
      writeln("<can't be done>");
    }
  }
        
  writeln();

  if r.hasFirst() then
    writeln("r#2               = ", r#2);
  if r.hasLast() then
    writeln("r#-2              = ", r#-2);
  writeln("r == subr         = ", r == subr);
  writeln("r != subr         = ", r != subr);
  writeln("r[subr]           = ", r[subr]);
  writeln("r[subr] == subr[r]= ", r[subr] == subr[r]);
  
  var r2 = r;
  writeln("Copying...        = ", r2);
  writeln();
}
