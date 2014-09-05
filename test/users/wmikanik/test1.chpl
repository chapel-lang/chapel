use Time;
var t: Timer;

config const verbose : bool = true;

//how big is the set to be covered
config const setCard = 120;
//how many subsets are given
config const subsetCnt = 100;

//indices of elements to be covered
var elIdx_r = 0 .. setCard;

//indices of subsets of elements
var ssIdx_r = 1 .. subsetCnt;

//A2 stores information about elements covered by each subset
//A2(i, j) stores information whether element j is covered by subset i
//the type of elements of A2 could be bool, but we use uint(8) 
var A2 : [ssIdx_r, elIdx_r[1..]] uint(8);

var canUse  : [ssIdx_r] bool;
var covered : [elIdx_r[1 ..]] bool;

//how many elements left uncovered
var elLeft : index ({elIdx_r});

//print information regarding number of locales and number of cores
//writeln ("numLocales ", numLocales);
//writeln ("total Cores ", + reduce ([i in LocaleSpace] Locales[i].numCores));
//writeln ("max parallelism ", + reduce ([i in LocaleSpace] Locales[i].maxTaskPar));
//writeln ("maxThreadsPerLocale ", maxThreadsPerLocale);
writeln("test1");

