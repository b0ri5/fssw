/*
    Copyright 2010 Greg Tener and Elena Erbiceanu
    Released under the Lesser General Public License v3.
*/

#include <fssw/FundamentalSchreierTrees.h>

#include <fssw/SchreierTree.h>

namespace fssw {

FundamentalSchreierTrees::~FundamentalSchreierTrees() {
  // free all of the copies of generators we needed
  for (vector<const MapPermutation *>::iterator
       it = allocated_generators_.begin(); it != allocated_generators_.end();
       ++it) {
    delete *it;
  }

  // free the words
  for (vector<const PermutationWord *>::iterator
       it = allocated_generator_words_.begin();
       it != allocated_generator_words_.end();
       ++it) {
    delete *it;
  }

  // free the trees
  for (vector<SchreierTree *>::iterator it = trees_.begin(); it != trees_.end();
       ++it) {
    delete *it;
  }
}

void FundamentalSchreierTrees::add_generator_no_copy(const MapPermutation &g) {
  original_generators_.push_back(&g);

  PermutationWord *w_ptr = new PermutationWord();

  w_ptr->compose(g);
  allocated_generator_words_.push_back(w_ptr);
  original_words_.push_back(w_ptr);
  distribute_generator(*w_ptr);
}

void FundamentalSchreierTrees::add_generator(
    const MapPermutation &g) {
  // copy the generator
  MapPermutation *g_ptr = new MapPermutation();
  g_ptr->from_string(g.to_string());

  // add and keep track of the copy
  add_generator_no_copy(*g_ptr);
  allocated_generators_.push_back(g_ptr);
}

void FundamentalSchreierTrees::append_to_base(int b) {
  // add b to the base
  base_.push_back(b);

  SchreierTree *new_tree_ptr = new SchreierTree();

  // then to the trees
  new_tree_ptr->set_root(b);
  trees_.push_back(new_tree_ptr);
  bool changed = false;

  if (base_.size() > 1) {  // a previous tree exists
    SchreierTree *prev_tree_ptr = trees_[base_.size() - 2];
    int b_prev = prev_tree_ptr->get_root();

    // add only the words that fix their tree's root
    for (vector<const PermutationWord *>::iterator
         it = prev_tree_ptr->generators_.begin();
         it != prev_tree_ptr->generators_.end();
         ++it) {
      const PermutationWord *w_ptr = *it;

      if (w_ptr->get_image(b_prev) == b_prev) {
        trees_[base_.size() - 1]->add_generator(**it);
      }
    }
  }
  else {
    // if we just added the first element to the base
    for (vector<const PermutationWord *>::iterator
         it = original_words_.begin();
         it != original_words_.end(); ++it) {
      trees_[0]->add_generator(**it);
    }
  }
}

void FundamentalSchreierTrees::distribute_generator(const PermutationWord &w) {
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

const SchreierTree* FundamentalSchreierTrees::get_tree(int i) {
  if (i < 0 || i >= trees_.size()) {
    return NULL;
  }

  return trees_[i];
}

int FundamentalSchreierTrees::get_base(int i) {
  if (i < 0 || i >= base_.size()) {
    return -1;
  }

  return base_[i];
}

int FundamentalSchreierTrees::get_base_length() {
  return base_.size();
}



bool FundamentalSchreierTrees::build_trees() {
  bool changed = false;
  for (int i = 0; i < trees_.size(); i++) {
    changed = changed || trees_.at(i)->build_tree();
  }

  return changed;
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
  /*vector<const PermutationWord>::const_iterator g_it;

  for (g_it = generators.begin(); g_it != generators.end(); ++g_it) {
    expand_base(*g_it);
  }*/

  // set up schreier trees
  //for (g_it = generators.begin(); g_it != generators.end(); ++g_it) {
  //  add_generator(*g_it);
  //}

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
