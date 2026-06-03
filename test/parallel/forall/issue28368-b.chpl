var A = [0,1,2,3,4,5,6,7,8,9];
var total = 0;
forall i in 0..#500 with (+ reduce total) {
  var totalInner = 0;
  forall j in [1,2,3] with (+ reduce totalInner) {
    totalInner reduce= (+ reduce (A + 1)) * j;
  }
  total reduce= totalInner * i;
}
// Should be: 550 (= + reduce 1..10) * 6 (= + reduce [1,2,3]) * 124750 (= + reduce 0..#500)
//            = 41167500
writeln("total == 41167500: ", total == 41167500);
