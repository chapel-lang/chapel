use Socket;
use UnitTest;

proc test_nagle_tcpListener(test: borrowed Test) throws {
  var port:uint(16) = 8811;
  var host = "127.0.0.1";
  var address = ipAddr.ipv4(IPv4Localhost, port);
  var server = listen(address);
  server.setNagle(true);
  test.assertEqual(nagle(server.socketFd), true);
  server.setNagle(false);
  test.assertEqual(nagle(server.socketFd), false);
}

proc test_nagle_tcpConn(test: borrowed Test) throws {
  var port:uint(16) = 8811;
  var host = "127.0.0.1";
  var address = ipAddr.ipv4(IPv4Localhost, port);
  var server = listen(address);
  sync {
    begin {
      var conn = server.accept();
    }
    var conn = connect(host, port);
    conn.setNagle(true);
    test.assertEqual(nagle(conn.socketFd), true);
    conn.setNagle(false);
    test.assertEqual(nagle(conn.socketFd), false);
  }
}

proc test_delayAck_tcpListener(test: borrowed Test) throws {
  var port:uint(16) = 8811;
  var host = "127.0.0.1";
  var address = ipAddr.ipv4(IPv4Localhost, port);
  var server = listen(address);
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
  sync {
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
}

UnitTest.main();
