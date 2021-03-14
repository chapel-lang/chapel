use RandomNumber8;

var count: [1..3] int = 0;
config const numberOfPoints = 10;

coforall i in 1..3 {
  var n = numberOfPoints/3 + if numberOfPoints%3 >= i then 1 else 0;
  for j in 1..n do
    if sqrt(RealRandomNumber(i)**2 + RealRandomNumber(i)**2) <= 1 then
      count[i] += 1;
}

writeln("Pi is approximately ", 4.0 * (+ reduce count) / numberOfPoints);
