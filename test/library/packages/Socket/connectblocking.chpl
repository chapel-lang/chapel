use UnitTest;
use Socket;

proc test_connection_ipv4_dns(test: borrowed Test) throws {
  var port:uint(16) = 8811;
  var host = "localhost";
  var address = ipAddr.ipv4(IPv4Localhost, port);
  var server = listen(address);
  sync {
    begin {
      var conn = server.accept();
    }
    var conn = connect(host, port, IPFamily.IPv4);
    test.assertEqual(conn.addr, address);
  }
}

proc test_connection_ipv4_ipaddr(test: borrowed Test) throws {
  var port:uint(16) = 8812;
  var address = ipAddr.ipv4(IPv4Localhost, port);
  var server = listen(address);
  sync {
    begin {
      var conn = server.accept();
    }
    var conn = connect(address);
    test.assertEqual(conn.addr, address);
  }
}

proc test_fail_ipv4_noserver(test: borrowed Test) throws {
  var port:uint(16) = 8813;
  var host = "127.0.0.1";
  var address = ipAddr.ipv4(IPv4Localhost, port);

  try {
    var conn = connect(address);
    // making error fail programatically a better API can be fail() function
    test.assertEqual(-1, 0);
    conn.close();
  }
  catch e {
    test.assertEqual(e.message(), "Connection refused (connect() failed)");
  }
}

UnitTest.main();
