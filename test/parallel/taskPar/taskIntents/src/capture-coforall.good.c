before coforall
#include "output-uninit.good.c"
updateVars()
#include "output-assigned.good.c"
coforall - after updateVars
#include "output-partly-assigned.good.c"
coforall - jjjjj=2
#include "output-partly-assigned.good.c"
after coforall
#include "output-assigned.good.c"
done
