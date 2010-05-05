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
    bin/         includes scripts and compiled binaries
    include/     headers go here
    scons/       local scons install, called by scons.[sh|bat]
    site_scons/  scons-specific configuration stuff
    src/         source goes here
    test/        unittests
    third-party/ tools (like gtest)
    
  * Working in IDEs
    Eclipse (needs MercurialEclipse plugin) :
      1) Create new C++ empty Makefile project (using "Other toolchain")
      
      2) Right-click the project in Project Explorer and go to
         Team->Share Project and share it.
         
      3) Right-click the project again and go to Team->Pull and pull it using
         https://fssw.googlecode.com/hg/ as the URL.
         
      4) In properties, specify the build command
         "python ${ProjDirPath}/scons/scons.py".
         Under the behavior tab, remove "all" and set the clean field to "-c".
         
      5) To create a make target for bin/cpplint_all.py and bin/run_unittests.py
         add the targets which have the commands
         $ python ${ProjDirPath}/bin/cpplint_all.py
         $ python ${ProjDirPath}/bin/run_unittests.py