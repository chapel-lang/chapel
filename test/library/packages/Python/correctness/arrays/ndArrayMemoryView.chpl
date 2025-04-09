use Python;

var code = """
import struct

def getBufferObj():
  buf = struct.pack("d"*12, *[1.5*x for x in range(12)])
  return buf
""";

var interp = new Interpreter();

var buf = interp.createModule(code).get('getBufferObj')();

var memview = interp.get('memoryview')(buf).call('cast', 'd', kwargs=["shape"=>(3,4,)]);
writeln(memview.get('shape'));
writeln(memview.get('strides'));
writeln(memview.get('ndim'));
try {
  var pyArr = new PyArray(real, 2, interp, buf.getPyObject(), isOwned=false);
} catch e {
  writeln(e.message());
  writeln("This is expected because the buffer is not writable");
}
