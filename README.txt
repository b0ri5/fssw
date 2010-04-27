For Developers (G&E):
  - Style
    The coding style is adopted from
      http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml.
    Use bin/cpplint_all.py (no arguments) to ensure that any changes you have
    made comply with the style. 
    
  - Testing
    gtest is used for testing. It can be found at
      http://code.google.com/p/googletest/.
    All unittests go in test/ and the unittests are compiled into bin/test/.
    To run all of the tests use bin/run_unittests.py. 
    
  - Anything else?