"""
  Copyright 2010 Greg Tener
  Released under the Lesser General Public License v3.
"""

import os
import checks

env = Environment(tools = ['default', 'packaging', 'boris_scons_tools'])

# add checks for the high resolution timer
# conf = Configure(env, checks.hrtime_checks)

# get the environment for high resolution timing
# hrtime_env = checks.config_hrtime(env, conf)

# conf.Finish()

# tell the sconscripts about the timing environment 
# Export('hrtime_env')


debug_env = checks.config_debug(env)

Export({'env': debug_env, 'libsuffix': '-db'})
debug_env.SConscript('src/SConscript',
          build_dir='build/debug/src', duplicate=0)

conf = Configure(debug_env, checks.all_checks)

debug_env['CONFIGURATION'] = 'debug'
debug_env['GTEST_LIB'] = ''
debug_env['GTEST_INCLUDE'] = ''
has_gtest = False
libgtest = None

if conf.CheckLibWithHeader('gtest', 'gtest/gtest.h', 'C++'):
  has_gtest = True
else:
  print '  gtest not found, checking for cmake to build it'
  
  if conf.CheckExecutable('cmake'):
    # build the copy of gtest in thrd-party
    
    debug_env['GTEST_INCLUDE'] = '#/third-party/gtest-1.5.0/include'
    # the scons will set the lib path
    libgtest = debug_env.SConscript('third-party/SConscript')
    Import('libgtest')
    has_gtest = True
  else:
    print '  cmake not found, download it at http://www.cmake.org/'
    has_gtest = False

if has_gtest:
  Export('libgtest')
  debug_env.SConscript('test/SConscript',
          build_dir='build/debug/test', duplicate=0)

env = conf.Finish()

#Export({'env': release_env, 'libsuffix': ''})
#release_env.SConscript('src/SConscript', build_dir='build/release', duplicate=0)

#Export({'env': profile_env, 'libsuffix': '-prof'})
#profile_env.SConscript('src/SConscript', build_dir='build/profile', duplicate=0)