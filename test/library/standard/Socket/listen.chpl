use UnitTest;
use Socket;

proc test_listen_ipv4(test: borrowed Test) throws {
  var port:uint(16) = 8811;
  var host = "127.0.0.1";
  var address = ipAddr.ipv4(IPv4Localhost, port);
  var server = listen(address);
  defer server.close();

  test.assertEqual(server.addr.port, port);
  test.assertEqual(server.addr.host, host);
  test.assertNotEqual(server.socketFd, -1);
}

proc test_listen_ipv6(test: borrowed Test) throws {
  var port:uint(16) = 8811;
  var host = "::1";
  var address = ipAddr.ipv6(IPv6Localhost, port);
  var server = listen(address);
  defer server.close();

  test.assertEqual(server.addr.port, port);
  test.assertEqual(server.addr.host, host);
  test.assertNotEqual(server.socketFd, -1);
}

UnitTest.main();
