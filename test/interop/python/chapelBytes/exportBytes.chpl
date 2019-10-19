use Bytes;

export proc noArgsRetBytes(): bytes {
  var result: bytes = "foo";
  return result;
}

export proc takeBytesRetVoid(in b: bytes): void {
  writeln(b);

  for i in 1..b.length do
    if b[i].toByte() == 0x00 then
      writeln("Found embedded null at: " + i:string);

  return;
}

export proc takeBytesRetBytes(in b: bytes): bytes {
  var result: bytes = b + b", yo";
  return result;
}
