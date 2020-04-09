use ZMQ;

config const node: string;

var context: Context;
var socket = context.socket(ZMQ.REQ);
socket.connect(node);

var val = 42;
socket.send(val);
writeln("sent: " + val:string);
val = socket.recv(int);
writeln("received: " + val:string);
