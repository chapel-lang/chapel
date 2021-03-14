use IO;

var f: file;

try {
  try {
    f = open("foo", iomode.cw);
    f.check();
    writeln(f.path);
    f.close();
  } catch e: SystemError {
    writeln("open error:  " + e.details);  
  }
  try {
    f.check();
    writeln("You shouldn't see this");
  } catch e: SystemError {
    writeln("Task failed successfully");  
  }
} catch {
  halt("catchall");
}