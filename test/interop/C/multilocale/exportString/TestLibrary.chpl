




export proc noArgsRetString(): string {
  return "Greetings, human!";
}

export proc stringArgsRetVoid(in a: string, in b: string) {
  writeln(a);
  writeln(b);
}

export proc stringArgsRetString(in a: string, in b: string): string {
  return a + b;
}

