TODO
----

1. Fix segfault.
  -- There are now two additional modules, FFTWsegfault.chpl and FFTWsegfaultfix.chpl. 
     If you include these, instead of the regular FFTW module (lines commented at the top
	 of testFFTW.chpl), you should trigger it.
	 FFTWsegfaultfix.chpl appears to fix the segfault with some writelns. ?????
	 
	 I just noted that this could fail with an error from fftw saying that an alloc failed
	 (after appearing to be stuck). Clearly something is not being passed correctly to FFTW.

1. thread support
1. MPI support
1. Pull C prototypes out to allow power users to access things directly (maybe??)

NOTES
-----

1. This is a pretty low-level interface. One can imagine higher level wrappers to do things
more directly for users who don't want to deal with reading the FFTW manual.
1. The testFFTW.chpl routine is a good example of how to use these routines. 
1. The output from testFFTW is a bunch of numbers, all of which should be very small (<< 10^-10,
maybe even smaller)

