




export proc noArgsRetString(): string {
  return "Greetings human!";
}

export proc stringArgsRetVoid(a: string, b: string) {
  writeln(a);
  writeln(b);
}

export proc stringArgsRetString(a: string, b: string): string {
  return a + b;
}

//
// Strings passed in via C may be string literals in RODATA. Use this test
// to validate that the correct behavior occurs when constructing a string
// from a read only buffer.
// To do this, we make a new string that is a shallow copy of the input,
// and then modify it to see what happens.
//
export proc inplaceUpdateVisible(a: string) {
  var tmp = new string(a, isowned=false);
  tmp[0] = '%';
}

