config const offset = 1.2;

config const n = 10:int(8);
{
  const D = {-n/2..n};
  writeln(D.translate(offset));
}

