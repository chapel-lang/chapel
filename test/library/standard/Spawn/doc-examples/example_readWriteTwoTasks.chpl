//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
/* START_EXAMPLE */
use Subprocess;

var input = ["a", "b", "c"];

var sub = spawn(["cat"], stdin=pipeStyle.pipe, stdout=pipeStyle.pipe);
cobegin {
  {
    // one task writes data to the subprocess
    for x in input {
      sub.stdin.writeln(x);
    }
    // this close is important; otherwise the other task blocks forever
    sub.stdin.close();
  }

  {
    var line:string;
    while sub.stdout.readln(line) {
      writeln("Got line: ", line);
    }
  }
}
sub.wait();

// prints out:
// Got line: a
// Got line: b
// Got line: c
/* STOP_EXAMPLE */
