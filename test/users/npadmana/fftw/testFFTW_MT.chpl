/* An example of using the multithreaded FFTW routines.

   For the simplest usage, all that the user needs to do is to "use FFTW_MT", 
   and then proceed as follows. This automatically initializes the multithreaded
   version of FFTW, and sets plans to use here.maxTaskPar threads.

   If the user would like to change the number of threads used, plan_with_nthreads 
   changes this.

   If the user does not want the module to auto-initialize, compile with 
   autoInitFFTW_MT=false (-sautoInitFFTW_MT=false).

   // TODO : Should give compilation examples below.


*/

/* Note that there is no need to also use FFTW; that is automatically included. 

   This test case just calls the routines in testFFTW; see documentation there.
*/
use FFTW_MT, testFFTW;

config var nthread = 2; // Number of threads


proc main() {
  plan_with_nthreads(nthread);
  testAllDims();
  cleanup_threads();
  cleanup();
}
