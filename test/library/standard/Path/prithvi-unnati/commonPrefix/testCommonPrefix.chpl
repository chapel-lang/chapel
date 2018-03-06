/*
Test cases to validate the commonPrefix([]) in Path.chpl
*/

use Path;

//paths=null
writeln(commonPrefix([""]));

//only one path has been sent as argument
writeln(commonPrefix(["/home/user/Documents/foo/bar"]));

//writing test for absolute path when one of the paths is substring of others
writeln(commonPrefix(["/home/user/Documents/foo/bar","/home/user/Documents/foo","/home/user/Documents/foo/bar/baz"]));

//testing when one path is absolute and the other is relative
writeln(commonPrefix(["/home/user/Documents/foo/bar","home/user/Documents/foo/bar"]));

//Only a substring is common
writeln(commonPrefix(["/home/user/Documents/foo/bar","/home/user/Desktop/bar","/home/user/Documents/bar"]));

//only relative paths are provided
writeln(commonPrefix(["../Documents/foo/bar","../Documents/foo/bar/baz","../Documents/foo"]));




