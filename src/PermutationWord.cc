/*
    Copyright 2010 Greg Tener and Elena Erbiceanu
    Released under the Lesser General Public License v3.
*/

#include <fssw/PermutationWord.h>

namespace fssw {

PermutationPart::PermutationPart()
: g(), is_inverse(false) {
}

PermutationPart::PermutationPart(const MapPermutation &g_, bool is_inverse_)
: g(g_), is_inverse(is_inverse_) {
}

bool PermutationPart::from_string(string s) {
  size_t power_pos = s.find("^{-1}");

  // if positive, the whole string should be one permutation
  if (power_pos == string::npos) {
    return g.from_string(s);
  }

  // if inverse, there should be no other characters after "^{-1}"
  if (static_cast<int>(power_pos) + strlen("^{-1}") < s.length()) {
    fprintf(stdout, "Unexpected characters after ^{-1}: %s\n.", s.c_str());
    return false;
  }

  // read inverse permutation
  is_inverse = true;
  string perm_s = s.substr(0, power_pos);
  return g.from_string(perm_s);
}

int PermutationWord::get_image(int a) const {
  for (vector<PermutationPart>::const_iterator perm_it =
      permutations_.begin(); perm_it != permutations_.end(); ++perm_it) {
    if (!perm_it->is_inverse) {
      a = perm_it->g.get_image(a);
    } else {
      a = perm_it->g.get_inverse_image(a);
    }
  }
  return a;
}

int PermutationWord::get_inverse_image(int a) const {
  for (vector<PermutationPart>::const_iterator perm_it = permutations_.begin();
      perm_it != permutations_.end(); ++perm_it) {
    if (!perm_it->is_inverse) {
      a = perm_it->g.get_inverse_image(a);
    } else {
      a = perm_it->g.get_image(a);
    }
  }
  return a;
}

void PermutationWord::compose(const PermutationWord &g) {
  // appends the list of permutations from g to this
  // does not check last permutation to contract g*g^{-1} to identity
  permutations_.insert(permutations_.end(), g.permutations_.begin(),
      g.permutations_.end());
}

void PermutationWord::compose_inverse(const PermutationWord &g) {
  // appends the list of permutations from g to this, in reverse order
  // does not check last permutation to contract g*g^{-1} to identity
  int g_start_pos = permutations_.size();
  permutations_.insert(permutations_.end(), g.permutations_.rbegin(),
      g.permutations_.rend());
  // negates the permutations added from g
  for (int pos = g_start_pos; pos < permutations_.size(); ++pos) {
    permutations_.at(pos).is_inverse = !permutations_.at(pos).is_inverse;
  }
}

void PermutationWord::clear() {
  permutations_.clear();
}

bool PermutationWord::is_identity() const {
  // For now, using the less efficient version that requires full evaluation;
  // Future: expand function and return false as soon as one element is moved.
  MapPermutation g;
  evaluate(&g);
  return g.is_identity();
}

void PermutationWord::evaluate(MapPermutation *g_ptr) const {
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

    PermutationPart perm;
    if (!perm.from_string(perm_s)) {
      return false;
    }

    permutations_.push_back(perm);

    // advance to rest of string
    s = s.substr(compose_pos + 1, s.length());
    compose_pos = s.find("*");
  }

  // push the last permutation
  PermutationPart perm;
  if (!perm.from_string(s)) {
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
  s += part_it->g.to_string();
  s += part_it->is_inverse ? "^{-1}" : "";
  ++part_it;

  // rest of the elements
  while (part_it != permutations_.end()) {
    s += "*" + part_it->g.to_string();
    s += part_it->is_inverse ? "^{-1}" : "";
    ++part_it;
  }

  return s;
}

void PermutationWord::compose(const MapPermutation &g) {
  // does not check last permutation to contract g*g^{-1} to identity
  permutations_.push_back(PermutationPart(g, false));
}

void PermutationWord::compose_inverse(const MapPermutation &g) {
  // does not check last permutation to contract g*g^{-1} to identity
  permutations_.push_back(PermutationPart(g, true));
}

}  // namespace fssw
