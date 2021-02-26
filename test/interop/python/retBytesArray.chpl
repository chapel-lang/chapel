export proc makeBytesArray(): [] bytes {
  var result: [0..2] bytes = [b'f\x00oo', b'bar', b'baz'];
  return result;
}

export proc display(in msg: string) {
  writeln(msg);
}

