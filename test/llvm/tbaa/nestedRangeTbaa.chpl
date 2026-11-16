proc makePair(value: int) {
  return (value..value+2, value+3);
}

var pair = makePair(5);
var copy = pair;
writeln(copy(0).low, " ", copy(0).high, " ", copy(1));

// CHECK: !{{[0-9]+}} = !{!"_tuple_2_range_int64_t_both_one_int64_t_chpl", !{{[0-9]+}}, i64 0, !{{[0-9]+}}, i64 16}
// CHECK-NOT-FAST: !{{[0-9]+}} = !{i64 0, i64 8, !{{[0-9]+}}, i64 8, i64 8, !{{[0-9]+}}, i64 16, i64 8, !{{[0-9]+}}}
