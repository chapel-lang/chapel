
var x: int;

begin       with(in x, inout x, out x, const x, const in x, const ref x, ref x, param x, type x)  {}

cobegin     with(in x, inout x, out x, const x, const in x, const ref x, ref x, param x, type x)  {{}{}}

coforall x  with(in x, inout x, out x, const x, const in x, const ref x, ref x, param x, type x)  {}

forall x    with(in x, inout x, out x, const x, const in x, const ref x, ref x, param x, type x)  {}
