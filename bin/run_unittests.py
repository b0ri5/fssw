#!/usr/bin/env python

import os
import optparse

def main():
  parser = optparse.OptionParser()
  
  parser.add_option('-d', '--debug', dest='debug', action="store_true",
          help='run the debug tests',
          default=True)
  
  parser.add_option('-r', '--release', dest='release', action="store_true",
          help='run the release tests',
          default=False)
  
  options, args = parser.parse_args()
  
  dir = os.path.dirname(__file__)
  
  if options.debug:
    for f in os.listdir(os.path.join(dir, 'test') ):
      if not f.endswith('-db'):
        continue
      
      os.system(os.path.join(dir, 'test', f) )
      
if __name__ == "__main__":
  main()