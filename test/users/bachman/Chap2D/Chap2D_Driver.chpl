use parameters;
use domains;
use arrays;
use Chap2D_Module;
use ARK43;
use IO_Module;
use AllLocalesBarriers;

//use compare_fortran;
use FFT_utils;
use Time;
use IO;

proc main() {

    Initialize();

    for i in (Nt_start+1)..(Nt_start+Nt) {

      TimeStep();

      Diagnostics(i);

    } // Timestepping loop

} // main
