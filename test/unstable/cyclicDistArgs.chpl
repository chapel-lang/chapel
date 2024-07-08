use DSIUtil;
use CyclicDist;

const validDist = new cyclicDist(0, Locales);

const badDist1 = new cyclicDist(0, dataParTasksPerLocale=getDataParTasksPerLocale());
const badDist2 = new cyclicDist(0, dataParIgnoreRunningTasks=getDataParIgnoreRunningTasks());
const badDist3 = new cyclicDist(0, dataParMinGranularity=getDataParMinGranularity());
const badDist4 = new cyclicDist(0, rank=_determineRankFromStartIdx(0));
const badDist5 = new cyclicDist(0, idxType=_determineIdxTypeFromStartIdx(0));
