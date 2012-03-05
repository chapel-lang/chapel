proc QuickSort(Data: [?Dom])
	where Dom.rank == 1, Data.eltType implements LessThanComparable
{
	if(Data(mid) < Data(lo)) then
		Data(mid) <=> Data(lo);
}
