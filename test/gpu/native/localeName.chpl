use Subprocess;
use IO;

// get the hostname of the host running this test
var sub = spawn(["hostname", ], stdout=pipeStyle.pipe); sub.wait();
var hostname: string;
sub.stdout.readln(hostname);

// store hostname in file for prediff
var outfile = open("localeName.hostname", ioMode.cw);
var writer = outfile.writer();
writer.writeln(hostname);
writer.close();
outfile.close();

// test output
writeln(here.name);
writeln(here.gpus[0]);
on here.gpus[0] {
  writeln(here.name);
}
