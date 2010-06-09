#ifndef INCLUDE_FSSW_FUNDAMENTALSCHREIERTREES_H_
#define INCLUDE_FSSW_FUNDAMENTALSCHREIERTREES_H_

/*
    Copyright 2010 Greg Tener and Elena Erbiceanu
    Released under the Lesser General Public License v3.
*/

#include <vector>
#include <string>

using std::vector;
using std::string;

namespace fssw {

class MapPermutation;
class PermutationWord;
class SchreierTree;

/*
 * TODO: reevaluate the name of this class and fill in its description
 */

class FundamentalSchreierTrees {
 public:

  ~FundamentalSchreierTrees();

  // tries to reduce g to identity through the current tree structure
  int strip(const PermutationWord &g, PermutationWord *h_ptr) const;
  int strip(const MapPermutation &g, PermutationWord *h_ptr) const;

  // append b to the base (add a new tree);
  // also copies the appropriate generators in the previous tree to the new one
  void append_to_base(int b);

  // adds "g" as an original generator; makes a copy of "g"
  void add_generator(const MapPermutation &g);
  bool add_generator(string s);
  void add_generator_no_copy(const MapPermutation &g);

  // accessors used in testing
  const SchreierTree *get_tree(int i) const;
  int get_base(int i) const;
  void get_base(vector<int> *v_ptr) const;
  int get_base_length() const;
  const PermutationWord *get_original_word(int i) const;
  int get_original_generators_length() const;

  // returns the order of the group, returns -1 if an overflow occurs
  long long int order() const;

  // for testing purposes, output as a string
  string to_string() const;

  // calls build_tree() on each schreier tree, returns true if a tree changed
  bool build_trees();

  int schreier_sims();

  // ensures that H^(i)_b = H^(i+1), and modifies "i" to be the next place
  // this condition should be ensured
  bool ensure_stabilizer_is_generated(int *i_ptr);

  // appends elements to the base until each generator moves the base
  bool ensure_each_generator_moves_base();

  // returns true if the schreier generators of H^(i)_{b_i} are all contained in
  // H^(i+1)
  bool is_strongly_generated();

  bool does_each_generator_move_base() const;

 private:
  // adds "w" to each schreier tree up until "w" moves a base element
  void distribute_generator(const PermutationWord &w);

  vector<int> base_;
  vector<SchreierTree *> trees_;
  vector<const MapPermutation *> original_generators_;
  vector<const PermutationWord *> original_words_;

  vector<const MapPermutation *> allocated_generators_;
  vector<const PermutationWord *> allocated_generator_words_;
};

template <typename T>
static void delete_vector_ptrs(const vector<T *> &v) {
  for (typename vector<T *>::const_iterator
       it = v.begin(); it != v.end();
       ++it) {
    delete *it;
  }
}

}  // namespace fssw

#endif  // INCLUDE_FSSW_FUNDAMENTALSCHREIERTREES_H_
