/*
    Copyright 2010 Greg Tener and Elena Erbiceanu
    Released under the Lesser General Public License v3.
*/

#include <fssw/FundamentalSchreierTrees.h>

#include <gtest/gtest.h>

#include <fssw/PermutationWord.h>
#include <fssw/SchreierTree.h>

namespace fssw {

class FundamentalSchreierTreesTest : public ::testing::Test {
 public:
};

TEST_F(FundamentalSchreierTreesTest, Instantiation) {
  FundamentalSchreierTrees t;
  t.append_to_base(2);

  EXPECT_EQ(t.getBase(0), 2);
  EXPECT_EQ(t.getBase(1), -1);
}

TEST_F(FundamentalSchreierTreesTest, AddGenerator) {
  FundamentalSchreierTrees t;
  t.append_to_base(0);
  t.append_to_base(1);

  // w does not move a base element
  PermutationWord w1;
  w1.from_string("()");  
  t.add_generator(w1);
  // should show up in both trees
  EXPECT_TRUE(t.getTree(0)->has_generator(w1));
  EXPECT_TRUE(t.getTree(1)->has_generator(w1));

  // w moves the first base element
  PermutationWord w2;
  w2.from_string("(0 2)");  
  t.add_generator(w2);
  // should show up in the first tree only
  EXPECT_TRUE(t.getTree(0)->has_generator(w2));
  EXPECT_FALSE(t.getTree(1)->has_generator(w2));

  // w moves the second base element
  PermutationWord w3;
  w3.from_string("(1 3)");  
  t.add_generator(w3);
  // should show up in both trees
  EXPECT_TRUE(t.getTree(0)->has_generator(w3));
  EXPECT_TRUE(t.getTree(1)->has_generator(w3));
}

TEST_F(FundamentalSchreierTreesTest, StripGenerator) {
  FundamentalSchreierTrees t;
  t.append_to_base(0);
  t.append_to_base(1);
  // TODO: build simple strip example here ...
}

}  // namespace fssw
