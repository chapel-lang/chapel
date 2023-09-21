use Regex;

@deprecated("'Regex.compile' is deprecated. Please use 'new regex()' instead.")
proc compile(x) {
  return new regex(x);
}

@deprecated("regex.sub is deprecated. Please use string.replace.")
proc regex.sub(x, y) {
  return y.replace(this, x);
}