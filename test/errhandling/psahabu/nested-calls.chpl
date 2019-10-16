use ExampleErrors;

proc returnIntOrThrow(i: int) throws {
  if (i < 10) {
    throw new owned StringError(i:string + " is less than 10");
  }
  return i;
}

proc consumeInt(i: int) {
  writeln("failed: consuming int");
  return i + 1;
}

proc consumeTwoInts(i: int, j: int) {
  writeln("failed: consuming two ints");
  return i + j;
}

proc consumeIntOrThrow(i: int) throws {
  writeln("failed: consuming or throwing int");
  if (i < 20) {
    throw new owned StringError(i:string + "is less than 20");
  }
  return i;
}


try {
  writeln("consumeInt");
  var x = consumeInt(returnIntOrThrow(5));
} catch err {
  writeln(err.message());
}

try {
  writeln("consumeTwoInt (error first)");
  var y1 = consumeTwoInts(returnIntOrThrow(5), returnIntOrThrow(10));
} catch err {
  writeln(err.message());
}

try {
  writeln("consumeTwoInt (error second)");
  var y2 = consumeTwoInts(returnIntOrThrow(10), returnIntOrThrow(5));
} catch err {
  writeln(err.message());
}

try {
  writeln("consumeIntOrThrow");
  var z = consumeIntOrThrow(returnIntOrThrow(5));
} catch err {
  writeln(err.message());
}
