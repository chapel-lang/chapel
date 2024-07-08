export proc chplFromC(arr: c_ptr(c_int), n:c_int):c_int
{
  var sum:c_int = 0;
  for i in 0..#n {
    arr[i] = i:c_int;
    arr[i] += 1;
    sum += arr[i];
  }
  return sum;
}

extern {
  #include <stdio.h>

  int chplFromC(int *nums, int n);

  void doSomething(void);

  void doSomething(void) {
    int nums[100];
    int i;
    int got;

    for(i = 0; i < 100; i++ ) nums[i] = 0;
    got = chplFromC(nums, 100);

    for(i = 0; i < 100; i++ ) printf("nums[%i] = %i\n", i, nums[i]);
    printf("got %i\n", got);
  }
}

doSomething();


