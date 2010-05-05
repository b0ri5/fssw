/*
    Copyright 2010 Greg Tener and Elena Erbiceanu
    Released under the Lesser General Public License v3.
*/

#include <fssw/MapPermutation.h>

#include <gtest/gtest.h>

namespace fssw {

class MapPermutationTest : public ::testing::Test {
 public:
  void verify_mapping(MapPermutation g, int first, int second) {
    EXPECT_EQ(g.get_image(first), second);
    EXPECT_EQ(g.get_inverse_image(second), first);
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

}  // namespace fssw


