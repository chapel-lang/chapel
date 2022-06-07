use Socket;
use UnitTest;
use IO;

proc recv_string(test: borrowed Test) throws {
  var port:uint(16) = 8811;
  var address = ipAddr.ipv4(IPv4Localhost, port);
  var server = listen(address);
  sync {
    begin {
      var conn = server.accept();
      var writer = conn.writer();
      writer.write("hello world\n");
    }

    var conn = connect(address);
    var reader = conn.reader();
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
      var writer = conn.writer();
      writer.write(42, " ");
    }

    var conn = connect(address);
    var reader = conn.reader();
    var y:int = reader.read(int);
    test.assertEqual(y, 42);
  }
}

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

UnitTest.main();
