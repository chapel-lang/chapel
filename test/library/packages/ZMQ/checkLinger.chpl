use ZMQ;

var context: Context;
var socket = context.socket(ZMQ.REP);
socket.bind("tcp://*:*");
var x = socket.getsockopt(ZMQ.LINGER);
socket.setsockopt(ZMQ.LINGER, x + 1);
var y = socket.getsockopt(ZMQ.LINGER);

writeln(x + 1 == y);
