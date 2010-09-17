"""
  Copyright 2010 Greg Tener
  Released under the Lesser General Public License v3.
"""

import os
import checks

env = Environment(tools = ['default', 'packaging', 'boris_scons_tools'])

# begin checks
conf = Configure(env, checks.all_checks)

# check for make
env['HAS_MAKE'] = conf.CheckCommand('make --version')

# check for cmake
env['HAS_CMAKE'] = conf.CheckCommand('cmake')

if not env['HAS_CMAKE'] and env['CC'] == 'cl':
  print '  cmake not found (required for building tests), ',
  print 'download it at http://www.cmake.org'

env = conf.Finish()  # get our environment back!

# set up for using multiple configurations, using debug as the default
configs = ARGUMENTS.get('config', 'debug') 
config_config_suffixes = {'debug': '-db', 'release': '', 'profile': '-prof'}
 
for config in configs.split(','):
  print '***Building for %s***' % (config)
  config_env = checks.config(env, config)
  config_suffix = config_config_suffixes[config]
  config_env['CONFIGURATION'] = config
  
  # export the environment and config_suffix to the SConscripts
  Export({'env': config_env, 'config_suffix': config_suffix})
  
  # build the fssw library
  config_env.SConscript('src/SConscript',
          variant_dir='build/%s/src' % (config), duplicate=0)
  
  # build the third-party tools
  config_env['GTEST_INCLUDE'] = '#/third-party/gtest-1.5.0/include'
  config_env['GFLAGS_INCLUDE'] = '#/third-party/gflags-1.3/src'
  config_env.SConscript('third-party/SConscript')
  
  config_env.SConscript('test/SConscript',
                        variant_dir='build/%s/test' % (config), duplicate=0)