use CPtr;
proc main() {
  var x: c_array(int, 4);

  for i in 0..3 do
    x[i] = i;

  on Locales[numLocales-1] {
    // Test element access
    writeln(x[3]);

    // Test copy from remote data
    var copy = x;
    writeln(copy[3]);

    // Test assign from remote data
    var y: c_array(int, 4);
    y = x;
    writeln(y[3]);

    // Test element update
    x[3] = 10000;
  }

  writeln(x[3]);
}
