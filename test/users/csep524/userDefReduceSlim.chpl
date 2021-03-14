var randInts: [1..10] int;

const mostCommon = mostFrequent reduce randInts;
writeln(mostCommon);

class mostFrequent: ReduceScanOp {
  type eltType;

  var dom : domain(int);

  proc accumulate(inputval: eltType) {
    dom += inputval;
  }

  proc combine(partner: mostFrequent) {
    forall i in mostFrequent.dom {
      dom += i;
    }
  }

  proc generate() {
    return 5;
  }
}
