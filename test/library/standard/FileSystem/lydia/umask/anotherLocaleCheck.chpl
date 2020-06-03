use FileSystem;

var newMode = 0o777;
var oldMode = Locales[1].umask(newMode);
var restoring = Locales[1].umask(oldMode);
if (restoring != newMode) {
  writeln("Expected the second locale.umask to give ", newMode, " but was ", restoring);
} else {
  writeln("Altering a locale's umask from a different locale worked");
}
