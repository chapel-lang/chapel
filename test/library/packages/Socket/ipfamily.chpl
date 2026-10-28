use UnitTest;
use Socket;

proc test_available_families(test: borrowed Test) throws {
  test.assertEqual(IPFamily.IPv4:int, AF_INET:int);
  test.assertEqual(IPFamily.IPv6:int, AF_INET6:int);
  test.assertEqual(IPFamily.IPUnspec:int, AF_UNSPEC:int);
}

proc test_families_from_int(test: borrowed Test) throws {
  test.assertEqual(AF_INET:IPFamily, IPFamily.IPv4);
  test.assertEqual(AF_INET6:IPFamily, IPFamily.IPv6);
  test.assertEqual(AF_UNSPEC:IPFamily, IPFamily.IPUnspec);
  try {
    var x = 12345:IPFamily;
    test.assertTrue(false);
  } catch e: IllegalArgumentError {
  }
}

UnitTest.main();
