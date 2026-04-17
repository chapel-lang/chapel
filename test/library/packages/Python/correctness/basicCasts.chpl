use Python;

var pyCode = """
def getInt():
  return 11

def getBool():
  return True

def getFloat():
  return 6.17

def getString():
  return "a str"

def getBytes():
  return b"a byte str"
""";

proc main() {

  var interp = new Interpreter();

  var pyCodeModule = interp.createModule(pyCode);

  var getIntFunc = pyCodeModule.get('getInt');

  writeln("checking ints");
  var i = getIntFunc(owned Value);
  writeln(i);
  var iAsDefaultInt = i: int;
  writeln(iAsDefaultInt);
  var iAsInt64 = i: int(64);
  writeln(iAsInt64);
  var iAsInt32 = i: int(32);
  writeln(iAsInt32);
  var iAsInt16 = i: int(16);
  writeln(iAsInt16);
  var iAsInt8 = i: int(8);
  writeln(iAsInt8);

  writeln("checking uints");
  var iAsDefaultUint = i: uint;
  writeln(iAsDefaultUint);
  var iAsUint64 = i: uint(64);
  writeln(iAsUint64);
  var iAsUint32 = i: uint(32);
  writeln(iAsUint32);
  var iAsUint16 = i: uint(16);
  writeln(iAsUint16);
  var iAsUint8 = i: uint(8);
  writeln(iAsUint8);

  writeln("checking reals");
  var getRealFunc = pyCodeModule.get('getFloat');
  var r = getRealFunc(owned Value);
  var rAsDefaultReal = r: real;
  writeln(isClose(rAsDefaultReal, 6.17));
  var rAsReal64 = r: real(64);
  writeln(isClose(rAsReal64, 6.17));
  var rAsReal32 = r: real(32);
  writeln(isClose(rAsReal32, 6.17));

  writeln("checking bools");
  var getBoolFunc = pyCodeModule.get('getBool');
  var b = getBoolFunc(owned Value);
  writeln(b);
  var bAsChapelBool = b: bool;
  writeln(bAsChapelBool);

  writeln("checking strings");
  var getStrFunc = pyCodeModule.get('getString');
  var s = getStrFunc(owned Value);
  writeln(s);
  var sAsChapelStr = s: string;
  writeln(sAsChapelStr);

  writeln("checking bytes");
  var getBytesFunc = pyCodeModule.get('getBytes');
  var byt = getBytesFunc(owned Value);
  writeln(byt);
  var bytAsBytes = byt: bytes;
  writeln(bytAsBytes);
}
