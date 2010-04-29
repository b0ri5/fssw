For Developers (G&E):
  * Compiling
    The build tool used is SCons. 
    
    $ scons.bat (Windows) 
    $ scons.sh (Mac/Unix)
    
  * Style
    The coding style is adopted from
      http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml.
    Use bin/cpplint_all.py (no arguments) to ensure that any changes you have
    made comply with the style.
    
    $ python bin/cpplint_all.py
    
    If anything needs to be done in python, use pychecker at 
      http://pychecker.sourceforge.net/. 
    
  * Testing
    gtest is used for testing. It can be found at
      http://code.google.com/p/googletest/.
    All unittests go in test/ and the unittests are compiled into bin/test/.
    To run all of the tests do
    
    $ python bin/run_unittests.py.
    
  * Directory structure
    bin/        includes scripts and compiled binaries
    include/    headers go here
    scons/      local scons install, called by scons.[sh|bat]
    site_scons/ scons-specific configuration stuff
    src/        source goes here
    test/       unittests