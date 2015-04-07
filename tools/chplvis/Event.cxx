#include "Event.h"


bool operator < (Event &lh, Event &rh)
{
  if (lh.sec < rh.sec) return true;
  if (lh.usec < rh.sec) return true;
  return false;
}

bool operator == (Event &lh, Event &rh)
{
  return lh.sec == rh.sec && lh.usec == rh.usec;
}
