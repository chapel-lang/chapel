function main() {

  function nested() {
     var w : integer = 3;
     deeplyNested();
          
    function deeplyNested() {
      var x : integer = 4;
      writeln(w);
        
      function reallyDeeplyNested() {
        var y : integer = 5;
        writeln(x);
        superNested(); 
        
        function superNested() {
          var z : float = 6.0;
          writeln(y);
          writeln(z);
        }
      }
      reallyDeeplyNested();        
    }     
  }
  nested(); 
}
