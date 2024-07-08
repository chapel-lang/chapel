// this is an empty test file to make the test exist
// the interesting stuff happens in `.chpldocopts` and `.prediff`
// this test builds the `.rst` for `Math` and `AutoMath` and then manually
// invokes `make test` in the prediff. The prediff then contains some logic to
// check if the built `Math.txt` has an index that matches the definitions.
// This both ensures that new documented Math functions are added to the index
// and that the `.. includes` from AutoMath add the right information
module usemath {}
