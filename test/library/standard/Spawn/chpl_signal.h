#include <signal.h>

int chpl_SIGRTMAX(void);

// Allow us to call the SIGRTMAX macro from Chapel code
int chpl_SIGRTMAX(void) {
  return SIGRTMAX;
}
