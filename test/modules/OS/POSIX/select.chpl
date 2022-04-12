use OS.POSIX;

// This test works by writing to and reading from, in separate tasks,
// many pipes simultaneously.  The select_posix() calls are used to
// decide which pipe(s) to operate upon, based on whether we have
// space for writing and/or data available for reading.

config const numPipes = 2;
config const numXfers = numPipes * 4 + 1;

var fdsRead: [0..<numPipes] c_int;
var fdsWrite: [0..<numPipes] c_int;

for i in 0..<numPipes {
  var fildes:2*c_int;
  writeln(pipe(c_ptrTo(fildes)) == 0);
  fdsRead[i] = fildes(0);
  fdsWrite[i] = fildes(1);
}

cobegin {
  beReader(fdsRead);
  beWriter(fdsWrite);
}

proc beReader(fds:[] c_int) {
  // The reader expects to receive each value in 0..<numXfers exactly
  // once, from some fd.
  var numReceived = 0;
  var sawVal:[0..<numXfers] bool;
  var fdset:fd_set;
  var timeout:struct_timeval;
  while numReceived < numXfers {
    const (fdMin, fdMax) = fdsetSetup(c_ptrTo(fdset), fds);
    timeout.tv_sec = 2:time_t;
    timeout.tv_usec = 0:suseconds_t;
    const numFdsReady = select_posix((fdMax + 1):c_int,
                                     c_ptrTo(fdset), c_nil, c_nil,
                                     c_ptrTo(timeout));
    if numFdsReady < 0 {
      writeln('beReader() select(): ', strerror(errno):string);
      break;
    } else if numFdsReady == 0 {
      writeln('beReader() select(): no fds ready?!');
      break;
    } else {
      for fd in fdMin..fdMax {
        if FD_ISSET(fd:c_int, c_ptrTo(fdset)) {
          var val:int;
          writeln(POSIX.read(fd:c_int, c_ptrTo(val),
                             numBytes(val.type):c_size_t));
          writeln('read ', val, ' from fd ', fd);
          numReceived += 1;
          if numReceived > numXfers {
            writeln('too many xfers?!');
            break;
          } else if !sawVal.dim(0).contains(val) then {
            writeln(val, ' is out of range?!');
            break;
          } else if sawVal[val] then {
            writeln(val, ' was previously seen?!');
            break;
          } else {
            sawVal[val] = true;
          }
        }
      }
    }
  }

  // Run a final select(), which should time out.
  const (fdMin, fdMax) = fdsetSetup(c_ptrTo(fdset), fds);
  timeout.tv_sec = 0:time_t;
  timeout.tv_usec = 500_000:suseconds_t;
  writeln(select_posix((fdMax + 1):c_int, c_ptrTo(fdset), c_nil, c_nil,
                       c_ptrTo(timeout)) == 0);
}

proc beWriter(fds:[] c_int) {
  // The writer sends each value in 0..<numXfers exactly once, to
  // some fd.
  var fdset:fd_set;
  var timeout:struct_timeval;
  var numSent = 0;
  while numSent < numXfers {
    const (fdMin, fdMax) = fdsetSetup(c_ptrTo(fdset), fds);
    timeout.tv_sec = 2:time_t;
    timeout.tv_usec = 0:suseconds_t;
    const numFdsReady = select_posix((fdMax + 1):c_int,
                                     c_nil, c_ptrTo(fdset), c_nil,
                                     c_ptrTo(timeout));
    if numFdsReady < 0 {
      writeln('beWriter() select(): ', strerror(errno):string);
      break;
    } else if numFdsReady == 0 {
      writeln('beWriter() select(): no fds ready?!');
      break;
    } else {
      for fd in fdMin..fdMax {
        if FD_ISSET(fd:c_int, c_ptrTo(fdset)) {
          writeln('write ', numSent, ' to fd ', fd);
          writeln(POSIX.write(fd:c_int, c_ptrTo(numSent),
                              numBytes(numSent.type):c_size_t));
          numSent += 1;
          if numSent >= numXfers {
            break;
          }
        }
      }
    }
  }
}

proc fdsetSetup(pFdset:c_ptr(fd_set), fds:[] c_int):2*int {
  var fdMin = max(int);
  var fdMax = min(int);
  FD_ZERO(pFdset);
  for fd in fds {
    assert(fd >= 0 && fd < FD_SETSIZE);
    FD_SET(fd, pFdset);
    if fd < fdMin then fdMin = fd;
    if fd > fdMax then fdMax = fd;
  }
  return (fdMin, fdMax);
}
