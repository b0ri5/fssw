#!/usr/bin/env sage

"""
  Run the tests which require sage/gap to supply ground truths. 
"""

from sage.all import gap

import unittest

import os
import sys

# show the local location of the fssw extension module
sys.path.append(os.path.join(os.path.dirname(__file__), '..', 'python'))

import fssw

class FundamentalSchreierTreesTest(unittest.TestCase):
  def test_initialization(self):
    t = fssw.FundamentalSchreierTrees()

  """
    Checks that is_strongly_generated is correct for the first several small
    groups
  """
  def test_is_strongly_generated(self):
    for G in test_utils.small_groups(range(1, 1)):
      t = fssw.FundamentalSchreierTrees()
      stab_chain = G.StabChain()
      strong_gens = stab_chain.StrongGeneratorsStabChain()
      base = stab_chain.BaseStabChain()
      
      for gen in strong_gens:
        t.add_generator(test_utils.gap_gen_to_string(gen))
      
      for b in base:
        t.append_to_base(b)
      
      t.build_trees()
      print 'is_strongly_generated on group of order %s, %d generators,', \
            ' and base %s' % \
            (G.Order(), len(strong_gens), base)
      self.assertTrue(t.is_strongly_generated())
    
  def test_schreier_sims(self):
    for G in test_utils.small_groups(range(60, 200)):
      t = fssw.FundamentalSchreierTrees()
      try:
        minimal_gens = G.MinimalGeneratingSet()
      except:
        minimal_gens = G.SmallGeneratingSet()
      
      for gen in minimal_gens:
        t.add_generator(test_utils.gap_gen_to_string(gen))
      
      print 'schreier_sims on group of order %s, %s' % \
            (G.Order(), list(minimal_gens))
      t.schreier_sims()
      self.assertTrue(t.is_strongly_generated())
    
def main():
  for G in small_groups(range(1, 1)):
    gens =  G.StabChain().StrongGeneratorsStabChain()
    
    for gen in gens:
      print gen, test_utils.gap_gen_to_string(gen)
  
if __name__ == "__main__":
  loader = unittest.TestLoader()
  suite = loader.loadTestsFromTestCase(FundamentalSchreierTreesTest)
  unittest.TextTestRunner(verbosity=2).run(suite)
  #main()
