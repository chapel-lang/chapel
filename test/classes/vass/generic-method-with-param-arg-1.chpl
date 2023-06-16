override proc DefaultRectangularArr.getAllocationDomainForDefaultRectangularArray3(): 3 * range(int(64), strides=strideKind.any)
{
  param rank = 3;
  if this.dom.dsiNumIndices == 1000000 {
    return (this:BaseArr).getAllocationDomainForDefaultRectangularArray3();
  } else {
    var p:rank * range(int(64), strides=strideKind.any);
    for i in 0..#rank do
      p(i)=dom.ranges(i);
    return p;
  }
}

proc BaseArr.getAllocationDomainForDefaultRectangularArray3(): 3 * range(int(64), strides=strideKind.any)
{
   halt("getAllocationDomainForDefaultRectangularArray:non-DefaultRectangular array encountered");
}

var A: [1..1, 1..1, 1..1] int;
writeln(A.getAllocationDomainForDefaultRectangularArray3());
