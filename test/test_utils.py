#!/usr/bin/env sage

"""
  Code common to at least two tests/data generators.
"""

import StringIO
import re
import sys

from sage.all import gap

def small_groups(orders):
  for n in orders:
    # small groups library returns polycyclic groups
    for pcgroup in gap.AllSmallGroups(n):
      # find an isomorphism into a permutation group
      i = gap.IsomorphismPermGroup(pcgroup)
      gap_G = gap.Image(i, pcgroup)
      # reduce the degree of the representation if we can
      i_small = gap.SmallerDegreePermutationRepresentation(gap_G)
      gap_G_small = gap.Image(i_small, gap_G)
      # yield this small representation
      yield gap_G_small
      
def symmetric_groups(ns):
  for n in ns:
    yield gap.SymmetricGroup(gap.IsPermGroup, n)

def alternating_groups(ns):
  for n in ns:
    yield gap.AlternatingGroup(gap.IsPermGroup, n)
    
def cyclic_groups(ns):
  for n in ns:
    yield gap.CyclicGroup(gap.IsPermGroup, n)
    
def dihedral_groups(ns):
  for n in ns:
    yield gap.DihedralGroup(gap.IsPermGroup, 2 * (n + 1))

def mathieu_groups(ns):
  for n in [9, 10, 11, 12, 21, 22, 23, 24]:
    if n in ns:
      yield gap.MathieuGroup(gap.IsPermGroup, n)
      
def suzuki_groups(ns):
  for n in ns:
    yield gap.SuzukiGroup(gap.IsPermGroup, 2**(2 * n + 1))
    
def ree_groups(ns):
  for n in ns:
    ree = gap.ReeGroup(3**(2 * n + 1))
    i = gap.IsomorphismPermGroup(ree)
    gap_G = gap.Image(i, ree)
    yield gap_G

def gap_gen_to_string(g):
  seen = set()
  s = ""
  f = StringIO.StringIO()
  
  for i in xrange(gap.SmallestMovedPointPerm(g),
                  gap.LargestMovedPointPerm(g) + 1):
    i = gap(i)
    if int(i) in seen:
      continue
    
    a = i
    seen.add(int(a))  # need to convert to an int instead of a gap int
    cycle = [a]
    
    while i != a**g:
      a = a**g
      seen.add(int(a))
      cycle.append(a)
    
    if len(cycle) > 1:
      f.write("(%s)" % ' '.join([str(a) for a in cycle]))

  return f.getvalue()


def atlasrep_groups(n):
  if not gap.LoadPackage('"atlasrep"'):
    print 'Error loading atlasrep'
    sys.exit(1);
  
  max_degree = n
  
  if not n in [10**i for i in [4, 5, 6, 7]]:
    sys.stderr.write('use n = 10**[4|5|6|7] for atlasrep\n')
    sys.exit(1)
    
  min_degree = max_degree / 10 + 1
  
  atlas_info = gap.DisplayAtlasInfo()
  
  for line in atlas_info.split('\n')[2:]:
    group_name = line[:line.index(' ')]

    # find a minimal permutation group representation
    group_info = gap.DisplayAtlasInfo('"%s"' % group_name, gap.IsPermGroup,
                                      gap.true, gap.NrMovedPoints, '"minimal"')
    
    # None is returned if no such permutation representation exists
    if not group_info:
      continue
    
    # group_info looks like:
    # Representations for G = Ru:    (all refer to std. generators 1)
    # ---------------------------
    # 1: G <= Sym(4060)
    index_string = group_info.split('\n')[2]
    index = int(index_string[:index_string.index(':')])
    degree = int(re.findall(r'Sym\((\d+)[a-z]?\)', index_string)[0])

    # ignore groups with excessively large degrees
    if degree < min_degree or degree > max_degree:
      sys.stderr.write('skipping %s with degree %d ' % (group_name, degree) + 
                       'because it is not within [%d, %d]\n' %
                       (min_degree, max_degree))
      continue
      g
    sys.stderr.write('working on %s with degree %d\n' % (group_name, degree))
    
    generator_record = gap.AtlasGenerators('"%s"' % group_name, index)
    print '# %s' % group_name
    order = gap.get_record_element(generator_record, 'size')
    print order
    generators = gap.get_record_element(generator_record, 'generators')
    for gen in generators:
      print gap_gen_to_string(gen)

    print

if __name__ == "__main__":
  main()