enum rangeTypeOptions { posInt, negInt, posUint, negUint };

config const rangeType:rangeTypeOptions;


param typeSize = 8;
param stride = 1;

select (rangeType) {
  when rangeTypeOptions.posInt {
    var r = min(int(typeSize))..max(int(typeSize)) by stride;
    testRange(r);
  }
  when rangeTypeOptions.negInt {
    var r = min(int(typeSize))..max(int(typeSize)) by -stride;
    testRange(r);
  }
  when rangeTypeOptions.posUint {
    var r = min(uint(typeSize))..max(uint(typeSize)) by stride;
    testRange(r);
  }
  when rangeTypeOptions.negUint {
    var r = min(uint(typeSize))..max(uint(typeSize)) by -stride;
    testRange(r);
  }

}
proc testRange(r) {
  var numIterations = 0;
  for i in r {
    if (numIterations-1 == max(uint(typeSize))) {
      numIterations = 0;
      break;
    }
    numIterations+=1;
  }
  if (numIterations-1 != max(uint(typeSize))) {
    writeln("Wrong number of iterations occured");
  }
}

