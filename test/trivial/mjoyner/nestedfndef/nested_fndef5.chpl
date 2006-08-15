def main() {

  def nested() {
     var w : int = 3;
     deeplyNested();
          
    def deeplyNested() {
      var x : int = 4;
      writeln(w);
        
      def reallyDeeplyNested() {
        var y : int = 5;
        writeln(x);
        superNested(); 
        
        def superNested() {
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
