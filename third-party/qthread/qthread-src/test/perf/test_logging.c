#include<qthread/logging.h>
#define LVL1 1
#define LVL2 0
#define LVL3 1

int main(){
  qtlog(LOGERR, "This is LOGERR");
  qtlog(LOGWARN, "This is LOGWARN");
  qtlogargs(LOGERR, "This is LOGERR with arg of 5 in parentheses: (%d)", 5);
  qtlog(LVL1, "You should see this as LVL1");
  qtlogargs(LVL1, "Again with LVL1, with args 1,2,\"foo\" -> (%d, %d, %s)", 1, 2, "foo");
  qtlog(LVL2, "You should NOT see this message.");
  qtlog(LVL3, "You should see this as LVL3");
  return 0;
}
