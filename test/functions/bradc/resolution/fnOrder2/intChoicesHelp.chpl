use intChoicesTypes;

proc int8or64(x: int(64)) {
  writeln("64");
}

proc int8or64(x: int(8)) {
  writeln("8");
}

// ===

proc int8or32(x: int(32)) {
  writeln("32");
}

proc int8or32(x: int(8)) {
  writeln("8");
}

// ===

proc param64OrNot(x: int(64)) {
  writeln("64-bit not param");
}

proc param64OrNot(param x: int(64)) {
  writeln("64-bit param");
}

// ===

proc param8OrNot(x: int(8)) {
  writeln("8-bit not param");
}

proc param8OrNot(param x: int(8)) {
  writeln("8-bit param");
}

// ===

proc intOrEnum(x: int(64)) {
  writeln("64");
}

proc intOrEnum(x: sizes) {
  writeln("sizes");
}

proc intOrEnum(x: color) {
  writeln("color");
}

// ===

proc twoInts(x: int(64), y: int(64)) {
  writeln("64, 64");
}

proc twoInts(x: int(64), y: int(8)) {
  writeln("64, 8");
}

proc twoInts(x: int(8), y: int(64)) {
  writeln("8, 64");
}

proc twoInts(x: int(8), y: int(8)) {
  writeln("8, 8");
}

// ===

proc intEnum(x: int(64), y: int(64)) {
  writeln("64, 64");
}

proc intEnum(x: int(64), y: color) {
  writeln("64, color");
}

proc intEnum(x: color, y: int(64)) {
  writeln("color, 64");
}

proc intEnum(x: color, y: color) {
  writeln("color, color");
}

// ----

proc allSigs(x: sizes, y: sizes) {
  write("sizes, ");
  writeln("sizes");
}

proc allSigs(x: sizes, param y: int(8)) {
  write("sizes, ");
  writeln("param 8");
}

proc allSigs(x: sizes, y: color) {
  write("sizes, ");
  writeln("color");
}

proc allSigs(x: sizes, param y: int(64)) {
  write("sizes, ");
  writeln("param 64");
}

proc allSigs(x: sizes, y: int(64)) {
  write("sizes, ");
  writeln("64");
}

proc allSigs(x: sizes, y: int(8)) {
  write("sizes, ");
  writeln("8");
}

// ---

proc allSigs(x: color, y: sizes) {
  write("color, ");
  writeln("sizes");
}

proc allSigs(x: color, y: color) {
  write("color, ");
  writeln("color");
}

proc allSigs(x: color, param y: int(64)) {
  write("color, ");
  writeln("param 64");
}

proc allSigs(x: color, param y: int(8)) {
  write("color, ");
  writeln("param 8");
}

proc allSigs(x: color, y: int(64)) {
  write("color, ");
  writeln("64");
}

proc allSigs(x: color, y: int(8)) {
  write("color, ");
  writeln("8");
}

// ----

proc allSigs(param x: int(64), y: sizes) {
  write("param 64, ");
  writeln("sizes");
}

proc allSigs(param x: int(64), y: color) {
  write("param 64, ");
  writeln("color");
}

proc allSigs(param x: int(64), param y: int(64)) {
  write("param 64, ");
  writeln("param 64");
}

proc allSigs(param x: int(64), param y: int(8)) {
  write("param 64, ");
  writeln("param 8");
}

proc allSigs(param x: int(64), y: int(64)) {
  write("param 64, ");
  writeln("64");
}

proc allSigs(param x: int(64), y: int(8)) {
  write("param 64, ");
  writeln("8");
}

// ----


proc allSigs(param x: int(8), y: sizes) {
  write("param 8, ");
  writeln("sizes");
}

proc allSigs(param x: int(8), y: color) {
  write("param 8, ");
  writeln("color");
}

proc allSigs(param x: int(8), param y: int(64)) {
  write("param 8, ");
  writeln("param 64");
}

proc allSigs(param x: int(8), param y: int(8)) {
  write("param 8, ");
  writeln("param 8");
}

proc allSigs(param x: int(8), y: int(64)) {
  write("param 8, ");
  writeln("64");
}

proc allSigs(param x: int(8), y: int(8)) {
  write("param 8, ");
  writeln("8");
}

// ----

proc allSigs(x: int(64), y: sizes) {
  write("64, ");
  writeln("sizes");
}

proc allSigs(x: int(64), y: color) {
  write("64, ");
  writeln("color");
}

proc allSigs(x: int(64), param y: int(64)) {
  write("64, ");
  writeln("param 64");
}

proc allSigs(x: int(64), param y: int(8)) {
  write("64, ");
  writeln("param 8");
}

proc allSigs(x: int(64), y: int(64)) {
  write("64, ");
  writeln("64");
}

proc allSigs(x: int(64), y: int(8)) {
  write("64, ");
  writeln("8");
}


// ====

proc allSigs(x: int(8), y: sizes) {
  write("8, ");
  writeln("sizes");
}

proc allSigs(x: int(8), y: color) {
  write("8, ");
  writeln("color");
}

proc allSigs(x: int(8), param y: int(64)) {
  write("8, ");
  writeln("param 64");
}

proc allSigs(x: int(8), param y: int(8)) {
  write("8, ");
  writeln("param 8");
}

proc allSigs(x: int(8), y: int(64)) {
  write("8, ");
  writeln("64");
}

proc allSigs(x: int(8), y: int(8)) {
  write("8, ");
  writeln("8");
}

