/*
    Copyright 2010 Greg Tener and Elena Erbiceanu
    Released under the Lesser General Public License v3.
*/

#include <fssw/FundamentalSchreierTrees.h>

#include <fssw/SchreierTree.h>

#include <sstream>

using std::stringstream;

namespace fssw {

FundamentalSchreierTrees::~FundamentalSchreierTrees() {
  // free all of the copies of generators we needed
  delete_vector_ptrs(allocated_generators_);

  // free the words
  delete_vector_ptrs(allocated_generator_words_);

  // free the trees
  delete_vector_ptrs(trees_);
}

void FundamentalSchreierTrees::add_generator_no_copy(const MapPermutation &g) {
  original_generators_.push_back(&g);

  PermutationWord *w_ptr = new PermutationWord();

  w_ptr->compose(g);
  allocated_generator_words_.push_back(w_ptr);
  original_words_.push_back(w_ptr);
  distribute_generator(*w_ptr);
}

void FundamentalSchreierTrees::add_generator(const MapPermutation &g) {
  // copy the generator
  MapPermutation *g_ptr = new MapPermutation();
  g_ptr->from_string(g.to_string());

  // add and keep track of the copy
  add_generator_no_copy(*g_ptr);
  allocated_generators_.push_back(g_ptr);
}

bool FundamentalSchreierTrees::add_generator(string s) {
  MapPermutation g;
  bool rv = g.from_string(s);

  if (rv) {
    add_generator(g);
  }

  return rv;
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
  } else {
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
    PermutationWord* h_ptr) const {
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

int FundamentalSchreierTrees::strip(const MapPermutation &g,
    PermutationWord *h_ptr) const {
  PermutationWord w;

  w.compose(g);

  return strip(w, h_ptr);
}

const SchreierTree* FundamentalSchreierTrees::get_tree(int i) const {
  if (i < 0 || i >= trees_.size()) {
    return NULL;
  }

  return trees_[i];
}

int FundamentalSchreierTrees::get_base(int i) const {
  if (i < 0 || i >= base_.size()) {
    return -1;
  }

  return base_[i];
}

void FundamentalSchreierTrees::get_base(vector<int> *v_ptr) const {
  *v_ptr = base_;
}

int FundamentalSchreierTrees::get_original_generators_length() const {
  return original_generators_.size();
}

const PermutationWord *FundamentalSchreierTrees::get_original_word(int i)
      const {
  return original_words_.at(i);
}

int FundamentalSchreierTrees::get_base_length() const {
  return base_.size();
}



bool FundamentalSchreierTrees::build_trees() {
  bool changed = false;

  for (int i = 0; i < trees_.size(); i++) {
    SchreierTree &tree = *trees_[i];
    changed = tree.build_tree() || changed;  // order is important here!!!
  }

  return changed;
}

bool FundamentalSchreierTrees::ensure_each_generator_moves_base() {
  bool changed = false;

  // for each generator
  for (vector<const PermutationWord *>::const_iterator
       it = original_words_.begin(); it != original_words_.end(); ++it) {
    const PermutationWord &s = **it;

    // if it fixes the base
    if (s.fixes(base_)) {
      // append to the base an element that "s" moves
      int a = s.get_moved_element();
      append_to_base(a);
      changed = true;
    }
  }

  return changed;
}

int FundamentalSchreierTrees::schreier_sims() {
  ensure_each_generator_moves_base();
  build_trees();

  int i = base_.size() - 1;
  bool changed = false;

  while (i >= 0) {
    // test condition that H^(i)_{b_i} = H^(i+1)
    changed = ensure_stabilizer_is_generated(&i) || changed;
  }

  return changed;
}

bool FundamentalSchreierTrees::ensure_stabilizer_is_generated(int *i_ptr) {
  bool changed = false;
  SchreierTree &t = *trees_.at(*i_ptr);

  for (OrbitIterator orbit_it = t.get_orbit_iterator();
    orbit_it.not_at_end(); ++orbit_it) {
    int a = *orbit_it;

    // don't test the stabilizer of the root (it will be the identity)
    if (a == t.get_root()) {
      continue;
    }

    // find g such that: b^g = a, where b == t.get_root()
    PermutationWord b_to_a;
    t.path_from_root(a, &b_to_a);

    const vector<const PermutationWord*> &generators = t.generators_;

    for (vector<const PermutationWord*>::const_iterator
         s_it = generators.begin(); s_it != generators.end(); ++s_it) {
      const PermutationWord &s = **s_it;
      int as = s.get_image(a);

      // find g such that: b^g = as
      PermutationWord b_to_as;
      t.path_from_root(as, &b_to_as);

      // compose b_to_a with s
      PermutationWord b_to_a_s;
      b_to_a_s.compose(b_to_a);
      b_to_a_s.compose(s);

      // if b_to_a_s == b_to_as we will be sifting the identity which is
      // unnecessary
      if (b_to_a_s.is_equivalent(b_to_as)) {
        continue;
      }

      PermutationWord schreier_gen;
      schreier_gen.compose(b_to_a_s);
      schreier_gen.compose_inverse(b_to_as);

      PermutationWord h;
      // see what the schreier gen sifts to
      int j = strip(schreier_gen, &h);
      bool has_new_strong_gen = false;

      // if we didn't get all the way to the last tree, add the stripee
      if (j < base_.size()) {
        has_new_strong_gen = true;
      } else if (!h.is_identity()) {  // otherwise we got to the end, but "h" is
        has_new_strong_gen = true;    // not the identity, so extend the base
        append_to_base(h.get_moved_element());
      }

      if (!has_new_strong_gen) {
        continue;
      }

      changed = true;
      /*printf("b_to_a == %s\n", b_to_a.to_evaluated_string().c_str());
      printf("s == %s\n", s.to_evaluated_string().c_str());
      printf("b_to_a_s == %s\n", b_to_a_s.to_evaluated_string().c_str());
      printf("a == %d\n", a);
      printf("as == %d\n", as);
      printf("b_to_as == %s\n", b_to_as.to_string().c_str());
      printf("schreier_gen == %s\n", schreier_gen.to_evaluated_string().c_str());
      printf("h == %s\n", h.to_evaluated_string().c_str());
      printf("state before stripping %s [%s]\n%s\n", schreier_gen.to_string().c_str(),
          schreier_gen.to_evaluated_string().c_str(), to_string().c_str());*/
      // j = strip(schreier_gen, &h);

      // add the new generator
      PermutationWord *new_strong_gen = new PermutationWord();
      allocated_generator_words_.push_back(new_strong_gen);
      new_strong_gen->compose(h);

      for (int l = *i_ptr + 1; l <= j; ++l) {
        trees_[l]->add_generator(*new_strong_gen);
        trees_[l]->build_tree();
      }

      *i_ptr = j;
      return true;
    }
  }

  *i_ptr -= 1;

  return changed;
}

bool FundamentalSchreierTrees::does_each_generator_move_base() const {
  for (int i = 0; i < original_generators_.size(); ++i) {
    if (original_generators_[i]->fixes(base_)) {
      return false;
    }
  }

  return true;
}

bool FundamentalSchreierTrees::is_strongly_generated() {
  for (int i = 0; i < base_.size(); ++i) {
    // test each schreier generator
    SchreierTree &t = *trees_[i];

    for (OrbitIterator orbit_it = t.get_orbit_iterator();
      orbit_it.not_at_end(); ++orbit_it) {
      int a = *orbit_it;

      if (a == t.get_root()) {
        continue;
      }

      PermutationWord b_to_a;
      t.path_from_root(a, &b_to_a);

      const vector<const PermutationWord*> &generators = t.generators_;

      for (vector<const PermutationWord*>::const_iterator
           s_it = generators.begin(); s_it != generators.end(); ++s_it) {
        const PermutationWord &s = **s_it;
        int as = s.get_image(a);

        // find g such that: b^g = as
        PermutationWord b_to_as;
        t.path_from_root(as, &b_to_as);

        // construct the schreier gen
        PermutationWord schreier_gen;
        schreier_gen.compose(b_to_a);
        schreier_gen.compose(s);
        schreier_gen.compose_inverse(b_to_as);

        PermutationWord h;
        int j = strip(schreier_gen, &h);

        if (j < base_.size() || !h.is_identity()) {
          return false;
        }
      }
    }
  }

  return does_each_generator_move_base();
}

long long int FundamentalSchreierTrees::order() const {
  long long int product = 1;

  // the order is the product of the fundamental orbit sizes
  for (int i = 0; i < trees_.size(); ++i) {
    // check for overflow
    if (product * trees_[i]->size() < product) {
      return -1;
    }

    product *= trees_[i]->size();
  }

  return product;
}

string FundamentalSchreierTrees::to_string() const {
  stringstream ss;

  ss << "original_generators: ";

  for (int i = 0; i < original_generators_.size(); ++i) {
    const MapPermutation &s = *original_generators_[i];
    ss << s.to_string();
    if (i + 1 < original_generators_.size()) {
      ss << ", ";
    }
  }

  ss << "\n";
  ss << "base: ";

  for (int i = 0; i < base_.size(); ++i) {
    ss << base_[i] << " ";
  }

  ss << "\n";

  ss << "trees: \n";
  for (int i = 0; i < trees_.size(); i++) {
    ss << trees_[i]->to_string() << "\n";
  }

  ss << "\n";

  return ss.str();
}

}  // namespace fssw
