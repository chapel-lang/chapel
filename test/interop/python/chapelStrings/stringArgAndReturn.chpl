export proc noArgsRetString(): string {
  return "boo!";
}

export proc stringArgsRetVoid(a: string) {
  writeln(a);
}

export proc stringArgsRetString(a: string, b: string): string {
  return a + b;
}
