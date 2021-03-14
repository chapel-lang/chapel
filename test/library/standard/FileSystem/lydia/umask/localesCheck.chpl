use FileSystem;

var newMode = 0o777;
var oldMode = here.umask(newMode);

on Locales[1] {
  var anotherMode = 0o444;
  var myMode = here.umask(anotherMode);
  if (myMode == oldMode) {
    writeln("Locale 1's umask unaffected by Locale 0");
  } else if (myMode == newMode) {
    writeln("Locale 1's umask affected by Locale 0");
  } else {
    writeln("I don't even know where this mode came from");
    writeln(myMode);
  }
}
var locale0sMode = here.umask(oldMode);
if (locale0sMode == newMode) {
  writeln("As expected, changing locale 1's mode didn't affect locale 0's mode");
} else {
  writeln("Changing locale 1's mode alterred locale 0's mode");
}
