use utilities;
use dsiMethods;

proc partialReduce(arr, param onlyDim) {

  if onlyDim < 1 || onlyDim > arr.dsiGetBaseDom().rank then
    compilerError("Invalid partial reduction dimension: ", onlyDim);

  if arr.dsiGetBaseDom().rank == 1 then
    return + reduce arr;

  const PartialDom =
    arr.dsiGetBaseDom().dsiPartialDomain(exceptDim=onlyDim);

  var ResultArr: [PartialDom] arr.eltType;
  forall partialIdx in PartialDom {
    ResultArr[partialIdx] = + reduce arr.dsiPartialThese(onlyDim, 
        if isTuple(partialIdx) then partialIdx else (partialIdx, ));
  }
  return ResultArr;
}

proc partialReduceToTarget(arr, param onlyDim, target) {

  if onlyDim < 1 || onlyDim > arr.dsiGetBaseDom().rank then
    compilerError("Invalid partial reduction dimension: ", onlyDim);

  if arr.dsiGetBaseDom().rank == 1 then
    return + reduce arr;

  const PartialDom =
    arr.dsiGetBaseDom().dsiPartialDomain(exceptDim=onlyDim);

  forall partialIdx in PartialDom {
    target[partialIdx] = + reduce arr.dsiPartialThese(onlyDim, 
        if isTuple(partialIdx) then partialIdx else (partialIdx, ));
  }
}

proc bulkPartialReduce(arr, param onlyDim) {

  const dom = arr.dom;
  const dist = dom.dist;

  const PartialDom = dom.dsiPartialDomain(exceptDim=onlyDim);
  var ResultArr: [PartialDom] arr.eltType;

  var locResDom = dist.targetLocDom dmapped _getDistribution(dist);
  var locRes: [locResDom] ResultArr._value.myLocArr!.myElems.type;

  coforall l2 in
      dist.targetLocDom._value.dsiPartialDomain(exceptDim=onlyDim) {

    on ResultArr._value.locArr[l2].myElems {
      ref partialResult = ResultArr._value.locArr[l2].myElems;
      // FIXME should be a coforall - doesn't work
      forall l1 in dist.targetLocDom.dim(onlyDim)
          with (+ reduce partialResult) {

        const l = chpl__tuplify(l2).withIdx(onlyDim, l1);
        on dom.locDoms[l] {
          var __target = ResultArr._value.locArr[l2].clone();
          partialReduceToTarget(arr.locArr[l]!, onlyDim, __target);
          partialResult += __target.myElems;
          delete __target;
        }
      }
    }
  }
  return ResultArr;
}
