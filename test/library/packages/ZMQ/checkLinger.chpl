use ZMQ;

var context: Context;
var socket = context.socket(ZMQ.REP);
socket.bind("tcp://*:*");
var x = socket.getLinger();
socket.setLinger(x + 1);
var y = socket.getLinger();

writeln(x + 1 == y);
