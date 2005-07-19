function main() {

  function nested() {
     var w : integer = 3;
     deeplyNested();
          
    function deeplyNested() {
      var x : integer = 4;
      
      function reallyDeeplyNested() {
        var y : integer = 5;
        superNested(); 
        
        function superNested() {
          var z : float = 6.0;
          writeln(w);
          writeln(x);
          writeln(y);
          writeln(z);
        }
      }
      reallyDeeplyNested();        
    }     
  }
  nested(); 
}
