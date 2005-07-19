function main() {

  function nested() {
     var w : integer = 3;
     writeln(w);
     deeplyNested();
          
    function deeplyNested() {
      var x : integer = 4;
      writeln(x);
      
      function reallyDeeplyNested() {
        var y : integer = 5;
        writeln(y);
        superNested(); 
        
        function superNested() {
          var z : float = 6.0;
          writeln(z);
        }
      }
      reallyDeeplyNested();        
    }     
  }
  nested(); 
}

