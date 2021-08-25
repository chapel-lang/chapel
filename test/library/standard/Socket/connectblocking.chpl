use UnitTest;
use Socket;

proc test_connection_ipv4_dns(test: borrowed Test) throws {
  var port:uint(16) = 8811;
  var host = "127.0.0.1";
  var address = ipAddr.ipv4(IPv4Localhost, port);
  var server = listen(address);
  begin {
    try {
      var conn = server.accept();
      conn.close();
    }
    catch e {
      writeln(e);
    }
    server.close();
  }

  try {
    var conn = connect(host, port);
    test.assertEqual(conn.addr.host, address.host);
    test.assertEqual(conn.addr.port, address.port);
    test.assertEqual(conn.addr.family, address.family);
    conn.close();
  }
  catch e {
    writeln(e);
  }
}

proc test_connection_ipv4_ipaddr(test: borrowed Test) throws {
  var port:uint(16) = 8812;
  var host = "127.0.0.1";
  var address = ipAddr.ipv4(IPv4Localhost, port);
  var server = listen(address);
  begin {
    try {
      var conn = server.accept();
      conn.close();
    }
    catch e {
      writeln(e);
    }
    server.close();
  }

  try {
    var conn = connect(address);
    test.assertEqual(conn.addr.host, address.host);
    test.assertEqual(conn.addr.port, address.port);
    test.assertEqual(conn.addr.family, address.family);
    conn.close();
  }
  catch e {
    writeln(e);
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

proc test_connection_ipv6_dns(test: borrowed Test) throws {
  var port:uint(16) = 8814;
  var host = "::1";
  var address = ipAddr.ipv6(IPv6Localhost, port);
  var server = listen(address);
  begin {
    try {
      var conn = server.accept();
      conn.close();
    }
    catch e {
      writeln(e);
    }
    server.close();
  }

  try {
    var conn = connect(host, port);
    test.assertEqual(conn.addr.host, address.host);
    test.assertEqual(conn.addr.port, address.port);
    test.assertEqual(conn.addr.family, address.family);
    conn.close();
  }
  catch e {
    writeln(e);
  }
}

proc test_connection_ipv6_ipaddr(test: borrowed Test) throws {
  var port:uint(16) = 8815;
  var host = "::1";
  var address = ipAddr.ipv6(IPv6Localhost, port);
  var server = listen(address);
  begin {
    try {
      var conn = server.accept();
      conn.close();
    }
    catch e {
      writeln(e);
    }
    server.close();
  }

  try {
    var conn = connect(address);
    test.assertEqual(conn.addr.host, address.host);
    test.assertEqual(conn.addr.port, address.port);
    test.assertEqual(conn.addr.family, address.family);
    conn.close();
  }
  catch e {
    writeln(e);
  }
}

proc test_fail_ipv6_noserver(test: borrowed Test) throws {
  var port:uint(16) = 8816;
  var host = "::1";
  var address = ipAddr.ipv6(IPv6Localhost, port);

  try {
    var conn = connect(address);
    // making error fail programatically.
    test.assertEqual(-1, 0);
    conn.close();
  }
  catch e {
    test.assertEqual(e.message(), "Connection refused (connect() failed)");
  }
}

UnitTest.main();
