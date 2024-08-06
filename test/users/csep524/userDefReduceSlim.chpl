var randInts: [1..10] int;

const mostCommon = mostFrequent reduce randInts;
writeln(mostCommon);

class mostFrequent: ReduceScanOp {
  type eltType;

  var mySet : domain(int);

  proc identity {
    var x : eltType;
    return x;
  }

  proc accumulate(inputval: eltType) {
    mySet += inputval;
  }

  proc combine(partner: mostFrequent(?)) {
    // This is a user-submitted bug where 'mostFrequent' was mistakenly used
    // and caused an internal compiler error.
    forall i in mostFrequent.mySet {
      mySet += i;
    }
  }

  proc generate() {
    return 5;
  }

  proc clone() do return new mostFrequent(eltType=eltType);
}
