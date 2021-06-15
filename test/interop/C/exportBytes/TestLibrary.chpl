export proc noArgsRetBytes(): bytes {
  var result = b"Greetings!";
  return result;
}

export proc bytesArgRetVoid(in a: bytes, in b: bytes) {
  writeln(a);
  writeln(b);
}

export proc bytesArgsRetBytes(in a: bytes, in b: bytes): bytes {
  var result = a + b;
  return result;
}

