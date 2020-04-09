enum color { red, green, blue };

proc foo(e: enumerated) {
  writeln("In foo: ", e);
}

foo(color.green);
