function main() {         
  nested();
       
  function nested() {
    var x : int = 3;        
    writeln(x);          
    
    function deeplyNested() {
      var y : int = 4;
      writeln(y);
    }
    deeplyNested();     
  }
}
