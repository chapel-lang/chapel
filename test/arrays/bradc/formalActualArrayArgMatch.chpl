use BlockDist;

config param testErrorCase = 0;

const D = {1..10};
const D2 = {0..9};
const D3 = D dmapped Block({1..10});

var A: [D] real;
var A2: [D2] real;
var A3: [D3] real;
var A4: [D dmapped Block({1..10})] real;
var A5: [D dmapped Block({1..3})] real;  // same type, different dist value

proc foo(X: [D] real) {
  writeln("In foo!");
}

proc bar(X: [D3] real) {
  writeln("In bar!");
}

//
// Here, foo doesn't specify a domain map for D, so accepts actuals as
// long as the index set matches.  Therefore, only A2 does not.
//
foo(A);
if testErrorCase == 1 then
  foo(A2);
foo(A3);
foo(A4);
foo(A5);

//
// Here, bar() specifies a domain map, so doesn't accept actuals whose
// domain map doesn't match, either in type (A and A2) or value (A5).
//
if testErrorCase == 3 then
  bar(A);
if testErrorCase == 2 then
  bar(A2);
bar(A3);
bar(A4);
if testErrorCase == 4 then
bar(A5);
