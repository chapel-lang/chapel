enum day {sun, mon, tue, wed, thu, fri, sat};
use day;

// pick some arbitrary enums
config const lo = mon,
             hi = fri;

// range of enums
const meetingfree = lo..hi by 2;

// serial iteration
for c in meetingfree do
  writeln(c);
writeln("---");

// array over enum
var A: [sun..sat] int;

// zippered iteration over array of enum
forall (a, i) in zip(A, 1..) do
  a = i;

// parallel iteration
forall c in meetingfree do
  A[c] = -A[c];

writeln(A);

writeln(A[meetingfree]);
