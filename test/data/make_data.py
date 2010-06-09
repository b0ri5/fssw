#!/usr/bin/env sage

"""
  This file is used to generate whatever data tests might need.
"""

from sage.all import gap

import os
import sys
import optparse

# for test_utils
sys.path.append(os.path.join(os.path.dirname(__file__), '..'))

import test_utils

def main():
  parser = optparse.OptionParser()
  
  parser.add_option('-g', '--minimal-generators', dest='minimal_generators',
                    action="store_true", help='output minimal generators',
                    default=True)
  
  parser.add_option("-n", "--max-order", dest='maximum_order',
                    help='maximum order', default=10, type='int')

  options, args = parser.parse_args()

  # print the order and minimal generators of the groups, separated by newlines
  # <order>
  # <gen1>
  # <gen2>
  #
  # <order2>
  # ...
  if options.minimal_generators:
    for G in test_utils.small_groups(range(1, options.maximum_order + 1)):
      try:
        minimal_gens = G.MinimalGeneratingSet()
      except:
        minimal_gens = G.SmallGeneratingSet()
      
      print G.Order()
      
      if len(minimal_gens) == 0:
        print '()'
      else:  
        for gen in minimal_gens:
          print test_utils.gap_gen_to_string(gen)
        
      print
        
if __name__ == "__main__":
  main()