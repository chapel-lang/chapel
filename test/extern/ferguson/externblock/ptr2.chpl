extern {
  #include <stdio.h>

  void printit(int n, int* arr);
  void printit(int n, int* arr)
  {
    int i;
    for( i = 0; i < n; i++ ) printf("arr[%i] = %i\n", i, arr[i]);
  }
}

var myarr = c_calloc(c_int, 10);
for i in 0..#10 {
  myarr[i] = i:c_int;
}
printit(10, myarr);
c_free(myarr);
