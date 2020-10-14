proc returnIt(arg) { return arg; }

proc test1() {
  writeln();
  writeln();
  writeln("### test1");

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
test1();

proc test2()
{
  writeln();
  writeln();
  writeln("### test2");

  var A: [1..2] real;
  var AA: [1..2, 1..2] real;

  {
    writeln();
    writeln("reindex");
    var B = A.reindex(3..4);
    writeln("to var is view:               ", B.isReindexArrayView());

    const ref rD = B.domain;
    writeln("var.domain is view:           ", rD.isReindexDomainView());
  }

  {
    writeln();
    writeln("rank change");
    var B = AA[1, ..];
    writeln("to var is view:               ", B.isRankChangeArrayView());

    const ref rD = B.domain;
    writeln("var.domain is view:           ", rD.isRankChangeDomainView());
  }
}
test2();

proc test3() {
  writeln();
  writeln();
  writeln("### test3");

  var A: [1..2] real;
  var AA: [1..2, 1..2] real;

  {
    writeln();
    writeln("slice");
    A = 0;
    const ref rB = A[1..1];
    A = 1;
    writeln("to const ref behaves view:    ", A[1]);

    A = 0;
    var B = A[1..1];
    A = 1;
    writeln("to var behaves view:          ", B[1]);

    A = 0;
    const ref rReturned = returnIt(A[1..1]);
    A = 1;
    writeln("returned behaves view:        ", rReturned[1]);
  }

  {
    writeln();
    writeln("reindex");
    A = 0;
    const ref rB = A.reindex(3..4);
    A = 1;
    writeln("to const ref behaves view:    ", rB[3]);

    A = 0;
    var B = A.reindex(3..4);
    A = 1;
    writeln("to var behaves view:          ", B[3]);

    A = 0;
    const ref rReturned = returnIt(A.reindex(3..4));
    A = 1;
    writeln("returned behaves view:        ", rReturned[3]);
  }

  {
    writeln();
    writeln("rank change");
    AA = 0;
    const ref rB = AA[1, ..];
    AA = 1;
    writeln("to const ref behaves view:    ", rB[1]);

    AA = 0;
    var B = AA[1, ..];
    AA = 1;
    writeln("to var behaves view:          ", B[1]);

    AA = 0;
    const ref rReturned = returnIt(AA[1, ..]);
    AA = 1;
    writeln("returned behaves view:        ", rReturned[1]);
  }
}
test3();
