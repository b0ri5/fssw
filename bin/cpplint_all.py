#!/usr/bin/env python

import sys
import os
import subprocess as sp

src_dirs = [os.path.join('include', 'fssw'), 'src', 'test']

def main():
  bindir = os.path.dirname(__file__)
  cpplint = os.path.join(bindir, 'cpplint.py')
  root = os.path.split(bindir)[0]
  
  for src_dir in src_dirs:
    
    for src_file in os.listdir(os.path.join(root, src_dir) ):
      # only process C++ files and ignore the virtual warning
      if os.path.splitext(src_file)[1] in ['.cc', '.h']:
        cmd = [sys.executable, cpplint]
        # ignore the complaaint about using "long long int"
        if src_file.startswith('FundamentalSchreierTrees') or \
           src_file.startswith('SchreierSims_unittest'):
          cmd.append('--filter=-runtime/int')
        cmd.append(os.path.join(root, src_dir, src_file))

        p = sp.Popen(cmd, stdout=sp.PIPE, stderr=sp.PIPE)
        out, err = p.communicate()
        
        if not err.strip().endswith('Total errors found: 0'):
          print err
  
  print 'done'

if __name__ == "__main__":
  main()