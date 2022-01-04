use CommDiagnostics;

config type t = string;
config const numChars = 4;

proc main() {
  var data = ("x":t)*numChars;

  startCommDiagnostics();
  var dataLen = 0;

  // if data is "small" it'll be packed in the arg bundle.
  // in that case, we won't have any additional comm here beyond the `on`
  // itself.
  // if the data is "large", it'll do a GET for the buffer after deserialization
  on Locales[1] {
    dataLen = data.size;   // this is a PUT
    stopCommDiagnostics();

    writeln(getCommDiagnosticsHere());
  }
}
