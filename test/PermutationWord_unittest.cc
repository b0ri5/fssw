/*
    Copyright 2010 Greg Tener and Elena Erbiceanu
    Released under the Lesser General Public License v3.
*/

#include <fssw/PermutationWord.h>

#include <gtest/gtest.h>

namespace fssw {

class PermutationWordTest : public ::testing::Test {
 public:
};

TEST_F(PermutationWordTest, Instantiation) {
  PermutationWord w;
}

TEST_F(PermutationWordTest, FromString) {
  PermutationWord w;
  EXPECT_TRUE(w.from_string("()"));
  EXPECT_TRUE(w.is_identity());

  EXPECT_TRUE(w.from_string("(0 2)"));
  EXPECT_EQ(w.get_image(0), 2);
  EXPECT_EQ(w.get_image(2), 0);
  EXPECT_EQ(w.get_image(1), 1);

  EXPECT_TRUE(w.from_string("(0 2)*(1 3)"));
  EXPECT_EQ(w.get_image(0), 2);
  EXPECT_EQ(w.get_image(2), 0);
  EXPECT_EQ(w.get_image(1), 3);
  EXPECT_EQ(w.get_image(3), 1);

  EXPECT_TRUE(w.from_string("(0 2 1)^{-1}*(1 3)"));
  // corresponds to (0 1 2)*(1 3)
  EXPECT_EQ(w.get_image(0), 3);
  EXPECT_EQ(w.get_image(1), 2);
  EXPECT_EQ(w.get_image(2), 0);
  EXPECT_EQ(w.get_image(3), 1);

  EXPECT_FALSE(w.from_string("(0 2 1)^{-1} k"));
  EXPECT_TRUE(w.is_identity());

  EXPECT_FALSE(w.from_string("(0 2 1)^{-1}*  **(1 2)"));
  EXPECT_TRUE(w.is_identity()) << w.to_string();

  EXPECT_FALSE(w.from_string("**(0 2 1)^{-1}*(1 2)"));
  EXPECT_TRUE(w.is_identity());
}

TEST_F(PermutationWordTest, Evaluate) {
  PermutationWord w;

  ASSERT_TRUE(w.from_string("(0 2)*(2 3)"));
  EXPECT_EQ(w.get_image(0), 3);
  EXPECT_EQ(w.get_image(2), 0);
  EXPECT_EQ(w.get_image(1), 1);
  EXPECT_EQ(w.get_image(3), 2);
}

TEST_F(PermutationWordTest, EvaluateWithInverse) {
  PermutationWord w;

  ASSERT_TRUE(w.from_string("(0 2)*(2 1 3)^{-1}*(0 1)(2 4)"));
  EXPECT_EQ(w.get_image(0), 3);
  EXPECT_EQ(w.get_image(2), 1);
  EXPECT_EQ(w.get_image(1), 4);
  EXPECT_EQ(w.get_image(3), 0);
  EXPECT_EQ(w.get_image(4), 2);
}

TEST_F(PermutationWordTest, ComposeMapPermutationAndInverse) {
  PermutationWord w;
  MapPermutation g, h;
  ASSERT_TRUE(g.from_string("(0 2)"));
  ASSERT_TRUE(h.from_string("(2 3)"));

  w.compose(g);
  w.compose_inverse(h);
  w.compose(g);
  w.compose(g);

  EXPECT_EQ(w.to_string(), "(0 2)*(2 3)^{-1}*(0 2)*(0 2)");
}

TEST_F(PermutationWordTest, ComposeWordAndInverse) {
  PermutationWord w1, w2, w;
  ASSERT_TRUE(w1.from_string("(0 2)*(1 2)^{-1}"));
  ASSERT_TRUE(w2.from_string("(2 3)^{-1}"));

  w.compose(w1);
  w.compose(w2);
  EXPECT_EQ(w.to_string(), "(0 2)*(1 2)^{-1}*(2 3)^{-1}");

  w.clear();
  w.compose_inverse(w2);
  w.compose_inverse(w1);
  EXPECT_EQ(w.to_string(), "(2 3)*(1 2)*(0 2)^{-1}");
}

TEST_F(PermutationWordTest, GetFirstMovedElement) {
  PermutationWord w;

  EXPECT_EQ(w.get_first_moved_element(), -1);
  ASSERT_TRUE(w.from_string("(0 1)"));

  EXPECT_EQ(w.get_first_moved_element(), 0);
  ASSERT_TRUE(w.from_string("(0 1)*(1 0)"));

  EXPECT_EQ(w.get_first_moved_element(), -1);
  ASSERT_TRUE(w.from_string("(0 1)*(1 0)*(2 3)"));

  EXPECT_EQ(w.get_first_moved_element(), 2);
  ASSERT_TRUE(w.from_string("(0 1)*(0 2 1)*(2 1)"));

  EXPECT_EQ(w.get_first_moved_element(), -1);
}

TEST_F(PermutationWordTest, IsEquivalent) {
  PermutationWord w1, w2;

  // basic cases: identity
  EXPECT_TRUE(w1.is_equivalent(w2));
  EXPECT_TRUE(w2.is_equivalent(w1));

  // single permutation
  ASSERT_TRUE(w1.from_string("(0 1)"));
  ASSERT_TRUE(w2.from_string("(0 1)"));
  EXPECT_TRUE(w1.is_equivalent(w2));
  EXPECT_TRUE(w2.is_equivalent(w1));

  // composed permutations
  ASSERT_TRUE(w1.from_string("(0 1)*(1 2)"));
  ASSERT_TRUE(w2.from_string("(0 2 1)"));
  EXPECT_TRUE(w1.is_equivalent(w2));

  // non-equiv
  ASSERT_TRUE(w1.from_string("(0 1 2)"));
  ASSERT_TRUE(w2.from_string("(0 3)"));
  EXPECT_FALSE(w1.is_equivalent(w2));
  EXPECT_FALSE(w2.is_equivalent(w1));
}

}  // namespace fssw


