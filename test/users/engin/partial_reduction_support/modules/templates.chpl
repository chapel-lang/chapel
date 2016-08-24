use utilities;
use dsiMethods;

iter _array.partialThese(param onlyDim, otherIdx) {
  for i in this.domain._value.dsiPartialThese(onlyDim, otherIdx) {
    /*writeln("Serial Accessing ", i);*/
    yield this._value.dsiAccess(otherIdx.merge(onlyDim, i));
  }
}

iter _array.partialThese(param onlyDim, otherIdx, param tag) where
tag==iterKind.leader {

  for i in this.domain._value.dsiPartialThese(onlyDim, otherIdx, tag=tag) {
    yield i;
  }
}

iter _array.partialThese(param onlyDim, otherIdx, param tag, followThis) where
tag==iterKind.follower {

  for i in this.domain._value.dsiPartialThese(onlyDim, otherIdx, 
      tag=tag, followThis=followThis) {
    writeln("Follower Accessing ", i);
    yield this._value.dsiAccess(otherIdx.merge(onlyDim, i));
  }
}

iter _array.partialThese(param onlyDim, otherIdx, param tag) where
tag==iterKind.standalone {

  for i in this.domain._value.dsiPartialThese(onlyDim, otherIdx, tag=tag) {
    /*writeln("Standalone Accessing ", i);*/
    yield this._value.dsiAccess(otherIdx.merge(onlyDim, i));
  }
}


proc dsiPartialReduce_template(arr: [], param onlyDim) {
  return dsiPartialReduce_template(arr._value, onlyDim);
}

proc dsiPartialReduce_template(arr, param onlyDim) {

  if onlyDim < 1 || onlyDim > arr.dsiGetBaseDom().rank then
    halt("Invalid partial reduction dimension: ", onlyDim);

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

proc dsiPartialReduce_template(arr, param onlyDim, result) {

  if onlyDim < 1 || onlyDim > arr.dsiGetBaseDom().rank then
    halt("Invalid partial reduction dimension: ", onlyDim);

  if arr.dsiGetBaseDom().rank == 1 then
    return + reduce arr;

  const PartialDom =
    arr.dsiGetBaseDom().dsiPartialDomain(exceptDim=onlyDim);

  forall partialIdx in PartialDom {
    /*writeln(partialIdx);*/
    result[partialIdx] = + reduce arr.dsiPartialThese(onlyDim, 
        if isTuple(partialIdx) then partialIdx else (partialIdx, ));
  }
}
