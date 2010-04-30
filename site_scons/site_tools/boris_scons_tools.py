from SCons.Script import *

def generate(env, **kw):
  env.PrependENVPath('PATH', os.environ['PATH'])
  env.Tool('default')

  # append the builders for using CMake
  bld = Builder(action = run_cmake, emitter = modify_cmake_targets)
  env.Append(BUILDERS = {'CMake' : bld})

  bld = Builder(action = build_cmake_lib, emitter = modify_build_cmake_lib_targets)
  env.Append(BUILDERS = {'CMakeLib' : bld})
  
def exists(env):
  return True
  
  
"""
  Replace the cmake_project's directory with some standard
  cmake files that will be placed there to know that cmake
  has been executed.
"""
def modify_cmake_targets(target, source, env):
  make_dir = str(target[0])
  target = []
  
  target.append(Dir(os.path.join(make_dir, 'CMakeFiles')))

  for cmake_file in ['cmake_install.cmake', 'CMakeCache.txt']:
    target.append(File(os.path.join(make_dir, cmake_file)))

  return target, source

"""
  Execute cmake.
"""
def run_cmake(target, source, env):
  make_dir = os.path.split(str(target[0]))[0]
  cmake_dir = os.path.split(str(source[0]))[1]
  
  cmake_cmd = 'cd %s && cmake %s' % \
               (make_dir, os.path.join('..', cmake_dir))
  
  rv = Execute(cmake_cmd)
  
  # if the cmake command failed, remove the temporary files
  # so that it looks like it failed
  if rv:
    for cmake_file in target:
      Execute(Delete(cmake_file))
  
  return rv

"""
  Given the directory that cmake populated,
  and the target names desired, modify the targets and
  sources appropriately.
"""
def modify_build_cmake_lib_targets(target, source, env):
  make_dir = str(source[0])
  new_target = []
  new_source = []
  target_dir = ''
  cmake_generator = env['CMAKE_GENERATOR']
   
  if cmake_generator in ['NMake Makefiles',
                         'Unix Makefiles']:
    new_source = [os.path.join(make_dir, 'Makefile')]
    target_dir = make_dir
  elif cmake_generator.startswith('Visual Studio'):
    for lib in target:
      new_source.append(os.path.join(make_dir, '%s.vcproj' % (lib)))
      target_dir = os.path.join(make_dir, 'release')
  
  for lib in target:
      libname = '%s%s%s' % (env['LIBPREFIX'], lib, env['LIBSUFFIX'])
      new_target.append(os.path.join(target_dir, libname))
  
  return new_target, new_source

def build_cmake_lib(target, source, env):
  source_dir, source_name = os.path.split(str(source[0]))
  cmake_generator = env['CMAKE_GENERATOR']
  
  # see if we're using a makefile
  if cmake_generator == 'NMake Makefiles':
    return Execute('cd %s && nmake' % (source_dir))
  elif cmake_generator == 'Unix Makefiles':
    return Execute('cd %s && make' & (source_dir))
  elif cmake_generator.startswith('Visual Studios') and not \
       cmake_generator.startswith('Visual Studios 6'):
    # visual studios targets are a list of vcproj files
    rv = True
    for vcproj in target:
      rv = rv and Execute('cd %s && msbuild %s /p:Configuration=Debug' %
                          (source_dir, vcproj))
    return rv