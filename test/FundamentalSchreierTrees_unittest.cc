/*
    Copyright 2010 Greg Tener and Elena Erbiceanu
    Released under the Lesser General Public License v3.
*/

#include <fssw/FundamentalSchreierTrees.h>

#include <gtest/gtest.h>

namespace fssw {

class FundamentalSchreierTreesTest : public ::testing::Test {
 public:
};

TEST_F(FundamentalSchreierTreesTest, Instantiation) {
  FundamentalSchreierTrees t;
  t.append_to_base(0);
}

}  // namespace fssw
