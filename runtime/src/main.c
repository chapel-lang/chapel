#include "chplrt.h"
#include "chpl_rt_utils_static.h"
#include "chpl-init.h"
#include "chplexit.h"
#include "config.h"

int main(int argc, char* argv[]) {

  // Initialize the runtime
  chpl_rt_init(argc, argv);                 

  // Run the main function for this node.
  chpl_task_callMain(chpl_executable_init); 

  // have everyone exit, returning the value returned by the user written main
  // or 0 if it didn't return anything
  chpl_rt_finalize(chpl_gen_main_arg.return_value);

  return 0; // should never get here
}
