/*
    Copyright 2010 Greg Tener and Elena Erbiceanu
    Released under the Lesser General Public License v3.
*/

#include <fssw/FundamentalSchreierTrees.h>

#include <fssw/SchreierTree.h>

namespace fssw {

void FundamentalSchreierTrees::append_to_base(int b) {
  base_.push_back(b);

  SchreierTree *tree = new SchreierTree();
  tree->set_root(b);
  trees_.push_back(tree);
}

void FundamentalSchreierTrees::add_generator(const PermutationWord &w) {
  for (int i = 0; i < base_.size(); ++i) {
    trees_[i]->add_generator(w);

    int b = base_[i];

    if (w.get_image(b) != b) {
      break;
    }
  }
}

int FundamentalSchreierTrees::strip(const PermutationWord &g,
  PermutationWord* h_ptr) {
  h_ptr->clear();
  h_ptr->compose(g);

  for (int i = 0; i < base_.size(); ++i) {
    int a = h_ptr->get_image(base_.at(i));
    SchreierTree *tree_ptr = trees_.at(i);

    if (!tree_ptr->is_in_orbit(a)) {
      return i;
    }

    PermutationWord path;
    tree_ptr->path_to_root(a, &path);
    h_ptr->compose(path);
  }

  return base_.size();
}

const SchreierTree* FundamentalSchreierTrees::getTree(int i) {
  if (i < 0 || i >= trees_.size()) {
    return NULL;
  }
  return trees_[i];
}

int FundamentalSchreierTrees::getBase(int i) {
  if (i < 0 || i >= base_.size()) {
    return -1;
  }
  return base_[i];
}

void FundamentalSchreierTrees::build_trees() {
  for (int i = 0; i < trees_.size(); i++) {
    trees_.at(i)->build_tree();
  }
}

bool FundamentalSchreierTrees::expand_base(const PermutationWord &g) {
  // ensures one condition that B is a base: the only permutation that does
  // not move any element of the base should be the identity;
  for (int i = 0; i <= base_.size(); ++i) {
    int b = base_[i];

    if (g.get_image(b) != b) {
      // g moves at least one base element
      return false;
    }
  }

  // g is in G_(b1,...,bk)
  int a = g.get_first_moved_element();

  // if g is the identity, base does not need to be expanded
  if (a == -1) {
    return false;
  }

  append_to_base(a);
  return true;
}

int FundamentalSchreierTrees::schreier_sims(
  const vector<const PermutationWord> &generators) {
  // add elements to the base until there are no generators in G_(b1,...,bk)
  vector<const PermutationWord>::const_iterator g_it;

  for (g_it = generators.begin(); g_it != generators.end(); ++g_it) {
    expand_base(*g_it);
  }

  // set up schreier trees
  for (g_it = generators.begin(); g_it != generators.end(); ++g_it) {
    add_generator(*g_it);
  }

  build_trees();

  // now: H^(k+1) = e

  int i = base_.size() - 1;

  while (i >= 0) {
    // test condition that H^(i)_{b_i} = H^(i+1)
    SchreierTree *t_i = trees_[i];
    for (OrbitIterator orbit_it = t_i->get_orbit_iterator();
      orbit_it.has_next(); ++orbit_it) {
      int a = *orbit_it;

      // find g such that: b_i^(g_a) = a
      PermutationWord g_a;
      t_i->path_to_root(a, &g_a);

      vector<const PermutationWord*> *S_i = &t_i->generators_;

      for (vector<const PermutationWord*>::const_iterator s_it = S_i->begin();
        s_it != S_i->end(); ++s_it) {
        const PermutationWord *s = *s_it;
        int as = s->get_image(a);

        // find g_as such that: b_i^(g_as) = a^s
        PermutationWord g_as;
        t_i->path_to_root(as, &g_as);

        // ga_s = g_a * s
        PermutationWord ga_s;
        ga_s.compose(g_a);
        ga_s.compose(*s);

        if (!ga_s.is_equivalent(g_as)) {
          // TODO:...
        }
      }
    }
  }

  return 0;
}

}  // namespace fssw
