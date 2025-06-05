use Time;

use CompressedSparseLayout;

var t = new stopwatch();

config const correctness = true;

config const numIndices = 100;
config const density = 0.01;

const numIndicesInParentDom = numIndices/density;
const parentSize = sqrt(numIndicesInParentDom):int;
const parentDom = {1..parentSize, 1..parentSize};
const sparseStride = (1/sqrt(density)):int;
const sparseIndices = {1..parentSize by sparseStride,
                       1..parentSize by sparseStride};

proc startDiag() {
  if !correctness {
    t.start();
  }
}

proc stopDiag(key, dom) {
  if !correctness {
    t.stop();
    writeln(key, ": ", t.elapsed());
    t.clear();
  }
  assert(dom.size == sparseIndices.size);
}

{
  var cooDomBase: sparse subdomain(parentDom);
  cooDomBase += sparseIndices;
  var cooDom: sparse subdomain(parentDom);
  startDiag();
  cooDom = cooDomBase;
  stopDiag("COO to COO", cooDom);
}

{
  var cooDomBase: sparse subdomain(parentDom);
  cooDomBase += sparseIndices;
  var csrDom: sparse subdomain(parentDom) dmapped new csrLayout();
  startDiag();
  csrDom = cooDomBase;
  stopDiag("COO to CSR", csrDom);
}

// assignment from csr
{
  var csrDomBase: sparse subdomain(parentDom) dmapped new csrLayout();
  csrDomBase += sparseIndices;
  var cooDom: sparse subdomain(parentDom);
  startDiag();
  cooDom = csrDomBase;
  stopDiag("CSR to COO", cooDom);
}

{
  var csrDomBase: sparse subdomain(parentDom) dmapped new csrLayout();
  csrDomBase += sparseIndices;
  var csrDom: sparse subdomain(parentDom) dmapped new csrLayout();
  startDiag();
  csrDom = csrDomBase;
  stopDiag("CSR to CSR", csrDom);
}

{
  var cscDomBase: sparse subdomain(parentDom) dmapped new cscLayout();
  cscDomBase += sparseIndices;
  var cscDom: sparse subdomain(parentDom) dmapped new cscLayout();
  startDiag();
  cscDom = cscDomBase;
  stopDiag("CSC to CSC", cscDom);
}
