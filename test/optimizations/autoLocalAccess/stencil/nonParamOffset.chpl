use StencilDist;

const n = 10;

const Dom = stencilDist.createDomain({0..<n}, fluff=(1,));
const DomInner = Dom.expand(-2);

var InArr: [Dom] int = 1;
var OutArr: [Dom] int;

InArr.updateFluff();


{
  writeln("Starting loop 1, expecting all local accesses (total 18)");
  // Const offsets are supported
  const offset = 1;
  forall i in DomInner {
    OutArr[i] =
      InArr[i-offset] +
      InArr[i+offset];
  }
  writeln(OutArr);
  writeln("End first loop");
}

{
  writeln("Starting loop 2, expecting all local accesses (total 18)");
  // So are params
  param offset = 1;
  forall i in DomInner {
    OutArr[i] =
      InArr[i-offset] +
      InArr[i+offset];
  }
  writeln(OutArr);
  writeln("End second loop");
}

{
  OutArr = 0;
  writeln("Starting loop 3, expecting local access for output (total 18) ",
                            "and default accesses for input (total 18)");
  // The optimization shouldn't fire here as offset is not constant
  forall i in DomInner {
    for offset in -1..1 {
      OutArr[i] +=
          InArr[i+offset];
    }
  }
  writeln(OutArr);
  writeln("End third loop");
}

{
  OutArr = 0;
  writeln("Starting loop 4, expecting local access for output (total 18) ",
                            "and default accesses for input (total 18)");
  // Technically, this could be supported. However, ALA fires before
  // normalization where param loops are still not lowered.
  forall i in DomInner {
    for param offset in -1..1 {
      OutArr[i] +=
      InArr[i+offset];
    }
  }
  writeln(OutArr);
  writeln("End fourth loop");
}
