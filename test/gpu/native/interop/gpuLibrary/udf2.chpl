module Foo {

  extern proc chpl_task_getRequestedSubloc(): int(32);
  extern proc printf(s...);

  export proc add_int32(ref result: [] int(32), const ref a: [] int(32),
                        const ref b: [] int(32)) {
    printf("before on\n");
    sync begin on here.gpus[0] { // `sync begin on` is needed
      printf("subloc : %d\n", chpl_task_getRequestedSubloc()); // prints -2 without `sync begin on`
      const a_gpu = a, b_gpu = b;
      var result_gpu: result.type;
      @assertOnGpu
      foreach i in 0..2 {
        result_gpu[i] = a_gpu[i] + b_gpu[i];
      }
      result = result_gpu;
    }
  }
} // Foo
