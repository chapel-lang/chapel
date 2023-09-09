// This is a variant of the bug reported in the following issue:
//
//   https://github.com/chapel-lang/chapel/issues/23168
//
// Unlike the case in the issue, this time a function FORMAL has the tuple
// type, which should also work fine (and certainly not throw internal errors).

proc bar(args: 3*int) { }
// writeln(bar(1,2,3));
type Tbar = proc(args: 3*int): void; // breaks at compile-time
