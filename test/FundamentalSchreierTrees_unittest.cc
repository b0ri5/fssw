/*
    Copyright 2010 Greg Tener and Elena Erbiceanu
    Released under the Lesser General Public License v3.
*/

#include <fssw/FundamentalSchreierTrees.h>

#include <gtest/gtest.h>

#include <fssw/PermutationWord.h>

namespace fssw {

class FundamentalSchreierTreesTest : public ::testing::Test {
 public:
};

TEST_F(FundamentalSchreierTreesTest, Instantiation) {
  FundamentalSchreierTrees t;
  t.append_to_base(0);
}

TEST_F(FundamentalSchreierTreesTest, AddGenerator) {
  FundamentalSchreierTrees t;
  t.append_to_base(0);
  t.append_to_base(1);

  PermutationWord w;
  w.from_string("(0 1)");

  t.add_generator(w);
}

}  // namespace fssw
