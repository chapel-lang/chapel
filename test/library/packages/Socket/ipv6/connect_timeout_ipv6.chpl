use UnitTest;
use Socket;
use OS.POSIX;

proc test_success_ipv6(test: borrowed Test) throws {
  var port:uint(16) = 8822;
  var host = "::1";
  var address = ipAddr.ipv6(IPv6Localhost, port);
  var server = listen(address);
  sync {
    begin {
      var conn = server.accept();
    }
    var conn = connect(address, new struct_timeval(2,0));
    test.assertEqual(conn.addr, address);
  }
}

proc test_success_ipv6_real(test: borrowed Test) throws {
  var port:uint(16) = 8822;
  var host = "::1";
  var address = ipAddr.ipv6(IPv6Localhost, port);
  var server = listen(address);
  sync {
    begin {
      var conn = server.accept();
    }
    var conn = connect(address, 2);
    test.assertEqual(conn.addr, address);
  }
}

proc test_fail_backlog_ipv6(test: borrowed Test) throws {
  var port:uint(16) = 7722;
  var host = "::1";
  var address = ipAddr.ipv6(IPv6Localhost, port);
  var server = listen(address, backlog = 6);

  var failures = 0;
  coforall x in 1..20 with (+ reduce failures) do {
    try {
      var conn = connect(address, new struct_timeval(1, 0));
    }
    catch e {
      failures += 1;
    }
  }

  test.assertNotEqual(failures, 0);
}

proc test_fail_backlog_ipv6_real(test: borrowed Test) throws {
  var port:uint(16) = 7722;
  var host = "::1";
  var address = ipAddr.ipv6(IPv6Localhost, port);
  var server = listen(address, backlog = 6);

  var failures = 0;
  coforall x in 1..20 with (+ reduce failures) do {
    try {
      var conn = connect(address, 1);
    }
    catch e {
      failures += 1;
    }
  }

  test.assertNotEqual(failures, 0);
}

UnitTest.main();
