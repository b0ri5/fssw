#include <fssw/MapPermutation.h>

namespace fssw {

int MapPermutation::get_image(int a)
{
  if (images_.find(a) == images_.end() )
  {
      return a;
  }

  return images_[a];
}

void compose(const Permutation &g)
{

}

}  // namespace fssw
