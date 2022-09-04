/* 
I'm pleasantly suprised that this test works.  When using the `array_on_device`
memory strategy the copy `RightEightA = LeftEightA;` ends up lowering to
`cuMemcpyDtoD`.

Note that `RightEightA` and `LeftEightA` are references to overlapping regions
of another array who's values iterates from 0 to N (i.e. A = [0, 1, 2, 3, 4,
...., N];) If we did a simple for loop to copy elements like:

  for i in 2..N { RightEightA[i] = LeftEightA[i-2]; }

The resulting backing array A would look like A == [0, 1, 0, 1, 0, 1, ...];

Luckily for a normal array copy Chapel uses `memmove` which is cognizant of how
to handle situations like this. But it's not clear to be that `cuMemcpyDtoD`
would be (I can't find any documentation that would indicate one way or the
other).

I can find this documentation in the runtime API for CUDA for `cudaMemcpyAsync`
(a somewhat similiar function) that states "The memory areas may not overlap"
(though again, it's a completely different function so I'm not sure that says
much): 

https://docs.nvidia.com/cuda/cuda-runtime-api/group__CUDART__MEMORY.html#group__CUDART__MEMORY_1g85073372f776b4c4d5f89f7124b7bf79
*/

config const N = 1024 * 1024;

var HostCopyOfA : [0..N] int;

on here.gpus[0] {
  var A : [0..N] int;

  foreach i in 0..N do A[i] = i;

  ref LeftEightA = A[0..N];
  ref RightEightA = A[2..N];

  RightEightA = LeftEightA;

  HostCopyOfA = A;
}

for i in 2..N {
  if HostCopyOfA[i] != i-2 then writeln("Invalid value found at: ", i);
}
