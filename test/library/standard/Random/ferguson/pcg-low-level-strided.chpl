use Random;
use PCGRandomLib;

proc test_stride_step(stride: int, inc:uint, seed: uint) {
  var start:pcg_setseq_64_xsh_rr_32_rng;
  var a:pcg_setseq_64_xsh_rr_32_rng;
  var b:pcg_setseq_64_xsh_rr_32_rng;
  start.srandom(seed, inc);
  a.srandom(seed, inc);
  b.srandom(seed, inc);

  // Now advance a by stride steps
  for i in 0..#stride {
    a.random(inc);
  }

  // Now advance b by stride steps at once
  var rng_stride: pcg_64_stride;
  rng_stride = b.compute_stride(stride, inc);
  b.stride_step(rng_stride);

  if a.state != b.state {
    writeln("error in test_stride_step(", stride, ", ", inc, ", ", seed, ")");
    writef("a state %xt\n", a.state);
    writef("b state %xt\n", b.state);
    writef("start state %xt\n", start.state);
    writef("rng_stride %xt\n", rng_stride);
    writef("mul %xt inc %xt\n", 6364136223846793005, inc);
  }
  assert(a.state == b.state);
}

for stride in 1..20 {
  test_stride_step(stride, pcg_getvalid_inc(0), 101);
  test_stride_step(stride, pcg_getvalid_inc(1), 1207);
  test_stride_step(stride, pcg_getvalid_inc(2), 13);
  test_stride_step(stride, pcg_getvalid_inc(3), 807);
}
