use DSIUtil;
use BlockDist;

const Space = {1..100, 1..100};

const validDist = new blockDist(Space, Locales);

const badDist1 = new blockDist(Space, dataParTasksPerLocale=getDataParTasksPerLocale());
const badDist2 = new blockDist(Space, dataParIgnoreRunningTasks=getDataParIgnoreRunningTasks());
const badDist3 = new blockDist(Space, dataParMinGranularity=getDataParMinGranularity());
const badDist4 = new blockDist(Space, rank=Space.rank);
const badDist5 = new blockDist(Space, idxType=Space.idxType);
const badDist6 = new blockDist(Space, sparseLayoutType=unmanaged DefaultDist);
