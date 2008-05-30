var str = "a global string";

class Unallocated {
  var a, b: int;
  def method() {
    a = 1;
    return str;
  }
}

def main {
  var aaa: Unallocated;
  writeln(aaa.method());
}
