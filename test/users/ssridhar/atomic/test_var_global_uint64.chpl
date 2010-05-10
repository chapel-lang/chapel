var x: uint(64);
def main() {
  coforall i in 1..10 do
    atomic x = x + 1; 
  writeln(x);
}

