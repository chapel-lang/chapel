use Bytes;

export proc getFirstNullBytePos(in b: bytes): int {
  for i in 0..<b.size do
    if b[i] == 0x00 then
      return i;
  return -1;
}

export proc printBytes(in b: bytes): void {
  writeln(b);
  return;
}

export proc noArgsRetBytes(): bytes {
  var result: bytes = "Foo!";
  return result;
}

export proc takeBytesRetVoid(in b: bytes): void {
  writeln(b);

  const idx = getFirstNullBytePos(b);

  if idx >= 0 {
    const highlight = " " * idx + "^";
    writeln(highlight);
    writeln("Found embedded null at (0 index): " + idx:string);
  }

  return;
}

export proc takeBytesRetBytes(in b: bytes): bytes {
  var result: bytes = b + b", yo!";
  return result;
}
