// param ToLookFor = "??";
module M1 {
    module ToLookFor {
      // Multiple lines on purpose to test error message printing
    }
    module M2 {
        module ToLookFor {}
        module M3 {
            module ToLookFor {}
            module M4 {
                import ToLookFor;
                var x = ToLookFor.x;
            }
        }
    }
}
