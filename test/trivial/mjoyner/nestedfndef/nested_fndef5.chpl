fun main() {

  fun nested() {
     var w : int = 3;
     deeplyNested();
          
    fun deeplyNested() {
      var x : int = 4;
      writeln(w);
        
      fun reallyDeeplyNested() {
        var y : int = 5;
        writeln(x);
        superNested(); 
        
        fun superNested() {
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
