use Spawn;
use URL;
use Time;
use FileSystem;

config const host = "127.0.0.1";
config const port = "8000";

var server:subprocess(kind=iokind.dynamic, locking=true);

proc startServer() {
  writeln("starting server");

  // Start a little HTTP server
  server = spawn(["python3", "-m", "http.server", port, "--bind", host],
                 stdin=CLOSE, stdout=PIPE, stderr=PIPE);

  // Run curl command to make sure server is up
  var check = spawn(["curl",
                     "http://" + host + ":" + port + "/test.txt",
                     "--retry-connrefused",
                     "--retry-delay", "1",
                     "--retry", "20"],
                     stdin=CLOSE, stdout=PIPE, stderr=PIPE);
  check.communicate();

  if check.exit_status != 0 {
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
      server.send_signal(SIGINT);
    } catch e:ProcessLookupError {
      // Ignore it already being dead
    }
  }
}
