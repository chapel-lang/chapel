config param testError = 0, testDisplayRepresentation = false;

proc testRangeAPI(lbl, r: range(?), idx, subr, offset=3, count=2) {
  writeln(lbl);
  writeln("------------");
  writeln(r);
  writeln("idxType          = ", r.idxType:string);
  writeln("stridable        = ", r.stridable);
  writeln("boundedType      = ", r.boundedType);
  writeln("isRangeType()    = ", isRangeType(r.type));
  writeln("isBoundedRange() = ", isBoundedRange(r));
  writeln("isBounded()      = ", r.isBounded());
  writeln("hasLowBound()    = ", r.hasLowBound());
  writeln("hasHighBound()   = ", r.hasHighBound());
  writeln("stride           = ", r.stride);
  writeln("alignment        = ", r.alignment);
  writeln("aligned          = ", r.aligned);
  writeln("first            = ", if r.hasFirst() then r.first:string else "undefined");
  writeln("last             = ", if r.hasLast() then r.last:string else "undefined");
  writeln("low              = ", if r.hasLowBound() then r.low else "undefined");
  writeln("high             = ", if r.hasHighBound() then r.high else "undefined");
  writeln("alignedLow       = ", if r.hasLowBound() then r.alignedLow else "undefined");
  writeln("alignedHigh      = ", if r.hasHighBound() then r.alignedHigh else "undefined");
  writeln("isEmpty()        = ", r.isEmpty());
  if (isBoundedRange(r)) {
    writeln("size             = ", r.size);
    writeln("sizeAs(uint)     = ", r.sizeAs(uint));
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
    writeln("orderToIndex(", offset, ")  = ", r.orderToIndex(offset));
  if (isBoundedRange(r)) {
    if !chpl__singleValIdxType(r.idxType) {
      writeln("expand(2)        = ", r.expand(2));

      if r.hasFirst() then writeln("offset(1)        = ", r.offset(1));
    }
  }
  if !chpl__singleValIdxType(r.idxType) {
    writeln("translate(2)     = ", r.translate(2));
    writeln("translate(-2)    = ", r.translate(-2));
  }
  if (isBoundedRange(r)) {
    if !chpl__singleValIdxType(r.idxType) {
      writeln("exterior(2)      = ", r.exterior(2));
      writeln("exterior(-2)     = ", r.exterior(-2));
    }
    writeln("interior(",count,")      = ", r.interior(count));
    writeln("interior(",-count,")     = ", r.interior(-count));
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
    writeln("r#",count,"               = ", r#count);
  if r.hasLast() then
    writeln("r#-",count,"              = ", r#-count);
  writeln("r == subr         = ", r == subr);
  writeln("r != subr         = ", r != subr);
  writeln("r[subr]           = ", r[subr]);
  writeln("r[subr] == subr[r]= ", r[subr] == subr[r]);
  writeln("r:string          = ", r:string);
  
  var r2 = r;
  writeln("Copying...        = ", r2);
  writeln();
}
