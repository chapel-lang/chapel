use Time;

var negTd = new timeDelta(seconds=-30);
var posTd = new timeDelta(seconds=30);

var absNegTd = abs(negTd);
var absPosTd = abs(posTd);

assert(negTd != absNegTd);
assert(absNegTd == posTd);
assert(posTd == absPosTd);

// expect same behavior from new method approach
assert(absNegTd == negTd.abs());
assert(absPosTd == posTd.abs());
