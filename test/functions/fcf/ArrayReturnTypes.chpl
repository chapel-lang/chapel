//
// This module establishes that we can parse array return types with varying
// degrees of genericity. Previously, we could not parse function types
// with array return types due to a parser conflict.
//

var a = 0..0;
var b = 0..0;

proc p0(): [] {}
proc p1(): [a, b] {}
proc p2(): [a, b] ? {}
proc p3(): [a, b] int {}

type t1 = proc(_: int, _: int): [];
type t2 = proc(_: int, _: int): [] int;
type t3 = proc(_: int, _: int): [] ?;
type t4 = proc(_: int, _: int): [a];
type t5 = proc(_: int, _: int): [a] int;
type t6 = proc(_: int, _: int): [a] ?;
type t7 = proc(_: int, _: int): [a, b];
type t8 = proc(_: int, _: int): [a, b] int;
type t9 = proc(_: int, _:int): [a, b] ?;
type t10 = proc(): [];
type t11 = proc(): [] int;
type t11 = proc(): [] ?;
type t12 = proc(_: int): [];
type t13 = proc(_: int): [] int;
type t14 = proc(_: int): [] ?;

