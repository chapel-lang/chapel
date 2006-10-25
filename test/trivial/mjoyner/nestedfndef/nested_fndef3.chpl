def main() {

  def nested() {
     var w : int = 3;
     writeln(w);
     deeplyNested();
          
    def deeplyNested() {
      var x : int = 4;
      writeln(x);
      
      def reallyDeeplyNested() {
        var y : int = 5;
        writeln(y);
        superNested(); 
        
        def superNested() {
          var z : real = 6.0;
          writeln(z);
        }
      }
      reallyDeeplyNested();        
    }     
  }
  nested(); 
}

