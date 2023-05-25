class setConst {
  var thisShouldBeSix      =  5;
  var thisShouldBeEleven   = 10;
  var thisShouldBeSixteen  = 15;

}



const const_testSix     : borrowed setConst = (new owned setConst(thisShouldBeSix     =  6)).borrow();
const const_testEleven  : borrowed setConst = (new owned setConst(thisShouldBeEleven  = 11)).borrow();
const const_testSixteen : borrowed setConst = (new owned setConst(thisShouldBeSixteen = 16)).borrow();

writeln("const_testSix.thisShouldBeSix         = ",
         const_testSix.thisShouldBeSix);
writeln("const_testEleven.thisShouldBeEleven   = ",
         const_testEleven.thisShouldBeEleven);
writeln("const_testSixteen.thisShouldBeSixteen = ",
         const_testSixteen.thisShouldBeSixteen);
writeln();






var var_testSix     : borrowed setConst = (new owned setConst(thisShouldBeSix     =  6)).borrow();
var var_testEleven  : borrowed setConst = (new owned setConst(thisShouldBeEleven  = 11)).borrow();
var var_testSixteen : borrowed setConst = (new owned setConst(thisShouldBeSixteen = 16)).borrow();

writeln("var_testSix.thisShouldBeSix         = ",
         var_testSix.thisShouldBeSix);
writeln("var_testEleven.thisShouldBeEleven   = ",
         var_testEleven.thisShouldBeEleven);
writeln("var_testSixteen.thisShouldBeSixteen = ",
         var_testSixteen.thisShouldBeSixteen);
