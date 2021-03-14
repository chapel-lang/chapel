// This differs from DRimpl.chpl :
//
//  * handle partial reductions over forall-expressions, promoted
//    expressions, and arrays uniformly,
//
//  * drop checking of correct usage for simplicity.
//
// Includes minimal testing, cf. DRtest.chpl.
//
// Variations 1 and 2 differ only in resultDom - columns vs. rows.

use utilities;
writeln();

//---------------------------------

var D = { 1..2, 1..3 };

var A = [d in D] 100**d(0);

writeln("input #1");
writeln(A.domain);
writeln(A);
writeln();

//---------------------------------

var ARR1 = PR(
  perElemOp = new borrowed SumReduceScanOp(eltType=int),
  resultDom = { 1..1, 1..3 },
  sourceArr = A
);

writeln("array - preserving columns");
writeln(ARR1.domain);
writeln(ARR1);
writeln();

var ARR2 = PR(
  perElemOp = new borrowed SumReduceScanOp(eltType=int),
  resultDom = { 1..2, 1..1 },
  sourceArr = A
);

writeln("array - preserving rows");
writeln(ARR2.domain);
writeln(ARR2);
writeln();

//---------------------------------

var FE1 = PR(
  perElemOp = new borrowed SumReduceScanOp(eltType=int),
  resultDom = { 1..1, 1..3 },
  sourceExp = [d in D] 100**d(0)
);

writeln("forall expression - preserving columns");
writeln(FE1.domain);
writeln(FE1);
writeln();

var FE2 = PR(
  perElemOp = new borrowed SumReduceScanOp(eltType=int),
  resultDom = { 1..2, 1..1 },
  sourceExp = [d in D] 100**d(0)
);

writeln("forall expression - preserving rows");
writeln(FE2.domain);
writeln(FE2);
writeln();

//---------------------------------

var B = [d in D] d(1);
var C = A + B;

writeln("input #2");
writeln(C.domain);
writeln(C);
writeln();

var PE1 = PR(
  perElemOp = new borrowed SumReduceScanOp(eltType=int),
  resultDom = { 1..1, 1..3 },
  sourceExp = A + B
);

writeln("promoted expression - preserving columns");
writeln(PE1.domain);
writeln(PE1);
writeln();

var PE2 = PR(
  perElemOp = new borrowed SumReduceScanOp(eltType=int),
  resultDom = { 1..2, 1..1 },
  sourceExp = A + B
);

writeln("promoted expression - preserving rows");
writeln(PE2.domain);
writeln(PE2);
writeln();

//---------------------------------

proc PR(const perElemOp, const resultDom, const sourceArr : []) {
  const ref sourceDom = sourceArr.domain;
  return dsiPartialReduce(perElemOp, resultDom, sourceArr, sourceDom);
}

proc PR(const perElemOp, const resultDom, const sourceExp : _iteratorRecord) {
  if ! chpl_iteratorHasDomainShape(sourceExp) then
   compilerError("cannot compute a partial reduction over an expression without a domain shape");
  if chpl_iteratorFromForExpr(sourceExp) then
   compilerError("cannot compute a partial reduction over a  for-expression");

  var sourceDom = new _domain(sourceExp._shape_);
  sourceDom._unowned = true;
  return dsiPartialReduce(perElemOp, resultDom, sourceExp, sourceDom);
}

proc dsiPartialReduce(const perElemOp, const resultDom,
                      const sourceThing, const sourceDom)  throws
{
  var   resultArr: [resultDom] int = perElemOp.identity;

  // todo: switch to 'new owned'
  const resultReduceOp = new unmanaged PartRedOp(eltType=resultArr.type,
                                                 perElemOp = perElemOp);

  forall (sourceIdx, sourceElm) in zip(sourceDom, sourceThing)
    with (resultReduceOp reduce resultArr)
  {
    const resultIdx = fullIdxToReducedIdx(resultDom.dims(),
                                          sourceDom.dims(),
                                          sourceIdx);

    resultArr reduce= (resultIdx, sourceElm);
  }

  delete resultReduceOp;
  return  resultArr;
}
