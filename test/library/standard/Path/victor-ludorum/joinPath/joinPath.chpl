   use Path;

// writing tests for path joining where merging path name starts with ('/')
     writeln(joinPaths("/foo/bar", "/baz"));

// writing tests for path joining where path name dp 
// not have ('/') either in the starting of string and 
// also not in the end of string 
     writeln(joinPaths("/foo", "./baz"));

// joining more than 2 path example 
     writeln(joinPaths("/foo/", "", "./baz"));

// joining path having no '/' either in the starting 
// of string and also not in the end of string
     writeln(joinPaths("/foo" , "baz"));

// writing tests for path joining where merging path name starts with ('/')
     writeln(joinPaths("/foo/" , "/baz"));

// writing tests for path joining where path name starts with ('/') and in the 
// merging path name their is no '/'
     writeln(joinPaths("/foo/" , "./baz"));

// writing example test for joinPath()
     writeln(joinPaths("/foo/" , "baz"));
	 
// writing example test for single string for function .	 
     writeln(joinPaths("/foo/"));
	 
     writeln(joinPath("/foo","koo","./ik","/baz","/kju"));
