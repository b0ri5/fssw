#!/usr/bin/env sage

#from sage.interfaces.gap import gap
#from sage.groups.perm_gps.permgroup import PermutationGroup

from sage.all import *

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
      
def main():
  for G in small_groups(range(1, 100)):
    print G
    print G.StabChain().StrongGeneratorsStabChain()
  
if __name__ == "__main__":
  main()