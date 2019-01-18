use Path;

var originalPath = "resultSameAsArgument.chpl";
// Well, I know this file exists, better not rename it.
var firstRun = realPath(originalPath);
// firstRun will be an exact path
var secondTimeThrough = realPath(firstRun);
// Nothing should need to be done to firstRun, so the result of that
// call should match the argument.
if (firstRun == secondTimeThrough) {
  writeln("Yay, realPath works on a path that has already been fixed!");
} else {
  writeln("Expected " + firstRun + " but was " + secondTimeThrough);
 }
