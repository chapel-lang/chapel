use UnitTest;
use Socket;

proc test_send_recv(test: borrowed Test) throws {
  var host = "127.0.0.1";
  var address = ipAddr.create(host, 8111);
  var receiver = new udpSocket();
  bind(receiver, address);
  var sender = new udpSocket();
  var sent = b"hello";
  var received:bytes;
  sync {
    begin {
      var sentBytes = sender.send(sent, address);
      test.assertEqual(sentBytes, sent.size);
    }
    received = receiver.recv(sent.size);
    test.assertEqual(received, sent);
  }
}

proc test_send_recv_from(test: borrowed Test) throws {
  var host = "127.0.0.1";
  var receiverAddress = ipAddr.create(host, 8111);
  var receiver = new udpSocket();
  bind(receiver, receiverAddress);
  var sender = new udpSocket();
  var senderAddress = ipAddr.create(host, 0);
  bind(sender, senderAddress);
  var sent = b"hello";
  sync {
    begin {
      var sentBytes = sender.send(sent, receiverAddress);
      test.assertEqual(sentBytes, sent.size);
    }
    var (received, receivedAddress) = receiver.recvfrom(sent.size);
    test.assertEqual(received, sent);
    test.assertEqual(receivedAddress, sender.addr);
  }
}

proc test_send_recv_timed(test: borrowed Test) throws {
  var host = "127.0.0.1";
  var address = ipAddr.create(host, 7111);
  var receiver = new udpSocket();
  bind(receiver, address);
  var sender = new udpSocket();
  var sent = b"hello";
  var received:bytes;
  sync {
    begin {
      var sentBytes = sender.send(sent, address);
      test.assertEqual(sentBytes, sent.size);
    }
    received = receiver.recv(sent.size, new timeval(2,0));
    test.assertEqual(received, sent);
  }
}

proc test_send_recv_from_timed(test: borrowed Test) throws {
  var host = "127.0.0.1";
  var receiverAddress = ipAddr.create(host, 7222);
  var receiver = new udpSocket();
  bind(receiver, receiverAddress);
  var sender = new udpSocket();
  var senderAddress = ipAddr.create(host, 0);
  bind(sender, senderAddress);
  var sent = b"hello";
  sync {
    begin {
      var sentBytes = sender.send(sent, receiverAddress);
      test.assertEqual(sentBytes, sent.size);
    }
    var (received, receivedAddress) = receiver.recvfrom(sent.size, new timeval(2,0));
    test.assertEqual(received, sent);
    test.assertEqual(receivedAddress, sender.addr);
  }
}

proc test_send_no_recv(test: borrowed Test) throws {
  var host = "127.0.0.1";
  var address = ipAddr.create(host, 7111);
  var sender = new udpSocket();
  var sent = b"hello";
  var sentBytes = sender.send(sent, address);
  test.assertEqual(sentBytes, sent.size);
}

proc test_recv_timeout(test: borrowed Test) throws {
  var host = "127.0.0.1";
  var receiverAddress = ipAddr.create(host, 6111);
  var receiver = new udpSocket();
  bind(receiver, receiverAddress);
  var sent = b"hello";
  try {
    var received = receiver.recv(sent.size, new timeval(2,0));
    test.assertEqual(-1,0);
  } catch e:TimeoutError {
    test.assertEqual(e.message(), "Connection timed out (recv timed out)");
  }
}

proc test_recv_from_timeout(test: borrowed Test) throws {
  var host = "127.0.0.1";
  var receiverAddress = ipAddr.create(host, 6111);
  var receiver = new udpSocket();
  bind(receiver, receiverAddress);
  var sent = b"hello";
  try {
    var (received, receivedAddress) = receiver.recvfrom(sent.size, new timeval(2,0));
    test.assertEqual(-1,0);
  } catch e:TimeoutError {
    test.assertEqual(e.message(), "Connection timed out (recv timed out)");
  }
}

UnitTest.main();
