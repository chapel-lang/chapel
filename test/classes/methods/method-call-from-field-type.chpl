record R {
  var x: something(); // this doesn't resolve currently. If it
                      // were to start resolving, it should probably
                      // result in an error later, because 'this'
                      // isn't available for general method calls
                      // during initialization.
}

proc R.something() type { return int; }

var r:R;
