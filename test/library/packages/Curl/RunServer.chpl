use Subprocess;
use OS.POSIX;
use URL;
use Time;
use FileSystem;

config const host = "127.0.0.1";
config const port = "8000";

var server:subprocess(locking=true);

proc startServer() {
  // Run a curl command to check if the server is already up
  var check = spawn(["curl", "http://" + host + ":" + port + "/test.txt"],
                     stdin=pipeStyle.close, stdout=pipeStyle.pipe, stderr=pipeStyle.pipe);
  check.communicate();

  if check.exitCode == 0 {
    // Server already running, so nothing to do.
    return;
  }

  // Start a little HTTP server
  server = spawn(["python3", "-m", "http.server", port, "--bind", host],
                 stdin=pipeStyle.close, stdout=pipeStyle.pipe, stderr=pipeStyle.pipe);

  var ok = false;

  for tries in 1..10 {
    // Run curl command to make sure server is up
    // We could use curl to do the retries, but not all curl versions
    // have the relevant options.
    var check = spawn(["curl", "http://" + host + ":" + port + "/test.txt"],
                       stdin=pipeStyle.close, stdout=pipeStyle.pipe, stderr=pipeStyle.pipe);
    check.communicate();

    if check.exitCode == 0 {
      ok = true;
      break;
    }

    sleep(1);
  }

  if ok == false {
    halt("Unable to bring up http server");
  }

  // Wait on it in a task (to make sure to consume any output)
  begin with (ref server) {
    server.communicate();
  }
}

proc stopServer() {
  if server.running {
    // Kill the little HTTP server
    try! {
      server.sendPosixSignal(SIGINT);
    } catch e:ProcessLookupError {
      // Ignore it already being dead
    }
  }
}
