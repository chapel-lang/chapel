const edgeElems = 5;
const Elems = 0..#edgeElems;
const ElemSpace = {Elems, Elems, Elems};
use BlockDist;
const ElemDist = ElemSpace dmapped Block(ElemSpace);

var x8n: [ElemDist] 8*real;


// This one without the param works
proc foo(x8n:[ElemDist] 8*real) {
  var hourgam: 8*(4*real);
  var gamma: 4*(8*real) = (( 1.0,  1.0, -1.0, -1.0, -1.0, -1.0,  1.0,  1.0), 
                           ( 1.0, -1.0, -1.0,  1.0, -1.0,  1.0,  1.0, -1.0),
                           ( 1.0, -1.0,  1.0, -1.0,  1.0, -1.0,  1.0, -1.0),
                           (-1.0,  1.0, -1.0,  1.0,  1.0, -1.0,  1.0, -1.0));


  for eid in ElemDist {
    for /*param*/ i in 1..4 {
      var t = + reduce [j in 1..8] (x8n[eid][j] * gamma[i][j]);
      for param j in 1..8 {
        hourgam[j][i] = gamma[i][j];
      }
    }
  }

}

foo(x8n);

proc foo_param (x8n:[ElemDist] 8*real) {
  var hourgam: 8*(4*real);
  var gamma: 4*(8*real) = (( 1.0,  1.0, -1.0, -1.0, -1.0, -1.0,  1.0,  1.0), 
                           ( 1.0, -1.0, -1.0,  1.0, -1.0,  1.0,  1.0, -1.0),
                           ( 1.0, -1.0,  1.0, -1.0,  1.0, -1.0,  1.0, -1.0),
                           (-1.0,  1.0, -1.0,  1.0,  1.0, -1.0,  1.0, -1.0));


  for eid in ElemDist {
    for param i in 1..4 {
      var t = + reduce [j in 1..8] (x8n[eid][j] * gamma[i][j]);
      for param j in 1..8 {
        hourgam[j][i] = gamma[i][j];
      }
    }
  }

}


foo_param(x8n);
