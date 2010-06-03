/*
    Copyright 2010 Greg Tener and Elena Erbiceanu
    Released under the Lesser General Public License v3.
*/

#include <fssw/Permutation.h>

namespace fssw {

bool Permutation::fixes(int a) const {
  return get_image(a) == a;
}

}  // namespace fssw
