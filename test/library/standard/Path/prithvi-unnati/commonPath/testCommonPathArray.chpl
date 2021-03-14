/*
Test cases to validate the commonPath(paths: []) in Path.chpl
*/

use Path;

//paths=null
var EmptyArrayInput: [1..0] string;
writeln(commonPath(EmptyArrayInput));

//only one path has been sent as argument
writeln(commonPath(["/home/user/Documents/foo/bar"]));

//writing test for absolute path when one of the paths is substring of others
writeln(commonPath(["/home/user/Documents/foo/bar","/home/user/Documents/foo","/home/user/Documents/foo/bar/baz"]));

//testing when one path is absolute and the other is relative
writeln(commonPath(["/home/user/Documents/foo/bar","home/user/Documents/foo/bar"]));

//Only a substring is common
writeln(commonPath(["/home/user/Documents/foo/bar","/home/user/Desktop/bar","/home/user/Documents/bar"]));

//only relative paths are provided
writeln(commonPath(["../Documents/foo/bar","../Documents/foo/bar/baz","../Documents/foo"]));

//same paths
writeln(commonPath(["/home/user/Documents","/home/user/Documents"]));

//checking for array to be unchanged
var input = ["/home/user/Desktop/bar","/home/user/Documents/foo.chpl","/home/user/Documents/foo.good","/home/user/Documents/foo.txt","/home/user/Documents/bar.txt"];
writeln(commonPath(input(2..input.domain.last-1)));
writeln(input);
