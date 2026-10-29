// Regression test for #22691: with a single worker thread, a task waiting to
// read from a tcpConn must not block that worker, or the peer task never runs.
use Socket, IO, Time;
config param noRegex = false; // dummy config so COMPOPTS-level -snoRegex doesn't break

var server = listen(ipAddr.ipv4(IPv4Localhost, 0));
const address = server.addr;

sync {
  begin {
    // Make sure the client is already waiting in readLine before we accept.
    sleep(1);
    var conn = server.accept();
    var writer = conn.writer(locking=false);
    writer.write("hello world\n");
  }

  var conn = connect(address);
  var reader = conn.reader(locking=false);
  var x: bytes;
  reader.readLine(x);
  write(x);
}
