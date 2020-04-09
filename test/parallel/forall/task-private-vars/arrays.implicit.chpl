use BlockDist;

config const numMessages = 12;
config const asize = 100;

const MessageDom = {1..numMessages};
const MessageSpace = MessageDom dmapped Block(MessageDom);

proc makeArray() {
  var A: [1..asize] int;
  return A;
}

forall msg in MessageSpace
  with (var arr = makeArray())
{
  writeln(arr.domain);
}
