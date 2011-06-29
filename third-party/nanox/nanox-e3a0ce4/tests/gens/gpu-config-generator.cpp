#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <iostream>

int main()
{
   int i;
   int MAX_GPUS = 2;
   std::ostringstream exec_versions;
   exec_versions << "exec_versions=\"";
   std::ostringstream versions_env;
   char *max_gpus = getenv("NX_TEST_MAX_GPUS");
   if ( max_gpus != NULL) MAX_GPUS = atoi(max_gpus);

//   printf("exec_versions=\"normal cilk\"\n");

   for ( i=1; i<=MAX_GPUS; i++ ) {
      exec_versions << "normal" << i << " ";
      exec_versions << "cilk" << i << " ";
      versions_env << "test_ENV_normal" << i << "=\"NX_ARGS='--gpus=" << i <<"'\"" << std::endl;
      versions_env << "test_ENV_cilk" << i << "=\"NX_ARGS='--schedule=cilk --gpus=" << i << "'\"" << std::endl;
//      printf("test_ENV_normal=\"NX_ARGS=''\"\n");
//      printf("test_ENV_cilk=\"NX_ARGS='--schedule cilk'\"\n");
   }
   exec_versions << "\"" << std::endl;

   std::cout << exec_versions.str();
   std::cout << versions_env.str();
}

