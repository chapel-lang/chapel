use CTypes;

config param funcToTest : string = "invalid";

var myDom : domain(1);

select funcToTest {
  when "c_ptrTo" do c_ptrTo(myDom);
  when "c_ptrToConst" do c_ptrToConst(myDom);
  when "c_addrOf" do c_addrOf(myDom);
  when "c_addrOfConst" do c_addrOfConst(myDom);
  otherwise writeln("invalid function specified");
}
