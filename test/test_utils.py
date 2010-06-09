"""
  Code common to at least two tests/data generators.
"""

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
      
def gap_gen_to_string(g):
  l = g.ListPerm()
  seen = set()
  s = ""
  
  for i in range(1, len(l) + 1):
    if int(i) in seen:
      continue
    
    a = i
    seen.add(int(a))  # need to convert to an int instead of a gap int
    cycle = [a]
    
    while i != l[a]:
      a = l[a]
      seen.add(int(a))
      cycle.append(a)
      
    s += "(%s)" % ' '.join([str(a) for a in cycle])
    
  return s