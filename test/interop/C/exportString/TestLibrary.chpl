




export proc noArgsRetString(): string {
  return "Greetings, human!";
}

export proc stringArgsRetVoid(a: string, b: string) {
  writeln(a);
  writeln(b);
}

export proc stringArgsRetString(a: string, b: string): string {
  return a + b;
}

