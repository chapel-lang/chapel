var D1 = {1..10};
var A: [D1] int;

const D2 = {1..20 by 2};
//const D3 = {1:uint..10:uint};
//const D4 = {1..2, 1..5};

D1 = D2; // can domain reassign change stridedness?
//D1 = D3; // can domain reassign change idxType? Currently a compiler error
//D1 = D4; // can domain reassign change rank? Currently a compiler error
