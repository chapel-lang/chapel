
This folder contains a copy of the shipping homebrew formula `chapel-release.rb`, 
and the modified formula that would be used in the next chapel release as 
`chapel-main.rb`.  These are stored here to allow the Chapel team test upcoming changes without 
impacting the Homebrew core project formula: (https://github.com/Homebrew/homebrew-core/blob/master/Formula/chapel.rb).   
## Tracking changes
The homebrew team will update formulas under some circumstances and this mostly when a formula
has not specified a version for a dependency.  For example, the `chapel.rb` formula refers to the 
most recent version of `llvm` via this line `depends_on "llvm"`.  If the `llvm` project updates their 
formula, the homebrew team will modify dependencies by specifying a version such as `depends_on "llvm@13"`.
They will also indicate the formula changed via the addtion of a revision value such as `revision 1` as seen
in the 1.25.1 update:  https://github.com/Homebrew/homebrew-core/blob/c63259777f760d8cebdce758c3e91778e58b55a5/Formula/chapel.rb. 

## How to release the formula 
The homebrew docs explain how to issue a pull request here:  
https://docs.brew.sh/How-To-Open-a-Homebrew-Pull-Request.  Before issuing the request itself, 
the following commands should be run before submitting the PR: 
```
brew tests
brew install --build-from-source <CHANGED_FORMULA>
brew test <CHANGED_FORMULA>
brew audit --strict --online <CHANGED_FORMULA>`
```

These steps confirm that the formula will build within your local homebrew environment and 
that the formula itself is properly formatted.  No trailing whitespace or extraneous characters
thhat would cause the homebrew CI process to fail. 

### Linuxbrew testing 
The Ubuntu CI is run on a docker container per the instructions on their contribution documentation: 
https://github.com/Homebrew/homebrew-core/blob/master/CONTRIBUTING.md

The container is executed using the following instructions: 
```
docker run --interactive --tty --rm --pull always homebrew/ubuntu16.04:latest /bin/bash
```

Formulas can be copied to the container using the following command: 
```
docker cp chapel.rb <container name or container ID>:/home/linuxbrew/chapel.rb
```

## How to test formula changes
To test local homebrew builds, the following items are needed: 
 - A Chapel tarball 
 - The sha256 for that tarball

 The steps used to do this on a local system are as follows: 
 - Clone Chapel source 
 - Build the tarball via `./util/cron/create_tarball.bash`
 - Run `shasum -a 256 "url for the tarball"`
 - Verify the permissions on the tarball - it should be set to `644` if you are attempting to use a web location. 
 - Rename `chapel-main.rb` to `chapel.rb` 
 - Modify the `chapel.rb` file to include the tarball URL (this can be a network location or a local file) and sha256 value
 - Then the following commands can be run (https://github.com/Cray/chapel-private/issues/2866): 
   - `brew install --build-from-source chapel.rb`
 - Finally, the install can be validated by running: 
   - `chpl --version`
   - Compile and run pidigits