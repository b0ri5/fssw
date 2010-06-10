/*
    Copyright 2010 Greg Tener and Elena Erbiceanu
    Released under the Lesser General Public License v3.
*/

#include <fssw/PermutationWord.h>

#include <cstdio>
#include <cstring>

#include <set>

using std::set;

namespace fssw {

vector<MapPermutation*> MapPermutationAllocator::permutations_;

MapPermutation* MapPermutationAllocator::new_permutation() {
  MapPermutation *g = new MapPermutation();
  permutations_.push_back(g);
  return g;
}

void MapPermutationAllocator::clear_memory() {
  for (vector<MapPermutation*>::iterator perm_it = permutations_.begin();
      perm_it != permutations_.end(); ++perm_it) {
    delete *perm_it;
  }

  permutations_.clear();
}

PermutationPart::PermutationPart(const MapPermutation *g_ptr_, bool is_inverse_)
: g_ptr(g_ptr_), is_inverse(is_inverse_) {
}

bool PermutationPart::from_string(string s) {
  MapPermutation *new_g_ptr = MapPermutationAllocator::new_permutation();

  size_t power_pos = s.find("^{-1}");

  // if positive, the whole string should be one permutation
  if (power_pos == string::npos) {
    if (new_g_ptr->from_string(s)) {
      g_ptr = new_g_ptr;
      return true;
    } else {
      return false;
    }
  }

  // if inverse, there should be no other characters after "^{-1}"
  if (static_cast<int>(power_pos) + strlen("^{-1}") < s.length()) {
    fprintf(stdout, "Unexpected characters after ^{-1}: %s\n.", s.c_str());
    return false;
  }

  // read inverse permutation
  is_inverse = true;
  string perm_s = s.substr(0, power_pos);
  if (new_g_ptr->from_string(perm_s)) {
    g_ptr = new_g_ptr;
    return true;
  }
  return false;
}

int PermutationWord::get_image(int a) const {
  for (vector<PermutationPart>::const_iterator perm_it =
      permutations_.begin(); perm_it != permutations_.end(); ++perm_it) {
    if (!perm_it->is_inverse) {
      a = perm_it->g_ptr->get_image(a);
    } else {
      a = perm_it->g_ptr->get_inverse_image(a);
    }
  }
  return a;
}

int PermutationWord::get_inverse_image(int a) const {
  // this implementation needs to iterate over the words in reverse
  for (vector<PermutationPart>::const_reverse_iterator
      perm_it = permutations_.rbegin();
      perm_it != permutations_.rend(); ++perm_it) {
    if (!perm_it->is_inverse) {
      a = perm_it->g_ptr->get_inverse_image(a);
    } else {
      a = perm_it->g_ptr->get_image(a);
    }
  }

  return a;
}

int PermutationWord::get_moved_element() const {
  // this basically checks for identity more efficiently;
  // returns first moved element or -1 if none are moved.
  set<int> moved_elements;

  for (vector<PermutationPart>::const_iterator part_it = permutations_.begin();
    part_it != permutations_.end(); ++part_it) {
    const map<int, int> *images_ptr = &(part_it->g_ptr->images_);

    for (map<int, int>::const_iterator images_it = images_ptr->begin();
      images_it != images_ptr->end(); ++images_it) {
      int element = images_it->first;

      if (moved_elements.find(element) != moved_elements.end()) {
        continue;
      }

      // element is moved for the first time in this MapPermutation,
      // check if it remains moved at the end of the word:
      if (get_image(element) != element) {
        return element;
      }

      moved_elements.insert(element);
    }
  }

  // permutation was in fact identity
  return -1;
}

// appends the list of permutations from "w" to this
void PermutationWord::compose(const PermutationWord &w) {
  vector<PermutationPart>::const_iterator w_it;

  // check the permutations to contract g*g^{-1} to the identity
  w_it = w.permutations_.begin();

  while (permutations_.size() > 0 && w_it != w.permutations_.end() &&
         permutations_.back().g_ptr == w_it->g_ptr &&
         permutations_.back().is_inverse != w_it->is_inverse) {
    permutations_.pop_back();
    ++w_it;
    // printf("*\n");
  }

  // append the permutations from "w" skipping the ones canceled
  permutations_.insert(permutations_.end(), w_it, w.permutations_.end());
}

/*
 * Compose "this" with the inverse of "w"
 * If "this" is u_1 ... u_k and "w" is w_1 ... w_l then
 * u_1 ... u_k * (w_1 ... w_l)^-1
 * u_1 ... u_k * w_l^-1 ... w_1^-1
 * u_1 ... u_k-m  w_l-m^-1 ... w_1^-1 for m cancellations
 */
void PermutationWord::compose_inverse(const PermutationWord &w) {
  vector<PermutationPart>::const_reverse_iterator w_it;

  // check for cancellations
  // the permutations in "w" are traversed in reverse order
  w_it = w.permutations_.rbegin();

  while (permutations_.size() > 0 && w_it != w.permutations_.rend() &&
         permutations_.back().g_ptr == w_it->g_ptr &&
         permutations_.back().is_inverse == w_it->is_inverse) {
    permutations_.pop_back();
    ++w_it;
    // printf("-\n");
  }

  // append then negate the non-canceled permutations
  for (; w_it != w.permutations_.rend(); ++w_it) {
    permutations_.push_back(*w_it);
    permutations_.back().is_inverse = !w_it->is_inverse;
  }

  return;
  // appends the list of permutations from g to this, in reverse order
  // does not check last permutation to contract g*g^{-1} to identity
  int g_start_pos = permutations_.size();
  permutations_.insert(permutations_.end(), w.permutations_.rbegin(),
      w.permutations_.rend());
  // negates the permutations added from g
  for (int pos = g_start_pos; pos < permutations_.size(); ++pos) {
    permutations_.at(pos).is_inverse = !permutations_.at(pos).is_inverse;
  }
}

void PermutationWord::clear() {
  permutations_.clear();
}

bool PermutationWord::is_identity() const {
  return get_moved_element() == -1;
}

void PermutationWord::evaluate(MapPermutation *g_ptr) const {
  g_ptr->clear();
  set<int> moved_elements;

  for (vector<PermutationPart>::const_iterator part_it = permutations_.begin();
    part_it != permutations_.end(); ++part_it) {
    const map<int, int> *images_ptr = &(part_it->g_ptr->images_);

    for (map<int, int>::const_iterator images_it = images_ptr->begin();
      images_it != images_ptr->end(); ++images_it) {
      int element = images_it->first;

      if (moved_elements.find(element) == moved_elements.end()) {
        // element is moved for the first time in this MapPermutation,
        // find its image in the word:
        g_ptr->set_image(element, get_image(element));
        moved_elements.insert(element);
      }
    }
  }
}

bool PermutationWord::from_string(string s) {
  clear();

  if (s.compare("()") == 0) {
    return true;
  }

  size_t compose_pos = s.find("*");

  while (compose_pos != string::npos) {
    // read current permutation element
    string perm_s = s.substr(0, compose_pos);

    PermutationPart perm(MapPermutationAllocator::new_permutation(), false);
    if (!perm.from_string(perm_s)) {
      clear();
      return false;
    }

    permutations_.push_back(perm);

    // advance to rest of string
    s = s.substr(compose_pos + 1, s.length());
    compose_pos = s.find("*");
  }

  // push the last permutation
  PermutationPart perm(MapPermutationAllocator::new_permutation(), false);
  if (!perm.from_string(s)) {
    clear();
    return false;
  }

  permutations_.push_back(perm);
  return true;
}

string PermutationWord::to_string() const {
  string s;

  if (permutations_.size() == 0) {
    return "()";
  }

  // print first element
  vector<PermutationPart>::const_iterator part_it = permutations_.begin();
  s += part_it->g_ptr->to_string();
  s += part_it->is_inverse ? "^{-1}" : "";
  ++part_it;

  // rest of the elements
  while (part_it != permutations_.end()) {
    s += "*" + part_it->g_ptr->to_string();
    s += part_it->is_inverse ? "^{-1}" : "";
    ++part_it;
  }

  return s;
}

string PermutationWord::to_evaluated_string() const {
  MapPermutation g;

  evaluate(&g);

  return g.to_string();
}

void PermutationWord::compose(const MapPermutation &g) {
  // does not check last permutation to contract g*g^{-1} to identity
  permutations_.push_back(PermutationPart(&g, false));
}

void PermutationWord::compose_inverse(const MapPermutation &g) {
  // does not check last permutation to contract g*g^{-1} to identity
  permutations_.push_back(PermutationPart(&g, true));
}

// helper which does what PermutationWord::get_image() does but only works on
// a portion of the permutations
static int get_subword_image(int a,
    const vector<PermutationPart>::const_iterator &begin,
    const vector<PermutationPart>::const_iterator &end) {
  for (vector<PermutationPart>::const_iterator it = begin; it != end; ++it) {
    if (!it->is_inverse) {
      a = it->g_ptr->get_image(a);
    } else {
      a = it->g_ptr->get_inverse_image(a);
    }
  }

  return a;
}

// optimization idea: can ignore equal prefixes and equal suffixes
bool PermutationWord::is_equivalent(const PermutationWord &w) const {
  // the set of elements "a" such that
  // this->get_image(a) == b ==> w->get_image(a) == b
  set<int> checked;

  // eat the common prefix
  vector<PermutationPart>::const_iterator this_it = permutations_.begin();
  vector<PermutationPart>::const_iterator w_it = w.permutations_.begin();

  while (this_it != permutations_.end() &&
         w_it != w.permutations_.end() &&
         this_it->g_ptr == w_it->g_ptr &&
         this_it->is_inverse == w_it->is_inverse) {
    ++this_it;
    ++w_it;
  }

  // eat the common suffix
  vector<PermutationPart>::const_iterator this_it_end = permutations_.end();
  vector<PermutationPart>::const_iterator w_it_end = w.permutations_.end();

  // only try to move back if we're not at this_it or w_it already
  if (this_it != this_it_end && w_it != w_it_end) {
    // need to decrement so that we are not at .end() for anyone
    --this_it_end;
    --w_it_end;

    while (this_it_end != this_it &&
           w_it_end != w_it &&
           this_it_end->g_ptr == w_it_end->g_ptr &&
           this_it_end->is_inverse == w_it_end->is_inverse) {
      --this_it_end;
      --w_it_end;
    }

    ++this_it_end;
    ++w_it_end;
  }

  // check each letter in "this"
  for (vector<PermutationPart>::const_iterator part_it = this_it;
      part_it != this_it_end; ++part_it) {
    const map<int, int> &images = part_it->g_ptr->images_;

    for (map<int, int>::const_iterator images_it = images.begin();
         images_it != images.end(); ++images_it) {
      int a = images_it->first;

      if (checked.find(a) != checked.end()) {
        continue;
      }

      if (get_subword_image(a, part_it, this_it_end) ==
          get_subword_image(a, w_it, w_it_end)) {
        checked.insert(a);
      } else {
        return false;
      }
    }
  }

  // check each letter in "w"
  for (vector<PermutationPart>::const_iterator part_it = w_it;
      part_it != w_it_end; ++part_it) {
    const map<int, int> &images = part_it->g_ptr->images_;

    for (map<int, int>::const_iterator images_it = images.begin();
         images_it != images.end(); ++images_it) {
      int a = images_it->first;

      if (checked.find(a) != checked.end()) {
        continue;
      }

      if (get_subword_image(a, w_it, w_it_end) != a) {
        return false;
      }
    }
  }

  return true;
}

}  // namespace fssw
