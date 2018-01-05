   use Path;

   writeln(isAbsPath("/foo/bar"));

   writeln(isAbsPath("./baz"));
	 
   writeln(isAbsPath("/baz/.."));
	 
   writeln(isAbsPath("baz/"));
	 
   writeln(isAbsPath("//server"));
	 
   writeln(isAbsPath("\\server"));
	 
   writeln(isAbsPath("C:/foo/.."));
	 
   writeln(isAbsPath("C:\\foo\\.."));
	 	 
   writeln(isAbsPath("."));
