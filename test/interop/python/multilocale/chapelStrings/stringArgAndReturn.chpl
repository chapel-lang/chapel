export proc noArgsRetString(): string {
  return "boo!";
}

export proc stringArgsRetVoid(in a: string) {
  writeln(a);
}

export proc stringArgsRetString(in a: string, in b: string): string {
  return a + b;
}
