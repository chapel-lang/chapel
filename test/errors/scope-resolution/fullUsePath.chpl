param ToLookFor = "??";
module M1 {
    module ToLookFor {}
    module M2 {
        module ToLookFor {}
        module M3 {
            module ToLookFor {}
            module M4 {
                use ToLookFor;
                var x = ToLookFor.x;
            }
        }
    }
    proc main() {
      // import M4 so that it is checked
      import this.M2.M3.M4;
    }
}
