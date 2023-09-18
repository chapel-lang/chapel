// test warning for deprecated implicit conversion

// bool(?w) exists but is already deprecated

proc intq(arg: int(?w)) {
  writeln("in intq(", arg.type:string, ")");
}
proc uintq(arg: uint(?w)) {
  writeln("in uintq(", arg.type:string, ")");
}
proc realq(arg: real(?w)) {
  writeln("in realq(", arg.type:string, ")");
}
proc imagq(arg: imag(?w)) {
  writeln("in imagq(", arg.type:string, ")");
}
proc complexq(arg: complex(?w)) {
  writeln("in complexq(", arg.type:string, ")");
}

// not expecting warning for these cases
var oneInt: int = 1;
var oneUint: uint = 1;
intq(oneInt);
uintq(oneUint);
realq(1.0);
imagq(1.0i);
complexq(1.0+1.0i);

// expecting warning in these cases
intq(true);
uintq(true);
realq(oneInt);
complexq(oneInt);
