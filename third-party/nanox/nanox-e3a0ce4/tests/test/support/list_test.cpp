/*
<testinfo>
compile_versions="stdlist nanoslist"
test_CXXFLAGS_nanoslist="-DUSE_NANOS_LIST"
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
#include "list.hpp"
#include <list>
#include <algorithm>

using namespace std;
using namespace nanos;
using namespace nanos::ext;

int size;
/*
typedef List<int,int> IntList;
typedef ListNode<int,int> IntNode;
IntList myList;
*/
#ifdef USE_NANOS_LIST
typedef List<int> IntList;
#else
typedef std::list<int> IntList;
#endif

IntList myList;
WD *mainWD;

/*! works only with bf scheduler */
void barrier_code ( void * )
{

   nanos_team_barrier();
/*
   // Test concurrent access to a node and deletion
   if ( mainWD != getMyThreadSafe()->getCurrentWD())
      usleep(50);

   IntNode *node = myList.reference( 50 );

   // Otherwise it has been already deleted by the thread matching mainWD
   if ( node != NULL) {
   	myList.deleteReference( 50 );
        std::cout << "Found!" << std::endl;
   } else {
        std::cout << "Not found!" << std::endl;
   }

   nanos_team_barrier();

   if ( mainWD == getMyThreadSafe()->getCurrentWD()) {
      std::cout << "After second barrier" << std::endl;
   }

   IntList::NodeList deletedNodes;
   if ( mainWD == getMyThreadSafe()->getCurrentWD()) {
      myList.invalidateUnreferencedNodes( deletedNodes );
   }

   nanos_team_barrier();

   node = myList.reference( 50 );

   if ( node != NULL) {
   	myList.deleteReference( 50 );
        std::cout << "Found!" << std::endl;
   } else {
        std::cout << "Not found!" << std::endl;
   }

   nanos_team_barrier();

   if ( mainWD == getMyThreadSafe()->getCurrentWD()) {
      std::cout << "List size: " << myList.getSize() << std::endl;
   }

   // Let's fill the list and leave just one element unreferenced
   // Then one thread tries insistively to retrieve it while the other
   // keeps trying on delenting it

   volatile bool abort = false;

   if ( mainWD == getMyThreadSafe()->getCurrentWD()) {
      std::cout << "deletedNodes List size: " << deletedNodes.size() << std::endl;
      while ( !deletedNodes.empty() ) {
         myList.insert( (deletedNodes.front()).getKey(), *(deletedNodes.front()) );
         deletedNodes.pop_front();
      }
   } 

   nanos_team_barrier();

   if ( mainWD == getMyThreadSafe()->getCurrentWD()) {
      // Get the node and delete the two references it has
      IntNode *node = myList.reference(50);

      if ( node != NULL ) {
         myList.deleteReference( node->getKey() );
         myList.deleteReference( node->getKey() );
      }
      usleep(50);

      std::cout << "One Node unreferenced test, list size: " << deletedNodes.size() << std::endl;
      if ( deletedNodes.size() != 0 ) {
         std::cout << "Error: started with non empty list." << std::endl;
         abort = true;
         exit(1);
      }

      std::cout << "Invalidating..." << std::endl;
      while ( deletedNodes.size() < 1 && !abort ) {
         myList.invalidateUnreferencedNodes( deletedNodes );
      }

      std::cout << "One Node unreferenced test, list size: " << deletedNodes.size() << std::endl;
      for ( IntList::NodeList::iterator it = deletedNodes.begin(); it != deletedNodes.end(); it++ ) {
         std::cout << "Node: " << it->getKey()  << " has: " << (it)->getRefs() << " refs"<< std::endl;
      }

      if ( deletedNodes.size() != 1 ) {
         std::cout << "Error: invalidated more than one node." << std::endl;
         abort = true;
         exit(1);
      } else if ( deletedNodes.begin()->getKey() != 50 || deletedNodes.begin()->getRefs() > 0 ) {
         std::cout << "Error in deleted node: " << deletedNodes.begin()->getKey() << " with " << deletedNodes.begin()->getRefs() << " refs." << std::endl;
         abort = true;
         exit(1);
      }

   } else {
      IntNode *node = myList.reference(50);
      while ( node != NULL && !abort ) {
         myList.deleteReference( 50 );
         node = myList.reference( 50 );
      }
   }

   nanos_team_barrier();
*/
}

int main (int argc, char **argv)
{
   int i;
  
   // Create 100 nodes in the list and leave them all unreferenced
   for ( i = 0; i < 100; i++ ) {
      myList.push_front( i );
   }

   IntList::iterator result = find( myList.begin( ), myList.end( ), 10 );
   if  ( result == myList.end( ) ) {
      cout << "There is no 10 in list myList." << endl;
      abort();
   } else {
      cout << "There is a 10 in list myList and it is "
           << *(result) << "." << endl;
   }

   if ( myList.size() != 100 ) {
      cout << "List size does not match." << endl;
      abort();
   }

   myList.pop_front();

   if ( myList.size() != 99 ) {
      cout << "List size does not match." << endl;
      abort();
   }

   result = find( myList.begin( ), myList.end( ), 99 );
   if ( result != myList.end() ) {
      cout << "Deleted element found!" << endl;
      abort();
   }

/*
   // test clear urenferenced method
   IntList::NodeList deletedNodes;
   myList.invalidateUnreferencedNodes( deletedNodes );

   if ( deletedNodes.size() != 100 ) {
      std::cerr << "Number of deleted nodes: " << deletedNodes.size() << std::endl;
      exit(1);
   }

   // push them back to the list and leave them unreferenced
   for ( i = 0; i < 100; i++ ) {
      IntNode& node = deletedNodes.front();
      node = *myList.insert( node.getKey(), *node );
      myList.deleteReference( node.getKey() );
      deletedNodes.pop_front();
   }

   cout << "before we start with the threads, list size: " << myList.getSize() << std::endl;
*/

   cout << "start threaded tests" << endl;
   //all threads perform a barrier , before the barrier they will freely access the list
   ThreadTeam &team = *getMyThreadSafe()->getTeam();
   size = team.size();
   for ( unsigned i = 1; i < team.size(); i++ ) {
          WD * wd = new WD(new SMPDD(barrier_code));
          wd->tieTo(team[i]);
          sys.submit(*wd);
   }
   usleep(100);

   WD *wd = getMyThreadSafe()->getCurrentWD();
   wd->tieTo(*getMyThreadSafe());

   mainWD = wd;
   barrier_code(NULL);

   cout << "end" << endl;
}
