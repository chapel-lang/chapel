use Subprocess, CTypes;

config const n = 4;
config const verbose = false;

config const bufsz = 0;
extern var qbytes_iobuf_size:c_size_t;

if bufsz > 0 {
  qbytes_iobuf_size = bufsz:c_size_t;
}


var test:[1..n] string;
for i in 1..n {
  test[i] = i:string;
}

writeln("Test 1: stdin=bufferAll stdout=pipe with 1 task");
{
  // note: if pipeStyle.bufferAll is not used in this case,
  // this program will hang if n is large enough (e.g. 10000)
  // since cat will be waiting for this program to consume
  // the output - but we're not consuming the output while we
  // are in the loop writing to cat.
  var sub = spawn(["cat", ], stdin=pipeStyle.bufferAll, stdout=pipeStyle.pipe);
  for x in test {
    sub.stdin.writeln(x);
  }
  // This close call seems to block.
  // That makes sense because it's trying to flush the data,
  // but nothing is consuming the data.
  //sub.stdin.close();
  sub.wait();
  {
    var line:string;
    var i = 1;
    while sub.stdout.readln(line) {
      if verbose then writeln("Got line ", line);
      assert(line == test[i]);
      i += 1;
    }
  }
}


writeln("Test 2: pipe stdin, stdout in 2 tasks");
{
  var sub = spawn(["cat", ], stdin=pipeStyle.pipe, stdout=pipeStyle.pipe);
  cobegin {
    {
      for x in test {
        sub.stdin.writeln(x);
      }
      sub.stdin.close();
    }

    {
      var line:string;
      var i = 1;
      while sub.stdout.readln(line) {
        if verbose then writeln("Got line ", line);
        assert(line == test[i]);
        i += 1;
      }
    }
  }
  sub.wait();
}
