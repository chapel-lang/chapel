/* Tests accepting a string */
export proc takesString(x: string) {
  writeln(x);
}

/* Tests returning a string */
export proc getString(): string {
  var ret: string = "whee";
  return ret;
}

/* Tests taking and returning a string */
export proc takeAndReturn(x: string): string {
  return x;
}
