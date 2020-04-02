
proc copyBtoC(A:[], B:[])
{
  A._value.copyBtoC(B);
}

proc copyCtoB(A:[], B:[])
{
  A._value.copyCtoB(B);
}

proc BlockArr.copyBtoC(B)
{
  coforall loc in Locales do on loc
  {
    const privarr=this.dsiPrivatize(dom.pid);
    const privdom=dom.dsiPrivatize((dom.dist.pid,dom.dsiDims()));
    const privB=B._value.dsiPrivatize(B._value.dom.pid);

    param stridelevels=1;
    var dststrides:[1..#stridelevels] int(32); 
    var srcstrides: [1..#stridelevels] int(32);
    var count: [1..#(stridelevels+1)] int(32);
    var lid=loc.id; 

    var numLocales: int(32)=privdom.dist.targetLocDom.dim(1).size:int(32);
    var n:int(32)=privdom.dist.boundingBox.dim(1).size:int(32);
    var src = privarr.locArr[lid].myElems._value.data;

    dststrides[1]=1;
    srcstrides[1]=numLocales;

    var dststr=dststrides._value.data;
    var srcstr=srcstrides._value.data;
    var cnt=count._value.data;

    //Domain size (n) and first index (arrayini)

    var arrayini:int(32)=dom.dsiLow:int(32);

    //writeln("Domain: ",dom.whole.dims());

    //a,b: first and last global indices in each locale

    var a: int(32)=privdom.locDoms[lid].myBlock.low:int(32); 
    var b: int(32)=privdom.locDoms[lid].myBlock.high:int(32);
    var blksize=b-a+1;

    //writeln("Locale", here.id," : blksize ",blksize," subblock first index  a ",a,

    var t1,t2: real;
    var schunkini: int;
    var chunksize: int(32);
    //Loop of strided chunk sends (chpl_comm_put_strd)
    for k in 0..#numLocales do {

      var dst=(lid+k)%numLocales;

      //Compute the chunk first index in src locale to be send to locale dst.
      if (dst-a%numLocales+arrayini)<0 then schunkini=a+numLocales +(dst-a%numLocales+arrayini);
      else schunkini=a+(dst-a%numLocales+arrayini)%numLocales;

      //Compute chunk size
      if (schunkini+(blksize/numLocales)*numLocales>b) then chunksize=blksize/numLocales;
      else chunksize=blksize/numLocales+1;

      var destr = privB.locArr[dst].myElems._value.data;
      count[1]=1;
      count[2]=chunksize;

      __primitive("chpl_comm_put_strd",
		  __primitive("array_get",destr,
			      privB.locArr[dst].myElems._value.getDataIndex(schunkini)),
		  __primitive("array_get",dststr,dststrides._value.getDataIndex(1)),
		  dst,
		  __primitive("array_get",src,
			      privarr.locArr[lid].myElems._value.getDataIndex(schunkini)),  
		  __primitive("array_get",srcstr,srcstrides._value.getDataIndex(1)),
		  __primitive("array_get",cnt, count._value.getDataIndex(1)),
		  stridelevels);

    } // end for dst
  } // end coforall loc
} // end proc

proc  BlockArr.copyCtoB(B)
{
 
  coforall loc in Locales do on loc
  {
    const privarr=this.dsiPrivatize(dom.pid);
    const privdom=dom.dsiPrivatize((dom.dist.pid,dom.dsiDims()));
    const privB=B._value.dsiPrivatize(B._value.dom.pid);

    param stridelevels=1;
    var dststrides:[1..#stridelevels] int(32);
    var srcstrides: [1..#stridelevels] int(32);
    var count: [1..#(stridelevels+1)] int(32); 
    var lid=loc.id;
    var numLocales: int=privdom.dist.targetLocDom.dim(1).size;
    var n:int(32)=privdom.dist.boundingBox.dim(1).size:int(32);

    var dststr=dststrides._value.data;
    var srcstr=srcstrides._value.data;
    var cnt=count._value.data;

    //On each locale (src) we compute the chunk that goes to each dst
    var num: int;
    var schunkini: int;
    var chunksize: int;
    var a: int(32)=privdom.locDoms[lid].myBlock.low:int(32); 
    var b: int(32)=privdom.locDoms[lid].myBlock.high:int(32);
    num=b-a+1;

    var src = privarr.locArr[lid].myElems._value.data;
    var arrayini:int(32)=dom.dsiLow:int(32);

    var t,t1,t2: real;
    t=0.0;
    //Compute the starting index of the src chunk with destination dst

    for dst in 0..#numLocales do {
      if (dst-a%numLocales+arrayini)<0 then schunkini=a+numLocales+(dst-a%numLocales+arrayini);
      else schunkini=a+(dst-a%numLocales+arrayini)%numLocales;

      //Compute the size of the chunk. The number of elements of src subarray is num
      if (schunkini+(num/numLocales)*numLocales>b) then chunksize=num/numLocales;
      else chunksize=num/numLocales+1;

      var destr = privB.locArr[dst].myElems._value.data;
      dststrides[1]=numLocales:int(32);
      srcstrides[1]=1;
      count[1]=1;
      count[2]=chunksize:int(32);

      __primitive("chpl_comm_get_strd",
		  __primitive("array_get",src,
			      privarr.locArr[lid].myElems._value.getDataIndex(schunkini)),
		  __primitive("array_get",dststr,dststrides._value.getDataIndex(1)),
		  dst,
		  __primitive("array_get",destr,
			      privB.locArr[dst].myElems._value.getDataIndex(schunkini)),
		  __primitive("array_get",srcstr,srcstrides._value.getDataIndex(1)),
		  __primitive("array_get",cnt,count._value.getDataIndex(1)),
		  stridelevels);
    } //end for dst
  }//end for loc
} //end proc copyCtoB

