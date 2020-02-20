
const DOM = {1..1};
record R { }

type T1 = int;
type T2 = domain(1);
type T3 = [DOM] int;
type T4 = R;

// An error for 'int?' or 'R?' would exit compilation early,
// so the other errors would not be reported during the same compilation.

var x1: T1?;
var x2: T2?;
var x3: T3?;
var x4: T4?;
var x5: domain(1)?;
var x6: ([1..2]int)?;

type Q1 = T1?;
type Q2 = T2?;
type Q3 = T3?;
type Q4 = T4?;
