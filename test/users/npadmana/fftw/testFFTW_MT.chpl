use FFTW_MT, testFFTW;

config var nthread = 2; // Number of threads


proc main() {
  plan_with_nthreads(nthread);
  testAllDims();
  cleanup_threads();
  cleanup();
}
