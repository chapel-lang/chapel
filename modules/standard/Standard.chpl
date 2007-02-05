def _tuple2string(t) {
  var s: string;
  for param i in 1..t.size do
    s.write(t(i));
  return s;
}

def assert(test: bool) {
  if !test then
    _handleRuntimeError("assert failed");
}

def assert(test: bool, args ...?numArgs) {
  if !test then
    _handleRuntimeError("assert failed - "+_tuple2string(args));
}

def halt() {
  _handleRuntimeError("halt reached");
}

def halt(args ...?numArgs) {
  _handleRuntimeError("halt reached - "+_tuple2string(args));
}
