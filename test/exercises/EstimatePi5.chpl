use RandomNumber4;

var count = 0;
config const numberOfPoints = 10;

for i in 1..numberOfPoints {
  if sqrt(RealRandomNumber()**2 + RealRandomNumber()**2) <= 1 then
    count += 1;
}

writeln("Pi is approximately ", 4.0 * count / numberOfPoints);
