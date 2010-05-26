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

  EXPECT_EQ(t.get_base(0), 2);
  EXPECT_EQ(t.get_base(1), -1);
}

TEST_F(FundamentalSchreierTreesTest, DistributeGenerator) {
  FundamentalSchreierTrees t;

  t.append_to_base(0);
  t.append_to_base(1);
  ASSERT_EQ(2, t.get_base_length());

  // the identity should go in every tree
  MapPermutation s;
  s.from_string("()");
  t.add_generator(s);
  //t.distribute_generators();
  // should show up in both trees
  EXPECT_TRUE(t.get_tree(0)->has_generator(s));
  EXPECT_TRUE(t.get_tree(1)->has_generator(s));

  // s moves the first base element
  s.from_string("(0 2)");
  t.add_generator(s);
  // should show up in the first tree only
  EXPECT_TRUE(t.get_tree(0)->has_generator(s));
  EXPECT_FALSE(t.get_tree(1)->has_generator(s));

  // s moves the second base element
  s.from_string("(1 3)");
  t.add_generator(s);
  // should show up in both trees
  EXPECT_TRUE(t.get_tree(0)->has_generator(s));
  EXPECT_TRUE(t.get_tree(1)->has_generator(s));
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
  MapPermutation s;
  s.from_string("(0 1 2)");
  t.add_generator(s);
  s.from_string("(0 1)");
  t.add_generator(s);
  t.append_to_base(0);

  ASSERT_TRUE(t.build_trees());
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

  // try to sift (0 1 2), can strip to identity or (0)(1 2)
  g.from_string("(0 1 2)");
  EXPECT_EQ(1, t.strip(g, &h));
  EXPECT_EQ(0, h.get_image(0));
}

}  // namespace fssw
