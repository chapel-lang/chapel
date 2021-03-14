// This is a step up in complexity from
//     test/reductions/reduceLoopOfPromoted.chpl
// esp.
//     var z = + reduce ([i in 1..3] (10 * A));
//
// Namely, here the elements of the forall expression are themselves
// forall expressions over a scalar or over a promoted expression.

config const nn=2;
config const mm=3;

// Also consider the case when these are ranges, not domains.
const OuterDom = {1..nn};
const InnerDom = {1..mm};

const ArrayDom = {0..2};
var MyArray: [ArrayDom] real = [1.0, 2.0, 3.0];

var zScalar = + reduce ([i in OuterDom] [j in InnerDom] j);
writeln(zScalar);

var zPromo  = + reduce ([i in OuterDom] [j in InnerDom] (10 * MyArray));
writeln(zPromo);

// Stepping stones towards implementing these reduce expressions.

// We can rewrite the reductions using reduce intents.

var ResultScalar: [InnerDom] int = 0;
forall i in OuterDom with (+ reduce ResultScalar) do
  ResultScalar += [j in InnerDom] j;
writeln(ResultScalar);

var ResultPromo: [InnerDom] [ArrayDom] real = 0;
forall i in OuterDom with (+ reduce ResultPromo) do
  ResultPromo += [j in InnerDom] (10 * MyArray);
writeln(ResultPromo);

// If this is our accumulation state, we need to be able to add to it.

var ASscalar: [InnerDom] int = 0;
ASscalar += [j in InnerDom] j;
writeln(ASscalar);

var ASpromo: [InnerDom] [ArrayDom] real = 0;
ASpromo += [j in InnerDom] (10 * MyArray);
writeln(ASpromo);
