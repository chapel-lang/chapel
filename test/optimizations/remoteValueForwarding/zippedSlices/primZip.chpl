/*{*/

  //var a = [1,2,3];
  //var b = [4,5,6];

  //forall (aa,bb) in zip(a,b) {
    //aa = bb;
  //}

  //writeln(a);
//}

//{
  //use BlockDist;

  //var a = newBlockArr(1..10, int);
  ////var a: [1..3] int;
  //var b = newBlockArr(1..10, int);


  ////for (aa,bb) in zip(a,a.domain) {
    ////aa = bb;
  ////}

  ////for aa in a {
    ////aa = 1;
  ////}

  //forall (aa,bb) in zip(a,b) {
    //aa = bb;
  //}

  //writeln(a);
//}

//{

//use BlockDist;
//use CommDiagnostics;

//var a = newBlockArr(1..10, int);
//var b = newBlockArr(1..10, int);

//a = 1;
//b = 2;

//ref aSlice = a[2..9];
//ref bSlice = b[2..9];

//startCommDiagnostics();
//forall (aElem, bElem) in zip(aSlice,bSlice) {
  //aElem = bElem;
//}
//stopCommDiagnostics();
//printCommDiagnosticsTable();

//writeln(a);
//}

//{
//use BlockDist;
//use CommDiagnostics;

//var dom = newBlockDom(1..10);
//var innerDom = dom.expand(-1);

//var a: [dom] int;
//var b: [dom] int;

//a = 1;
//b = 2;

//startCommDiagnostics();
  //forall (aElem, bElem) in zip(a[innerDom],b[innerDom]) {
    //aElem = bElem;
  //}
//stopCommDiagnostics();
//printCommDiagnosticsTable();

//writeln(a);
//}

{
use BlockDist;
use CommDiagnostics;

var dom = newBlockDom(1..10);
var innerDom = dom.expand(-1);

var a: [dom] int;
var b: [innerDom] int;

a = 1;
b = 2;

startCommDiagnostics();
  forall (aElem, bElem) in zip(a[innerDom],b) {
    aElem += bElem;
  }
stopCommDiagnostics();
  writeln(a);
startCommDiagnostics();
  forall (bElem, aElem) in zip(b,a[innerDom]) {
    aElem += bElem;
  }
stopCommDiagnostics();
printCommDiagnosticsTable();

writeln(a);
}
