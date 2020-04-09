config param tryError = true;

proc int8or64(x: int(8)) {
  writeln("8");
}

proc int8or64(x: int(64)) {
  writeln("64");
}

proc int8or32(x: int(8)) {
  writeln("8");
}

proc int8or32(x: int(32)) {
  writeln("32");
}

proc param64OrNot(param x: int(64)) {
  writeln("64-bit param");
}

proc param64OrNot(x: int(64)) {
  writeln("64-bit not param");
}

proc param8OrNot(param x: int(8)) {
  writeln("8-bit param");
}

proc param8OrNot(x: int(8)) {
  writeln("8-bit not param");
}

proc param8Or32OrNot(param x: int(8)) {
  writeln("8-bit param");
}

proc param8Or32OrNot(x: int(8)) {
  writeln("8-bit not param");
}

proc param8Or32OrNot(param x: int(32)) {
  writeln("32-bit param");
}

proc param8Or32OrNot(x: int(32)) {
  writeln("32-bit not param");
}

proc allSigs(x: int(8)) {
  writeln("8");
}

proc allSigs(x: int(64)) {
  writeln("64");
}

proc allSigs(param x: int(8)) {
  writeln("param 8");
}

proc allSigs(param x: int(64)) {
  writeln("param 64");
}

enum sizes {
  small = 126,
  big = 131072
};

var aSize: sizes = sizes.small;
var anotherSize: sizes = sizes.big;

writeln("int8or64 variants");
if !tryError then writeln(); else int8or64(sizes.small);
if !tryError then writeln(); else int8or64(sizes.big);
if !tryError then writeln(); else int8or64(aSize);
if !tryError then writeln(); else int8or64(anotherSize);
writeln();

writeln("int8or32 variants");
if !tryError then writeln(); else int8or32(sizes.small);
if !tryError then writeln(); else int8or32(sizes.big);
if !tryError then writeln(); else int8or32(aSize);
if !tryError then writeln(); else int8or32(anotherSize);
writeln();

writeln("param64OrNot variants");
if !tryError then writeln(); else param64OrNot(sizes.small);
if !tryError then writeln(); else param64OrNot(sizes.big);
if !tryError then writeln(); else param64OrNot(aSize);
if !tryError then writeln(); else param64OrNot(anotherSize);
writeln();

writeln("param8OrNot variants");
if !tryError then writeln(); else param8OrNot(sizes.small);
if (tryError) then param8OrNot(sizes.big); else writeln();
if (tryError) then param8OrNot(aSize); else writeln();
if (tryError) then param8OrNot(anotherSize); else writeln();
writeln();

writeln("param8Or32OrNot variants");
if !tryError then writeln(); else param8Or32OrNot(sizes.small);
if !tryError then writeln(); else param8Or32OrNot(sizes.big);
if !tryError then writeln(); else param8Or32OrNot(aSize);
if !tryError then writeln(); else param8Or32OrNot(anotherSize);
writeln();

writeln("allSigs variants");
if !tryError then writeln(); else allSigs(sizes.small);
if !tryError then writeln(); else allSigs(sizes.big);
if !tryError then writeln(); else allSigs(aSize);
if !tryError then writeln(); else allSigs(anotherSize);
writeln();



