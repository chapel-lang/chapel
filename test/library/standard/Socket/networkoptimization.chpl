use Socket;
use UnitTest;

proc test_naggle_tcpListener(test: borrowed Test) throws {
  var port:uint(16) = 8811;
  var host = "127.0.0.1";
  var address = ipAddr.ipv4(IPv4Localhost, port);
  var server = listen(address);
  defer server.close();
  server.setNaggle(true);
  test.assertEqual(naggle(server.socketFd), true);
  server.setNaggle(false);
  test.assertEqual(naggle(server.socketFd), false);
}

proc test_naggle_tcpConn(test: borrowed Test) throws {
  var port:uint(16) = 8811;
  var host = "127.0.0.1";
  var address = ipAddr.ipv4(IPv4Localhost, port);
  var server = listen(address);
  defer server.close();
  begin {
    var conn = server.accept();
  }
  var conn = connect(host, port);
  defer conn.close();
  conn.setNaggle(true);
  test.assertEqual(naggle(conn.socketFd), true);
  conn.setNaggle(false);
  test.assertEqual(naggle(conn.socketFd), false);
}

proc test_delayAck_tcpListener(test: borrowed Test) throws {
  var port:uint(16) = 8811;
  var host = "127.0.0.1";
  var address = ipAddr.ipv4(IPv4Localhost, port);
  var server = listen(address);
  defer server.close();
  server.setDelayAck(true);
  test.assertEqual(delayAck(server.socketFd), true);
  server.setDelayAck(false);
  test.assertEqual(delayAck(server.socketFd), false);
}

proc test_delayAck_tcpConn(test: borrowed Test) throws {
  var port:uint(16) = 8811;
  var host = "127.0.0.1";
  var address = ipAddr.ipv4(IPv4Localhost, port);
  var server = listen(address);
  defer server.close();
  begin {
    var conn = server.accept();
  }
  var conn = connect(host, port);
  defer conn.close();
  conn.setDelayAck(true);
  test.assertEqual(delayAck(conn.socketFd), true);
  conn.setDelayAck(false);
  test.assertEqual(delayAck(conn.socketFd), false);
}

UnitTest.main();
