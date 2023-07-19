class setConst {
  var thisShouldBeSix      =  5;
  var thisShouldBeEleven   = 10;
  var thisShouldBeSixteen  = 15;

}
const const_testSixOwn     = new owned setConst(thisShouldBeSix     =  6);
const const_testElevenOwn  = new owned setConst(thisShouldBeEleven  = 11);
const const_testSixteenOwn = new owned setConst(thisShouldBeSixteen = 16);
const const_testSix     : borrowed setConst = const_testSixOwn.borrow();
const const_testEleven  : borrowed setConst = const_testElevenOwn.borrow();
const const_testSixteen : borrowed setConst = const_testSixteenOwn.borrow();

writeln("const_testSix.thisShouldBeSix         = ",
         const_testSix.thisShouldBeSix);
writeln("const_testEleven.thisShouldBeEleven   = ",
         const_testEleven.thisShouldBeEleven);
writeln("const_testSixteen.thisShouldBeSixteen = ",
         const_testSixteen.thisShouldBeSixteen);
writeln();



var var_testSixOwn     = new owned setConst(thisShouldBeSix     =  6);
var var_testElevenOwn  = new owned setConst(thisShouldBeEleven  = 11);
var var_testSixteenOwn = new owned setConst(thisShouldBeSixteen = 16);
var var_testSix     : borrowed setConst = var_testSixOwn.borrow();
var var_testEleven  : borrowed setConst = var_testElevenOwn.borrow();
var var_testSixteen : borrowed setConst = var_testSixteenOwn.borrow();

writeln("var_testSix.thisShouldBeSix         = ",
         var_testSix.thisShouldBeSix);
writeln("var_testEleven.thisShouldBeEleven   = ",
         var_testEleven.thisShouldBeEleven);
writeln("var_testSixteen.thisShouldBeSixteen = ",
         var_testSixteen.thisShouldBeSixteen);
