// This is a variant of the bug reported in the following issue:
//
//   https://github.com/chapel-lang/chapel/issues/23168
//
// Unlike the case in the issue, this time a function FORMAL has the tuple
// type, which should also work fine (and certainly not throw internal errors).

proc bar(args: 3*int) { writeln(args); }
type Tbar = proc(args: 3*int): void; // used to break at compile-time

var barFcf: Tbar = bar;
barFcf((1,2,3));
