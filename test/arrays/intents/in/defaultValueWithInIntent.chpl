// test from https://github.com/chapel-lang/chapel/issues/11424
var emptyArray: [1..0] string;

proc f(in x: [] string = emptyArray) {
  writeln(x.size);
}

f(['f']);
