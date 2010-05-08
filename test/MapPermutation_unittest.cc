/*
    Copyright 2010 Greg Tener and Elena Erbiceanu
    Released under the Lesser General Public License v3.
*/

#include <fssw/MapPermutation.h>

#include <gtest/gtest.h>

namespace fssw {

class MapPermutationTest : public ::testing::Test {
 public:
  void verify_mapping(MapPermutation g, int a, int a_image) {
    EXPECT_EQ(g.get_image(a), a_image);
    EXPECT_EQ(g.get_inverse_image(a_image), a);
  }
};

TEST_F(MapPermutationTest, Instantiation) {
  MapPermutation g;
}

TEST_F(MapPermutationTest, FromStringIdentity) {
  MapPermutation g;

  EXPECT_TRUE(g.from_string("()"));
  EXPECT_TRUE(g.is_identity());

  EXPECT_TRUE(g.from_string("(0)"));
  EXPECT_TRUE(g.is_identity());

  EXPECT_TRUE(g.from_string("(0)(1)"));
  EXPECT_TRUE(g.is_identity());
}

TEST_F(MapPermutationTest, FromStringMalformed) {
  MapPermutation g;

  EXPECT_FALSE(g.from_string(""));
  EXPECT_TRUE(g.is_identity());

  EXPECT_FALSE(g.from_string("("));
  EXPECT_TRUE(g.is_identity());

  EXPECT_FALSE(g.from_string(")"));
  EXPECT_TRUE(g.is_identity());

  EXPECT_FALSE(g.from_string("( )"));
  EXPECT_TRUE(g.is_identity());

  EXPECT_FALSE(g.from_string("(0 0)"));
  EXPECT_TRUE(g.is_identity());

  EXPECT_FALSE(g.from_string("(0 2)(1 2)"));
  EXPECT_TRUE(g.is_identity());

  EXPECT_FALSE(g.from_string("(0 2) (1 3)"));
  EXPECT_TRUE(g.is_identity());

  EXPECT_FALSE(g.from_string("(0 2)(1  3)"));
  EXPECT_TRUE(g.is_identity());
}

TEST_F(MapPermutationTest, FromStringCheckMapping) {
  MapPermutation g;

  EXPECT_TRUE(g.from_string("(0 2 4)(1 5)"));
  verify_mapping(g, 0, 2);
  verify_mapping(g, 2, 4);
  verify_mapping(g, 4, 0);
  verify_mapping(g, 1, 5);
  verify_mapping(g, 5, 1);
  verify_mapping(g, 3, 3);
}

TEST_F(MapPermutationTest, FromStringAllowSurroundingWhitespace) {
  MapPermutation g;
  EXPECT_TRUE(g.from_string("  ()"));
  EXPECT_TRUE(g.is_identity());

  EXPECT_TRUE(g.from_string("() "));
  EXPECT_TRUE(g.is_identity());

  EXPECT_TRUE(g.from_string("  (0 2)(1 3)  "));
  verify_mapping(g, 0, 2);
  verify_mapping(g, 2, 0);
  verify_mapping(g, 1, 3);
  verify_mapping(g, 3, 1);
}

TEST_F(MapPermutationTest, ToString) {
  MapPermutation g;

  g.from_string("()");
  EXPECT_EQ(g.to_string(), "()");

  g.from_string("(0 1)");
  EXPECT_EQ(g.to_string(), "(0 1)");

  g.from_string("  (0 2 4)(1 3)  ");
  EXPECT_EQ(g.to_string(), "(0 2 4)(1 3)");
}

TEST_F(MapPermutationTest, ComposeSingleCycle) {
  MapPermutation g, h, g_h;
  g.from_string("(0 1)");
  h.from_string("(1 2)");
  g_h.from_string("(0 2 1)");
  g.compose(h);

  EXPECT_TRUE(g_h.is_equal(g));
}

TEST_F(MapPermutationTest, ComposeMultiCycleWithSingleCycle) {
  MapPermutation g, h, g_h;
  g.from_string("(0 1)(2 3)");
  h.from_string("(1 2)");
  g_h.from_string("(0 2 3 1)");
  g.compose(h);

  EXPECT_TRUE(g_h.is_equal(g));

  g.from_string("(0 1)");
  h.from_string("(1 2)(0 3)");
  g_h.from_string("(0 2 1 3)");
  g.compose(h);

  EXPECT_TRUE(g_h.is_equal(g));

  g.from_string("(0 1)(2 4)");
  h.from_string("(1 2)(3 4)");
  g_h.from_string("(0 2 3 4 1)");
  g.compose(h);

  EXPECT_TRUE(g_h.is_equal(g));
}

TEST_F(MapPermutationTest, ComposeCancellingCycle) {
  MapPermutation g, h, g_h;

  g.from_string("(0 1)(2 3)");
  h.from_string("(1 0)(2 4)");
  g_h.from_string("(2 3 4)");
  g.compose(h);

  EXPECT_TRUE(g_h.is_equal(g));
}

TEST_F(MapPermutationTest, ComposeMultiCycle) {
  MapPermutation g, h, g_h;

  g.from_string("(0 1)(2 4)");
  h.from_string("(1 2)(3 4)");
  g_h.from_string("(0 2 3 4 1)");
  g.compose(h);

  EXPECT_TRUE(g_h.is_equal(g));

  g.from_string("(0 1)(2 3 4)");
  h.from_string("(1 2)(3 0)");
  g_h.from_string("(0 2)(1 3 4)");
  g.compose(h);

  EXPECT_TRUE(g_h.is_equal(g));
}
}  // namespace fssw


