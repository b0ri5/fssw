from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

ext = Extension(
        "fssw",
        sources=['fssw.pyx', 'cfssw.pxd'],
        language="C++",
        include_dirs=['../include'],
        libraries=["fssw-db"],
        library_dirs=["../lib"]
)

setup(
    name='Fastest Schreier Sims in the West',
    cmdclass={'build_ext': build_ext},
    ext_modules=[ext]
)
