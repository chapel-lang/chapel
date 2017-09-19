config const i = 2;


iter myiter() throws {

  if i == 0 then
    throw new Error("Test error 0");

  for j in 1..2 {
    yield j;
    if i == j then
      throw new Error("Test error " + j);
  }

  if i == 3 then
    throw new Error("Test error 3");
}

proc test() {
  try {
    writeln("before iter");
    for i in myiter() {
      writeln(i);
    }
    writeln("after iter");
  } catch e {
    writeln("Caught error ", e.msg);
  }
}

test();
