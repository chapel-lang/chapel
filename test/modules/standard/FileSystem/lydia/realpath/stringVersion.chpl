use FileSystem;

var relativePath = here.cwd() + "/../realpath/./";
var shortened = realPath(relativePath);
if shortened != here.cwd() {
  writeln("Expected " + here.cwd() + " but got " + shortened);
} else {
  writeln("It worked!");
}
