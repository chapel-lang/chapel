export proc noArgsRetBytes(): bytes {
  var result = b"Greetings!";
  return result;
}

export proc bytesArgRetVoid(a: bytes, b: bytes) {
  writeln(a);
  writeln(b);
}

export proc bytesArgsRetBytes(a: bytes, b: bytes): bytes {
  var result = a + b;
  return result;
}

