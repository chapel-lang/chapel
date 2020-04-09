
class GenericClass {
  type idxType;
  param rank : int;
  // Needs a runtime field, otherwise 'delete' can fail?
  var x : rank*idxType;

  proc init(type i, param r : int) {
    this.idxType = i;
    this.rank = r;
  }

  iter these() {
    for i in 1..10 {
      var ret : rank*idxType;
      for r in 0..rank-1 do ret(r) = i*(r+1);
      yield ret;
    }
  }

  proc chpl__promotionType() type {
    return rank*idxType;
  }
}

class ConcreteClass {
  var base : 2*int;

  proc init(x : int) {
    base = (x, -x);
  }

  proc chpl__promotionType() type {
    return base.type;
  }

  iter these() {
    for i in 1..10 {
      yield i*base;
    }
  }
}

record GenericRecord {
  type eltType;

  proc init(type t) {
    this.eltType = t;
  }

  proc chpl__promotionType() type {
    return eltType;
  }

  iter these() {
    for i in 1..10 {
      yield i:eltType;
    }
  }
}

record ConcreteRecord {
  var x : int;
  proc init(x : int) {
    this.x = x;
  }

  proc chpl__promotionType() type {
    return int;
  }

  iter these() {
    for i in 1..10 do yield x;
  }
}

proc promote(x) {
  const header = "----- " + x.type:string + " -----";
  writeln(header);

  const inc    = 1 + x;
  const negate = -1 * x;
  const add    = x + x;

  writeln(" 1 + x = ", inc);
  writeln("-1 * x = ", negate);
  writeln(" x + x = ", add);

  writeln("-" * header.size);
  writeln();
}

proc main() {
  var cr = new ConcreteRecord(10);
  promote(cr);

  var gri = new GenericRecord(int);
  promote(gri);

  var grr = new GenericRecord(real);
  promote(grr);

  var cc = new borrowed ConcreteClass(10);
  promote(cc);

  var gcTwo = new borrowed GenericClass(int, 2);
  promote(gcTwo);

  var gcOne = new borrowed GenericClass(int, 1);
  promote(gcOne);
}

