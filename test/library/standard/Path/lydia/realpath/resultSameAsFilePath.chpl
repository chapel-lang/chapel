use Path;

var originalPath = "resultSameAsFilePath.chpl";
// Well, I know this file exists, better not rename it.
var firstRun = realPath(originalPath);
// firstRun will be an exact path
// use the exact path to open a file
var f = open(firstRun, iomode.r);

var secondTimeThrough = f.realPath();
// Nothing should need to be done to f's path, so the result of that
// call should match the argument.
if (firstRun == secondTimeThrough) {
  writeln("Yay, realPath works on a path that has already been fixed!");
} else {
  writeln("Expected " + firstRun + " but was " + secondTimeThrough);
 }
