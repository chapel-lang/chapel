use Path;

// writing tests for path joining where merging path name starts with ('/')
writeln(joinPath("/foo/bar", "/baz"));

// writing tests for path joining where path name dp 
// not have ('/') either in the starting of string and 
// also not in the end of string 
writeln(joinPath("/foo", "./baz"));

// joining more than 2 path example 
writeln(joinPath("/foo/", "", "./baz"));

// joining path having no '/' either in the starting 
// of string and also not in the end of string
writeln(joinPath("/foo" , "baz"));

// writing tests for path joining where merging path name starts with ('/')
writeln(joinPath("/foo/" , "/baz"));

// writing tests for path joining where path name starts with ('/') and in the 
// merging path name their is no '/'
writeln(joinPath("/foo/" , "./baz"));

// writing example test for joinPath()
writeln(joinPath("/foo/" , "baz"));
	 
// writing example test for single string for function .	 
writeln(joinPath("/foo"));
writeln(joinPath("/foo","koo","./ik","/baz","/kju"));
