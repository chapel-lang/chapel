// Motivated by:
//  optimizations/bulkcomm/asenjo/stencilDR/v1/stencil.chpl
//  optimizations/bulkcomm/asenjo/ptransDR/v1/ptrans.chpl
//  optimizations/bulkcomm/asenjo/ptransDR/v2/ptrans.chpl

record localInfo {
  const i = 5;
}

var Data: [1..3] localInfo;
writeln(Data);

var Mata = [ new localInfo(6), new localInfo(7) ];
writeln(Mata);

// Please read .future when dealing with this test.
