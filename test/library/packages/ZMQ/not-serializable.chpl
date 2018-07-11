use ZMQ;

class Bar {
  var a: int;
  var b: string;
}

class Foo {
  var a: int;
  var b: real;
  var c: string;
  var d: borrowed Bar;
}

var foo = new borrowed Foo(42, 13.0, "hello", new borrowed Bar(29, "goodbye"));

var context = new ZMQ.Context();
var socket = context.socket(ZMQ.PUSH);
socket.bind("tcp://*:5555");
socket.send(foo);
