export proc makeStringArray(): [] string {
  var result: [0..2] string = ['foo', 'bar', 'baz'];
  return result;
}

export proc display(in msg: string) {
  writeln(msg);
}

