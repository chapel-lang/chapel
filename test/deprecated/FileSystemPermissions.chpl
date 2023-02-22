import FileSystem.getMode;
import FileSystem.copyMode;
import FileSystem.chmod;

// Since copyMode is overloaded, we have to call each overload to get the
// deprecation warnings.
try {
  copyMode("", "");
} catch e {
  writeln(e);
}

try {
  var err;
  copyMode(err, "", "");
} catch e {
  writeln(e);
}
