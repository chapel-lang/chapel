before cobegin
#include "output-uninit.good.c"
updateVars()
#include "output-assigned.good.c"
cobegin - after updateVars
#include "output-partly-assigned.good.c"
cobegin - the other stmt
#include "output-partly-assigned.good.c"
after cobegin
#include "output-assigned.good.c"
done
