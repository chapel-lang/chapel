
/*
A tool written in chapel to compile a chapel program and to demonstrate the use of the 
spawn module and how it is used to create subprocesses within a chapel program. 
*/

use Spawn;

const chapel: string = "chpl";

/* 
Should be included as an argument provided by user:
   - filename.chpl
*/  
proc main(args: [] string) {
  if args.size < 2 {
    writeln("Please input a file to be compiled");
    exit();
  }

  // Main compilation process
  var command: string  = cat_command(args);  // concatenate command 
  writeln(command);                          // print command for user
  spawn_sub(command);                        // spawn command to compile file
}


/*
Concatenates the given arugments to be sent to a subprocess
*/
proc cat_command(args: [] string): string {
  
  var com: string; 
  var file: string = (" " + args[1]);
  var name: string = (" -o " + args[1].strip(".chpl", trailing = true));
  com += chapel + file + name;
  return com;
}


/*
Send command to a subprocess to be executed on command line
*/
proc spawn_sub(toSpawn: string)  {

  // Spawns a shell that takes in and evaluates a string
  var sub = spawnshell(toSpawn);

  // Program waits for compilation to finish
  sub.wait();
}

