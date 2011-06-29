/*
<testinfo>
test_generator="gens/mixed-generator -a \"--gpus=0\""
</testinfo>
*/

#include "config.hpp"
#include "nanos.h"
#include <iostream>
#include "smpprocessor.hpp"
#include "system.hpp"
#include "threadteam.hpp"
#include <string.h>

using namespace std;
using namespace nanos;
using namespace nanos::ext;

#define BARR_NUM 10

int* counts;
int size;

/*! works only with bf scheduler */
void barrier_code ( void * )
{
       for ( int i = 0; i < BARR_NUM; i++ ) {
              nanos_team_barrier();

              cout << "Before the " << i << " barrier" << endl;
              counts[getMyThreadSafe()->getId()]++;

              nanos_team_barrier();

              cout << "After the " << i << " barrier" << endl;
              if ( counts[ (getMyThreadSafe()->getId()+1)%size ] != i+1 ) {
                 cerr << "Error: the barrier is broken." << std::endl;
                 abort();
              }
       }
}

int main (int argc, char **argv)
{
       cout << "start" << endl;
       //all threads perform a barrier: 
       ThreadTeam &team = *getMyThreadSafe()->getTeam();

       size = team.size();
       counts = new int[team.size()];
       counts[0] = 0;

       for ( unsigned i = 1; i < team.size(); i++ ) {
              counts[i] = 0;
              WD * wd = new WD(new SMPDD(barrier_code));
	      wd->tieTo(team[i]);
              sys.submit(*wd);
       }
       usleep(100);

       WD *wd = getMyThreadSafe()->getCurrentWD();
       wd->tieTo(*getMyThreadSafe());
       barrier_code(NULL);

       cout << "end" << endl;
}
