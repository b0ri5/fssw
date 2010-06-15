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
  
  # print the order and minimal generators of the groups, separated by newlines
  # <order>
  # <gen1>
  # <gen2>
  #
  # <order2>
  # ...
  parser.add_option('-g', '--minimal-generators', dest='minimal_generators',
                    action="store_true", help='output minimal generators',
                    default=True)
  
  parser.add_option("-n", "--maximum-n", dest='maximum_n',
                    help='maximum n', default=10, type='int')
  
  # the generator used to make the groups
  parser.add_option("-t", "--group-type", dest="group_type",
                    help='group type', default='small_groups', type='string')

  options, args = parser.parse_args()
  
  # output atlasrep groups 
  if options.group_type == 'atlasrep':
    test_utils.atlasrep_groups(options.maximum_n)
    return 0

  # otherwise output call a generator
  generator_method = getattr(test_utils, options.group_type)
  for G in generator_method(xrange(1, options.maximum_n)):
    if options.minimal_generators:
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