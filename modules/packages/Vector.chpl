/*
 * Obtained from https://github.com/pnnl/chgl/blob/master/src/Vectors.chpl
 */

use CyclicDist;
use BlockDist;
use Sort;
use Utilities;

config param VectorGrowthRate : real = 1.5;

class Vector {
  type eltType;
  const growthRate : real;
  var dom = {0..-1};
  var arr : [dom] eltType;
  var sz : int;
  var cap : int;
  
  proc init(type eltType, initialSize : integral = 0, growthRate = VectorGrowthRate) {
    this.eltType = eltType;
    this.growthRate = growthRate;
    // Right now 0..#initialSize is bugged if initialSize is 0, it becomes 1..0
    this.dom = {0..initialSize : int - 1};
    this.complete();
    this.cap = dom.size;
  }

  proc init(arr : [?D] ?eltType, growthRate = VectorGrowthRate) {
    this.eltType = eltType;
    this.growthRate = growthRate;
    this.dom = {0..#D.size};
    this.complete();
    this.arr = arr;
    this.cap = arr.size;
    this.sz = arr.size;
  }

  proc append(elt : eltType) {
    if sz == cap {
      var oldCap = cap;
      cap = round(cap * growthRate) : int;
      if oldCap == cap then cap += 1;
      this.dom = {0..#cap};
    }
    
    this.arr[sz] = elt;
    sz += 1;
  }
  
  proc intersection(other : Vector(eltType)) : Vector(eltType) {
    return new Vector(Utilities.intersection(this.toArray(), other.toArray()));
  }
  
  proc intersectionSize(other : Vector(eltType)) : int {
    return Utilities.intersectionSize(this.toArray(), other.toArray());
  }

  proc append(elts : [] eltType) {
    if sz + elts.size >= cap {
      cap = sz + elts.size;
      this.dom = {0..#cap};
    }

    this.arr[sz..#elts.size] = elts;
    sz += elts.size;
  }
  
  proc append(ir : _iteratorRecord) {
    if iteratorToArrayElementType(ir.type) != eltType {
      compilerError(
          "Attempt to append an iterable expression of type '", 
          iteratorToArrayElementType(ir.type) : string, "' when need type '", 
          eltType : string, "'"
      );
    }

    for elt in ir do append(elt);
  }

  proc sort() {
    Sort.sort(toArray());
  }

  proc this(idx : integral) ref {
    return arr[idx];
  }

  iter these() ref {
    for a in this.arr[0..#sz] do yield a;
  }

  iter these(param tag : iterKind) ref where tag == iterKind.standalone {
    forall a in this.arr[0..#sz] do yield a;
  }

  proc size return sz;

  proc clear() {
    this.sz = 0;
  }

  // Returns a reference to the array
  pragma "no copy return"
  proc toArray() {
    return arr[0..#sz];
  }

  proc getArray() {
   var dom = {0..#sz};
   var arr : [dom] eltType = this.arr[0..#sz];
   return arr;
  }

  proc readWriteThis(f) {
    f <~> "(Vector) {" <~> this.toArray() <~> "}";
  }
}
