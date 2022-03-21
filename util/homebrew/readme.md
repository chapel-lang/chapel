This folder contains both the current shipping homebrew formula `chapel-home.rb`, 
and the modified formula that would be used in the next chapel release as 
`chapel-release.rb`.  To test local homebrew builds, the following items are 
needed: 
 - A Chapel tarball 
 - The sha256 for that tarball

 The steps used to do this on a local system are as follows: 
 - Clone Chapel source 
 - Build the tarball via `./util/cron/create_tarball.bash`
 - Run `shasum -a 256 "url for the tarball"`
 - Rename `chapel-release.rb` to `chapel.rb` 
 - Modify the `chapel.rb` file to include the tarball URL and sha256 value
 - Then the following commands can be run (https://github.com/Cray/chapel-private/issues/2866): 
   - `brew install --build-from-source chapel.rb`
 - Finally, the install can be validated by running: 
   - `chpl --version`
   - Compile and run pidigits