

var DOM = {1..2};
var ARR: [DOM] int;

proc testFormal(const in domArg, const in arrArg) {
  domArg = {3..4};
  arrArg = [5, 6];
}

proc main {
  testFormal(DOM, ARR);

  begin with (const in DOM, const in ARR) {
    DOM = {3..4};
    ARR = [5, 6];
  }

  cobegin with (const in DOM, const in ARR) {
    { DOM = {3..4}; }
    { ARR = [5, 6]; }
  }

  coforall 1..3 with (const in DOM, const in ARR) {
    DOM = {3..4};
    ARR = [5, 6];
  }
}
