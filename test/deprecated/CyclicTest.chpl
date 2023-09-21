// {range,domain}.stridable is deprecated by Vass in 1.31

use CyclicStridable;

var D1 = {1..3};
var D2 = {1..7 by 3};

var CD1 = D1 dmapped Cyclic(1);
var CD2 = D2 dmapped Cyclic(2);

var CA1: [CD1] int = D1;
var CA2: [CD2] int = D2;

writeln("% ", (CD1, CD2, CA1, CA2));

var newD2 = Cyclic.createDomain(D2);
var newA2 = Cyclic.createArray(D2, int);

writeln("% ", (newD2, newA2));

for x in CD2 do writeln("% CD2: ", x);
for x in CA2 do writeln("% CA2: ", x);
for x in zip(CD2, CA2) do writeln("% CD2,CA2: ", x);
for x in zip(D2, CD2)  do writeln("% D2, CD2: ", x);

forall w in CD2 do writeln("% forall CD2");
forall w in CA2 do writeln("% forall CA2");
forall w in zip(D1, CD1, CD2) do writeln("% forall D1, CD1, CD2");
forall w in zip(D2, CD1, CD2) do writeln("% forall D2, CD1, CD2");
forall w in zip(CD1, CA1, CA2) do writeln("% forall CD1, CA1, CA2");
forall w in zip(CD2, CA1, CA2) do writeln("% forall CD2, CA1, CA2");

writeln("% ", (CD1[2..], CD2[..5]));
writeln("% ", (CA1[2], CA2[4], CA2[4.. by 3], CA2[..4 by -3]));

CD1 = 1..5; CD2 = 1..7 by 2;
writeln("% ", (CD1, CD2, CA1, CA2));
