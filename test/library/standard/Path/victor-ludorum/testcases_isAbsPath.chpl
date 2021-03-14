   use Path;
   use Spawn;

   writeln(isAbsPath("/foo/bar"));

   writeln(isAbsPath("./baz"));
	 
   writeln(isAbsPath("/baz/.."));
	 
   writeln(isAbsPath("baz/"));
	 
   writeln(isAbsPath("//server"));
	 
   writeln(isAbsPath("\\server"));
	 	 	 
   writeln(isAbsPath("."));

   // Check to see if we correctly detect Pythons's abspath to the $CWD as an
   // absolute path (This is to help identify platforms we don't support.)
   writeln(isAbsPath(getPythonsAbspath()));

   proc getPythonsAbspath() {
     var command = "python3 -c 'import os; print(os.getcwd())'";
     var sub = spawnshell(command, stdout=PIPE);

     var absPath:string;
     sub.stdout.readline(absPath);
     absPath = absPath.strip();
     sub.wait();
     return absPath;
   }
