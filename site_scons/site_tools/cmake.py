import SCons.Builder
import subprocess as sp
import os
import SCons.Node.FS
import SCons.Node.Python
from SCons.Script import *

"""
  Add some files that CMake creates to the targets
"""
def cmake_emitter(target, source, env):
  target_dir = target[0]
  
  # need to remove the source-tree directory to avoid a circular dependency
  new_targets = []
  
  new_targets.append(target_dir.Dir('CMakeFiles'))
  new_targets.append(target_dir.File('CMakeCache.txt'))
  new_targets.append(target_dir.File('cmake_install.cmake'))

  return new_targets, source

"""
  Make the binary-tree directory if needed and then run cmake.
    target == binary-tree/<cmake_files>
    source == source-tree directory
"""
def cmake_generator(target, source, env, for_signature):
  return [Mkdir('${TARGET.dir}'), env['CMAKECOM']]


"""
  Add whatever files we know to be in the source folder to the sources,
  and add the .lib or .a files to the targets so we know where they will be as
  well.
"""
def cmake_lib_emitter(target, source, env):
  source_dir = source[0]
  new_target = []
  new_source = []
  libnames = list(target)
  
  # if we're using visual studios, add a source for each
  # library specified as a .vcproj
  if env['CMAKE_GENERATOR'].startswith('Visual Studio'):
    for libname in libnames:
      new_source.append(source_dir.File('%s.vcproj' % (libname)))
      
    target_dir = source_dir.Dir(env['CONFIGURATION'])
  
  # add the .lib or .a's to the targets
  for libname in libnames:
    libpath = '%s%s%s' % (env['LIBPREFIX'], libname, env['LIBSUFFIX'])
    new_target.append(target_dir.File(libpath))
  
  return new_target, new_source

"""
  Builds the cmake binary-tree
"""
def cmake_lib_generator(target, source, env, for_signature):
  return ['cmake --build ${SOURCE.dir} --config %s' % env['CONFIGURATION']]


def set_cmake_generator(env):
  # TODO: determine the actual way of doing this...
  # use a cached file so build's don't always take forever since
  # system-information outputs a LOT
  if not os.path.isfile('.cmake_generator'):
    p = sp.Popen(['cmake', '--system-information'], stdout=sp.PIPE, stderr=sp.PIPE)
    firstline = p.stdout.readline()
    f = open('.cmake_generator', 'w')
    f.write(firstline)
    f.close()
    p.terminate()
  else:
    f = open('.cmake_generator')
    firstline = f.read()
    f.close()
    
  prefix = '-- Building for: '
  
  env['CMAKE_GENERATOR'] = firstline[len(prefix):].strip()
  

""" Add builders for CMake"""
def generate(env):
  env.PrependENVPath('PATH', os.environ['PATH'])
  env.Tool('default')
  
  env['CMAKECOM'] = 'cd ${TARGET.dir} && cmake ..' + \
                     os.sep + '${SOURCE.filebase}'
  
  # append the builders for using CMake
  bld = SCons.Builder.Builder(target_factory=SCons.Node.FS.Dir,
                              source_factory=SCons.Node.FS.Dir,
                              emitter = cmake_emitter,
                              generator=cmake_generator)
  env.Append(BUILDERS = {'CMake' : bld})
  
  # find and set the CMAKE_GENERATOR
  set_cmake_generator(env)
  
  # append the builders for using CMakeLib
  bld = SCons.Builder.Builder(source_factory=SCons.Node.FS.Dir,
                              target_factory=SCons.Node.Python.Value,
                              emitter=cmake_lib_emitter,
                              generator=cmake_lib_generator)
  env.Append(BUILDERS = {'CMakeLib' : bld})


""" See if we can call CMake"""
def exists(env):
  return env.Detect('cmake')