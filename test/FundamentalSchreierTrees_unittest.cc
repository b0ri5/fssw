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

TEST_F(FundamentalSchreierTreesTest, StripSmallest) {
  FundamentalSchreierTrees t;
  PermutationWord g;
  PermutationWord h;

  // for the last return, the identity should always strip
  EXPECT_EQ(0, t.strip(g, &h));
  EXPECT_TRUE(h.is_identity() );

  t.append_to_base(0);

  // for the part after the "if", the identity should strip all the way down
  EXPECT_EQ(1, t.strip(g, &h));
  EXPECT_TRUE(h.is_identity() );

  // for the "if", something not in the orbit
  g.from_string("(0 1)");
  EXPECT_EQ(0, t.strip(g, &h));
  EXPECT_STREQ("(0 1)", g.to_string().c_str() );
}

TEST_F(FundamentalSchreierTreesTest, StripSmall) {
  FundamentalSchreierTrees t;
  PermutationWord g;
  PermutationWord h;

  // test final return when something actually happens
  PermutationWord w1;
  w1.from_string("(0 1 2)");
  PermutationWord w2;
  w2.from_string("(0 1)");
  t.append_to_base(0);
  t.add_generator(w1);
  t.add_generator(w2);
  t.build_trees();
  // tree can be either {2 : w1, 1 : w2} or {1 : w1, 2 : w1}

  // try to sift (0 2)
  g.from_string("(0 2)");
  EXPECT_EQ(1, t.strip(g, &h));
  // we don't know the exact form of h, we just know that it must fix 0
  EXPECT_EQ(0, h.get_image(0));

  // try to sift (0 1), should strip to the identity
  g.from_string("(0 1)");
  EXPECT_EQ(1, t.strip(g, &h));
  EXPECT_TRUE(h.is_identity());

  // try to sift (0 2 1), must strip to identity
  g.from_string("(0 2 1)");
  EXPECT_EQ(1, t.strip(g, &h));
  EXPECT_TRUE(h.is_identity());
}

}  // namespace fssw
