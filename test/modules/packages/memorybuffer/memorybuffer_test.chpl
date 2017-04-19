use memorybuffer;

var x:[1..10] int(32);
var xbuf = new MemoryBuffer([1..10] int(32), x);

var bytes = xbuf.bytes();

var tmpbuf = MemoryBuffer([1..10] int(32)).freeze(x);

writeln(bytes.domain);
writeln(xbuf.get());

writeln(tmpbuf.get());

const res = MemoryBuffer([1..10] int(32)).thaw([1..10] int(32), tmpbuf.bytes());
writeln(res);

const strbuf = MemoryBuffer(string).freeze("spot");
writeln(strbuf.bytes());
const strres= MemoryBuffer(string).thaw(string, strbuf.bytes());
writeln(strres);

const strbuf2 = MemoryBuffer(string).freeze("see spot. see spot run.");
writeln(strbuf2.bytes());
const strres2 = MemoryBuffer(string).thaw(string, strbuf2.bytes());
writeln(strres2);
writeln(strres2.length);

