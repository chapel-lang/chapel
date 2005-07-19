function main() {         
  nested();
       
  function nested() {
    var x : integer = 3;        
    writeln(x);          
    
    function deeplyNested() {
      var y : integer = 4;
      writeln(y);
    }
    deeplyNested();     
  }
}