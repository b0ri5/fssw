/*
    Copyright 2010 Greg Tener and Elena Erbiceanu
    Released under the Lesser General Public License v3.
*/

#include <fssw/MapPermutation.h>

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
}

void MapPermutation::compose_inverse(const Permutation &g) {
}

void MapPermutation::clear() {
  images_.clear();
  inverse_images_.clear();
}

}  // namespace fssw
