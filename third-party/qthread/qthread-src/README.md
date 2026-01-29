QTHREADS
========

The Qthreads API is designed to make using large numbers of threads convenient and easy.
The Qthreads API also provides access to full/empty-bit (FEB) semantics,
 where every word of memory can be marked either full or empty,
 and a thread can wait for any word to attain either state.

Qthreads is essentially a library for spawning and controlling stackful coroutines:
 threads with small (4-8k) stacks.
The exposed user API resembles OS threads,
 however the threads are entirely in user-space and use their locked/unlocked status as part of their scheduling.

The library's metaphor is that there are many Qthreads and several "shepherds".
Shepherds generally map to specific processors or memory regions,
 but this is not an explicit part of the API.
Qthreads are assigned to specific shepherds and are only allowed to migrate
 when running on a scheduler that supports work stealing
 or when migration is explicitly triggered via user APIs.

The API includes utility functions for making threaded loops, sorting, and similar operations convenient.

There is also an experimental interface for qthreads using C++26 senders under development at [https://github.com/sandialabs/qthreads-stdexec](https://github.com/sandialabs/qthreads-stdexec).

## Collaboration

Need help or interested in finding out more? Join us on our Slack channel: https://join.slack.com/t/Qthreads/signup

## Compatibility

Millions of Qthreads should run fine even on a machine with a modest amount of RAM.
Generally the primary limit to the number of threads that can be spawned is memory use.

This library has been tested, and runs well, on 64-bit ARM and X-86 machines.
32-bit versions of those architectures as well as PowerPC-based architectures may also work.

This library is compatible with most Linux variants as well as OSX.
There is some preliminary support for BSD operating systems.
Windows is not currently supported.

## Building Qthreads

CMake 3.23 or later is required for building from source.
Hwloc is also highly recommended.

The following compilers are supported and tested regularly:
- gcc 9 or later
- clang 14 or later
- icc (last supported release)
- icx 2024 or later
- aocc 4.2 or later
- acfl 24.10
- Apple clang 15 or later
- nvc 24.9

To configure and build from source you can run (in the source directory):

```
mkdir build
cd build
cmake ..
make -j
```

