


// for binary output
extern proc putchar(i : int) : int;

config const size: int(64) = 200;
config const iters = 50;
  
const sizeRange = 0..#size;
const iterRange = 0..#iters;

proc main() {
  
  var bit : int = 128;
  var byte_acc : int = 0;
  
  writeln("P4");
  writeln(size, " ", size);
  
  var fy, z, c : complex;
  
  var fsize : int = size : int;
  
  for y in sizeRange {
    fy = 2i * y / size - 1i;
  
    for x in sizeRange {
      z = 0i;
      c = 2.0 * x / size - 1.5 + fy;

      var broken : bool = false;
      for i in iterRange {
        z = z * z + c;
        if abs(z) >= 2.0 then {
          broken = true;
          break;
        }
      }
      if !broken then
        byte_acc += bit;

      if bit > 1
        then bit >>= 1;
      else {
        putchar(byte_acc);
        bit = 128;
        byte_acc = 0;
      }
    }

    if bit != 128 then {
      putchar(byte_acc);
      bit = 128;
      byte_acc = 0;
    }
  
  }
  
}
