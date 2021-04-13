use BlockDist;

var distArr = newBlockArr(0..9, int);
var localArr: [0..9] int;

distArr = 3;

forall i in localArr.domain {
  localArr.localAccess[i] = distArr[i];
}

writeln(localArr);
