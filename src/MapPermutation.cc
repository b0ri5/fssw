/*
    Copyright 2010 Greg Tener and Elena Erbiceanu
    Released under the Lesser General Public License v3.
*/

#include <fssw/MapPermutation.h>

#include <cstdlib>
#include <vector>
#include <set>

using std::vector;
using std::set;

namespace fssw {

int MapPermutation::get_image(int a) {
  if (images_.find(a) == images_.end()) {
      return a;
  }

  return images_[a];
}

int MapPermutation::get_inverse_image(int a) {
  if (inverse_images_.find(a) == inverse_images_.end()) {
      return a;
  }

  return inverse_images_[a];
}

void MapPermutation::compose(const Permutation &g) {
  // at the end, check for validity
}

void MapPermutation::compose_inverse(const Permutation &g) {
  // at the end, check for validity
}

void MapPermutation::clear() {
  images_.clear();
  inverse_images_.clear();
}

bool MapPermutation::is_identity() {
  return images_.size() == 0;
}

static bool is_whitespace(char c) {
  if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
    return true;
  }

  return false;
}

static string trim_front(string s) {
  int start = 0;

  while (start < s.size() && is_whitespace(s.at(start))) {
    start++;
  }

  return s.substr(start);
}

static string trim_back(string s) {
  int end = s.size() - 1;

  while (end > 0 && is_whitespace(s.at(end))) {
    end--;
  }

  return s.substr(0, end);
}

static string trim_whitespace(string s) {
  s = trim_front(s);
  s = trim_back(s);

  return s;
}

static bool is_digit(char c) {
  return (c >= '0' && c <= '9');
}

// read an integer into a, advancing the pos
bool parse_nonnegative_int(const string &s, int *pos_ptr, int *a_ptr) {
  int len = 0;

  while (*pos_ptr + len < s.length() && is_digit(s.at(*pos_ptr + len))) {
    len++;
  }

  // if we see no digits, return false
  if (len == 0) {
    fprintf(stdout, "Expected integer at pos %d in %s.\n", *pos_ptr, s.c_str());
    return false;
  }

  // parse the integer and increment the position
  *a_ptr = atoi(s.substr(*pos_ptr, len).c_str());
  (*pos_ptr) += len;

  return true;
}

// parses a string "(0 2 3)" into a vector (doesn't check for validity)
// returns true if cycle is well-formed
bool parse_cycle(const string &s, int *pos_ptr, vector<int> *cycle_ptr,
    set<int> *elements_seen_ptr) {
  // starts with '('
  if (s.at(*pos_ptr) != '(') {
    fprintf(stdout, "Expected opening parenthesis at %d.\n", *pos_ptr);
    return false;
  }

  // pass up the '('
  (*pos_ptr)++;

  int a = 0;

  while (*pos_ptr < s.length()) {
    // try to read an int
    if (!parse_nonnegative_int(s, pos_ptr, &a)) {
      return false;
    }

    // if this element was in the permutation before, input is malformed.
    if (elements_seen_ptr->find(a) != elements_seen_ptr->end()) {
      fprintf(stdout, "Element repeated: %d\n", a);
      return false;
    }

    elements_seen_ptr->insert(a);
    cycle_ptr->push_back(a);

    // if we're not looking at a ' ' then break
    if (s.at(*pos_ptr) != ' ') {
      break;
    }

    (*pos_ptr)++;
  }

  if (*pos_ptr >= s.length() || s.at(*pos_ptr) != ')') {
    fprintf(stdout, "Expected ending parenthesis: %s at %d\n",
            s.c_str(), *pos_ptr);
    return false;
  }
  (*pos_ptr)++;

  // cycle was well formed
  return true;
}

// advances to the first non-whitespace character
void eat_whitespace(const string &s, int *pos_ptr) {
  while (*pos_ptr < s.length() && is_whitespace(s.at(*pos_ptr))) {
    (*pos_ptr)++;
  }
}

bool MapPermutation::from_string(string s) {
  clear();

  int pos = 0;

  // eat initial whitespace
  eat_whitespace(s, &pos);

  // special case, identity
  if (s.substr(pos, 2).compare("()") == 0) {
    return true;
  }

  // parse the cycles
  vector<vector<int> > cycles;
  set<int> elements_seen;

  while (pos < s.length() && s.at(pos) == '(') {
    vector<int> cycle;

    if (!parse_cycle(s, &pos, &cycle, &elements_seen)) {
      return false;
    }
    if (cycle.size() > 1) {
      cycles.push_back(cycle);
    }
  }

  // eat trailing whitespace
  eat_whitespace(s, &pos);

  // if there is still non-whitespace left over, invalid input
  if (pos < s.length()) {
    fprintf(stdout, "Expected only whitespace in: %s at %d\n", s.c_str(), pos);
    return false;
  }

  // form permutation from cycles
  for (vector<vector<int> >::const_iterator cycle_it = cycles.begin();
      cycle_it != cycles.end(); cycle_it++) {
    int element_pos = 0;
    int a = 0;
    int b = 0;

    while (element_pos + 1 < cycle_it->size()) {
      a = cycle_it->at(element_pos);
      b = cycle_it->at(element_pos + 1);
      images_[a] = b;
      inverse_images_[b] = a;
      element_pos++;
    }
    // connect circular ends
    a = cycle_it->at(cycle_it->size() - 1);
    b = cycle_it->at(0);
    images_[a] = b;
    inverse_images_[b] = a;
  }

  return true;
}

}  // namespace fssw
