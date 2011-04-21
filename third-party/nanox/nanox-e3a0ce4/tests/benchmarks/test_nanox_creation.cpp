/*
<testinfo>
test_generator=gens/nanox-creation-generator
exec_versions="1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24"
for ever in $exec_versions;
do
  declare test_ENV_$ever="NX_PES='$ever'"
done
</testinfo>
*/

#include "config.hpp"
#include "nanos.h"
#include <iostream>
#include "smpprocessor.hpp"
#include "system.hpp"
#include "threadteam.hpp"
#include <string.h>
#include <sys/time.h>

using namespace std;
using namespace nanos;
using namespace nanos::ext;

double get_wtime( void )
{

   struct timeval ts;
   double t;
   int err;

   err = gettimeofday( &ts, NULL );
   t = ( double ) ( ts.tv_sec )  + ( double ) ts.tv_usec * 1.0e-6;

   return t;
}

void empty( void * )
{
//       cout << "Empty task ran" << endl;
}

double *times;

/*! works only with bf scheduler */
void barrier_code ( void * )
{
       double start,end;
       unsigned int me = getMyThreadSafe()->getTeamId();
       nanos_team_barrier();

       start = get_wtime();

       for (int i=0; i < 1000000; i++) {
          WD * wd = new WD(new SMPDD(empty));
          sys.submit(*wd);
       }

       end = get_wtime();
       times[me] = (end - start)/(1000000)*1000;

       nanos_team_barrier();
}

int main (int argc, char **argv)
{
       double start,end;
       unsigned int me;

       cout << "start" << endl;
       //all threads perform a barrier: 
       ThreadTeam &team = *getMyThreadSafe()->getTeam();

       me = getMyThreadSafe()->getTeamId();
       times = new double[team.size()];

       for ( unsigned i = 1; i < team.size(); i++ ) {
              WD * wd = new WD(new SMPDD(barrier_code));
	      wd->tieTo(team[i]);
              sys.submit(*wd);
       }
       usleep(100);

       WD *wd = getMyThreadSafe()->getCurrentWD();
       wd->tieTo(*getMyThreadSafe());

       nanos_team_barrier();

       cout << "Start adding tasks" << endl;
       start = get_wtime();

       for (int i=0; i < 1000000; i++) {
          WD * wd = new WD(new SMPDD(empty));
          sys.submit(*wd);
       }

       end = get_wtime();
       times[me] = (end - start)/(1000000)*1000;
   
       nanos_team_barrier();

       for ( unsigned int i=1; i< team.size(); i++ ) {
          times[0]+=times[i];
       }
       times[0] = times[0]/team.size();

       printf("Time in ms\tTeam size\n");
       printf( "%f\t%d\n",  times[0], team.size() );

       cout << "end" << endl;
}
