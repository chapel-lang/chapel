fun main() {

  fun nested() {
     var w : int = 3;
     deeplyNested();
          
    fun deeplyNested() {
      var x : int = 4;
      
      fun reallyDeeplyNested() {
        var y : int = 5;
        superNested(); 
        
        fun superNested() {
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
