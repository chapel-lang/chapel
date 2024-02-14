use Socket;
use UnitTest;
use IO;

/* Note: if recv_string or recv_number hang,
   it could be due to issue #22691. In practice, they
   have not been hanging in nightly testing, bit it is possible
   that they could. */

proc recv_string(test: borrowed Test) throws {
  var port:uint(16) = 8811;
  var address = ipAddr.ipv4(IPv4Localhost, port);
  var server = listen(address);
  sync {
    begin {
      var conn = server.accept();
      var writer = conn.writer(locking=false);
      writer.write("hello world\n");
    }

    var conn = connect(address);
    var reader = conn.reader(locking=false);
    var x:bytes;
    reader.readLine(x);
    test.assertEqual(x, b"hello world\n");
  }
}

proc recv_number(test: borrowed Test) throws {
  var port:uint(16) = 8810;
  var address = ipAddr.ipv4(IPv4Localhost, port);
  var server = listen(address);
  sync {
    begin {
      var conn = server.accept();
      var writer = conn.writer(locking=false);
      writer.write(42, " ");
    }

    var conn = connect(address);
    var reader = conn.reader(locking=false);
    var y:int = reader.read(int);
    test.assertEqual(y, 42);
  }
}

/* Commented out to reduce nightly testing noise until #22691 is addressed
proc send_string(test: borrowed Test) throws {
  var port:uint(16) = 6000;
  var address = ipAddr.ipv4(IPv4Localhost, port);
  var server = listen(address);
  sync {
    begin {
      var conn = server.accept();
      var reader = conn.reader();
      var x:bytes;
      reader.readLine(x);
      test.assertEqual(x, b"hello world\n");
    }

    var conn = connect(address);
    var writer = conn.writer();
    writer.write("hello world\n");
  }
}

proc send_number(test: borrowed Test) throws {
  var port:uint(16) = 6000;
  var address = ipAddr.ipv4(IPv4Localhost, port);
  var server = listen(address);
  sync {
    begin {
      var conn = server.accept();
      var reader = conn.reader();
      var y:int = reader.read(int);
      test.assertEqual(y, 42);
    }

    var conn = connect(address);
    var writer = conn.writer();
    writer.write(42, " ");
  }
}
*/

/*
 This test is commented out because test systems don't necessarily
 have network connectivity to www.google.com.
proc send_http(test: borrowed Test) throws {
  var port:uint(16) = 80;
  var host = "www.google.com";

  var conn = connect(host, port, IPFamily.IPv4);
  var writer = conn.writer();

  writer.write(b"GET / HTTP/1.1\n\r\n");
  writer.write(b"Host: google.com\n\r\n");
  writer.flush();

  const reader = conn.reader();
  var b:bytes;
  reader.readLine(b);
  test.assertEqual(b.strip(), b"HTTP/1.1 200 OK");
}
*/

UnitTest.main();
