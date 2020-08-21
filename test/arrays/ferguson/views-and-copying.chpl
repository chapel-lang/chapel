proc returnIt(arg) { return arg; }

proc main() {
  var A: [1..2] real;
  var AA: [1..2, 1..2] real;

  {
    writeln();
    writeln("slice");
    const ref rB = A[1..1];
    writeln("to const ref is view:         ", rB.isSliceArrayView());

    var B = A[1..1];
    writeln("to var is view:               ", B.isSliceArrayView());
    
    const ref rReturned = returnIt(A[1..1]);
    writeln("returned is view:             ", rReturned.isSliceArrayView());

    const ref rD = A[1..1].domain;
    writeln(".domain to const ref is view: ", rD.isSliceDomainView());

    var D = A[1..1].domain;
    writeln(".domain to var is view:       ", D.isSliceDomainView());
  }

  {
    writeln();
    writeln("reindex");
    const ref rB = A.reindex(3..4);
    writeln("to const ref is view:         ", rB.isReindexArrayView());

    var B = A.reindex(3..4);
    writeln("to var is view:               ", B.isReindexArrayView());
    
    const ref rReturned = returnIt(A.reindex(3..4));
    writeln("returned is view:             ", rReturned.isReindexArrayView());

    const ref rD = A.reindex(3..4).domain;
    writeln(".domain to const ref is view: ", rD.isReindexDomainView());

    var D = A.reindex(3..4).domain;
    writeln(".domain to var is view:       ", D.isReindexDomainView());
  }

  {
    writeln();
    writeln("rank change");
    const ref rB = AA[1, ..];
    writeln("to const ref is view:         ", rB.isRankChangeArrayView());
    
    var B = AA[1, ..];
    writeln("to var is view:               ", B.isRankChangeArrayView());

    const ref rReturned = returnIt(AA[1, ..]);
    writeln("returned is view:             ", rReturned.isRankChangeArrayView());

    const ref rD = AA[1, ..].domain;
    writeln(".domain to const ref is view: ", rD.isRankChangeDomainView());

    var D = AA[1, ..].domain;
    writeln(".domain to var is view:       ", D.isRankChangeDomainView());
  }
}
