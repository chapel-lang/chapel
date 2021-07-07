use Sys;
use CPtr;
use SysCTypes;
use UnitTest;

proc test_addr_string_ipv4(test: borrowed Test) throws {
  var socket_addr:sys_sockaddr_t = new sys_sockaddr_t();
  var address = "127.0.0.1";
  var port:uint(16) = 8000;
  var family = AF_INET;
  socket_addr.set(address.c_str(), port, family);

  var getAddr = socket_addr.numericHost();
  var getPort = socket_addr.port();

  test.assertEqual(getAddr, address);
  test.assertEqual(getPort, port);
  test.assertEqual(socket_addr.family, family);
}

proc test_addr_string_ipv6(test: borrowed Test) throws {
  var socket_addr:sys_sockaddr_t = new sys_sockaddr_t();
  var address = "::1";
  var port:uint(16) = 8000;
  var family = AF_INET6;
  socket_addr.set(address.c_str(), port, family);

  var getAddr = socket_addr.numericHost();
  var getPort = socket_addr.port();

  test.assertEqual(getAddr, address);
  test.assertEqual(getPort, port);
  test.assertEqual(socket_addr.family, family);
}

proc test_addr_ipv4_standard(test: borrowed Test) throws {
  var socket_addr:sys_sockaddr_t = new sys_sockaddr_t();
  var address = "127.0.0.1";
  var port:uint(16) = 8000;
  var family = AF_INET;
  socket_addr.set(INADDR_LOOPBACK, port);

  var getAddr = socket_addr.numericHost();
  var getPort = socket_addr.port();

  test.assertEqual(getAddr, address);
  test.assertEqual(getPort, port);
  test.assertEqual(socket_addr.family, family);
}

proc test_addr_ipv6_standard(test: borrowed Test) throws {
  var socket_addr:sys_sockaddr_t = new sys_sockaddr_t();
  var address = "::1";
  var port:uint(16) = 8000;
  var family = AF_INET6;
  socket_addr.set(in6addr_loopback, port);

  var getAddr = socket_addr.numericHost();
  var getPort = socket_addr.port();

  test.assertEqual(getAddr, address);
  test.assertEqual(getPort, port);
  test.assertEqual(socket_addr.family, family);
}

UnitTest.main();
