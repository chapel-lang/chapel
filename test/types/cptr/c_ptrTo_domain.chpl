use CTypes;

config param funcToTest : string = "invalid";

var myDom : domain(1);

select funcToTest {
  when "c_ptrTo" do c_ptrTo(myDom);
  when "c_ptrToConst" do c_ptrToConst(myDom);
  otherwise writeln("invalid function specified");
}
