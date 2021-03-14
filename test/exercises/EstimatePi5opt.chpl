use RandomNumber4;

var count = 0;
config const numberOfPoints = 10;

count = + reduce [1..numberOfPoints]
  sqrt(RealRandomNumber()**2 + RealRandomNumber()**2) <= 1;

writeln("Pi is approximately ", 4.0 * count / numberOfPoints);
