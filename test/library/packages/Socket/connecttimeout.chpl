use UnitTest;
use Socket;
use OS.POSIX;

proc test_success_ipv4(test: borrowed Test) throws {
  var port:uint(16) = 8811;
  var host = "127.0.0.1";
  var address = ipAddr.ipv4(IPv4Localhost, port);
  var server = listen(address);
  sync {
    begin {
      var conn = server.accept();
    }
    var conn = connect(address, new struct_timeval(2,0));
    test.assertEqual(conn.addr, address);
  }
}

proc test_fail_backlog_ipv4(test: borrowed Test) throws {
  var port:uint(16) = 7711;
  var host = "127.0.0.1";
  var address = ipAddr.ipv4(IPv4Localhost, port);
  var server = listen(address, backlog = 6);

  var failures = 0;
  coforall x in 1..20 with (+ reduce failures) do {
    try {
      var conn = connect(address, new struct_timeval(1, 0));
      conn.close();
    }
    catch e {
      failures += 1;
    }
  }

  test.assertNotEqual(failures, 0);
}

UnitTest.main();
