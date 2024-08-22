use CTypes;

export proc takesArray(in x: [] c_ptrConst(c_char)) {
  for str in x {
    // Note: this assumes x will have initial contents
    writeln(string.createCopyingBuffer(str));
  }
}

export proc returnsArray(): [0..3] c_ptrConst(c_char) {
  var x: [0..3] c_ptrConst(c_char) = ["guess", "this", "could", "work?"];
  return x;
}
