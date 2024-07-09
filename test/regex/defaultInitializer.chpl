// tests the default type initializer, mostly to make sure it doesn't segfault
use Regex;
use IO;
use FileSystem;

config type T = string;

{
  var x: regex(T);
  writeln(x);
}

{
  var x: regex(T);
  var s = "hello":T;
  var m = s.find(x);
  writeln("find: '", m, "'");
}

{
  // calls the assignment op version that uses `._serialize`, which calls `qio_regex_get_options`
  var x: regex(T);
  on Locales[numLocales-1] {
    var y = new regex("":T);
    y = x;
    writeln("y: '", y, "'");
    writeln("x: '", x, "'");
  }
}

{
  var x: regex(T);
  for i in x.split("hello":T) {
    writeln("'", i, "'");
  }
}

{
  var x: regex(T);
  var s = "hello":T;
  var s2 = s.replace(x, "world":T);
  writeln("replaced: '", s2, "'");
}

{
  const testfile = "test.txt";
  {
    // make a test file
    var w = openWriter(testfile, locking=false);
    w.writeln("hello");
    w.writeln("world");
  }
  {
    try! {
      // search for the regex, triggers the bug
      var x: regex(T);
      var r = openReader(testfile, locking=false);
      var m = r.search(x);
      writeln(m); // succeds empty match when comm!=none
    } catch e: SystemError {
      writeln("caught: ", e.message()); // thrown when comm=none
    }
  }
  // delete the file
  remove(testfile);
}
