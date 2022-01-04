use RandomNumber7;

var count: sync int = 0;
config const numberOfPoints = 10;

coforall i in 1..numberOfPoints {
    if sqrt(RealRandomNumber()**2 + RealRandomNumber()**2) <= 1 then
    count.writeEF(count.readFE() + 1);
}

writeln("Pi is approximately ", 4.0 * count.readFE() / numberOfPoints);
