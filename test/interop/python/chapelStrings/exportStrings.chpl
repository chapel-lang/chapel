use Bytes;

export proc getFirstNullCodepointPos(in s: string): int {
  for i in 1..s.size do
    if s[i].toCodepoint() == 0x00 then
      return (i - 1);
  return -1;
}

export proc printString(in s: string): void {
  writeln(s);
  return;
}

export proc noArgsRetString(): string {
  var result: string = "Foo!";
  return result;
}

export proc takeStringRetVoid(in s: string): void {
  writeln(s);

  const idx = getFirstNullCodepointPos(s);

  if idx >= 0 {
    const highlight = " " * idx + "^";
    writeln(highlight);
    writeln("Found embedded null at (0 index): " + idx:string);
  }

  return;
}

export proc takeStringRetString(in s: string): string {
  var result: string = s + ", yo!";
  return result;
}
