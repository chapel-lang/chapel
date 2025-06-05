coforall loc in Locales do on loc {
  var n = 0;
  for gpu in here.gpus do on gpu {
    assert(gpu.gpuId == n);
    n += 1;
  }
}
