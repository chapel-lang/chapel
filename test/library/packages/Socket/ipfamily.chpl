use UnitTest;
use Socket;

const AF_UNSPEC = 0;

proc test_available_families(test: borrowed Test) throws {
  test.assertEqual(IPFamily.IPv4:int, AF_INET:int);
  test.assertEqual(IPFamily.IPv6:int, AF_INET6:int);
  test.assertEqual(IPFamily.IPUnspec:int, AF_UNSPEC:int);
}

UnitTest.main();
