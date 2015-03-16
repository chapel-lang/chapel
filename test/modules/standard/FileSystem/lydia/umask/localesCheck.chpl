use FileSystem;

var newMode = 0o777;
var oldMode = umask(newMode);

on Locales[1] {
  var anotherMode = 0o444;
  var myMode = umask(anotherMode);
  if (myMode == oldMode) {
    writeln("umask is locale specific");
  } else if (myMode == newMode) {
    writeln("umask is program specific");
  } else {
    writeln("I don't even know where this mode came from");
    writeln(myMode);
  }
}
var locale0sMode = umask(oldMode);
if (locale0sMode == newMode) {
  writeln("As suspected, changing locale 1's mode didn't affect locale 0's mode");
} else {
  writeln("Changing locale 1's mode alterred locale 0's mode");
}
