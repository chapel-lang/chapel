config const i = 2;


iter myiter() throws {
  if i == 0 then
    throw new Error("Test error 0");

  yield 1;

  if i == 1 then
    throw new Error("Test error 1");
  
  yield 2;
  
  if i == 2 then
    throw new Error("Test error 2");
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
