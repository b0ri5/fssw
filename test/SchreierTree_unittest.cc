/*
    Copyright 2010 Greg Tener and Elena Erbiceanu
    Released under the Lesser General Public License v3.
*/

#include <fssw/SchreierTree.h>

#include <gtest/gtest.h>


namespace fssw {

class SchreierTreeTest : public ::testing::Test {
 public:
};

TEST_F(SchreierTreeTest, Instantiation) {
  SchreierTree t;
  t.set_root(0);
}

TEST_F(SchreierTreeTest, AddsGenerator) {
  SchreierTree t;
  t.set_root(2);

  // one edge
  PermutationWord w;
  ASSERT_TRUE(w.from_string("(0 2)"));
  EXPECT_TRUE(t.add_generator(w));

  EXPECT_TRUE(t.is_in_orbit(0));

  // multi-edge
  ASSERT_TRUE(w.from_string("(1 2 3 4)(5 6)"));
  EXPECT_TRUE(t.add_generator(w));

  EXPECT_TRUE(t.is_in_orbit(1));
  EXPECT_TRUE(t.is_in_orbit(3));
  EXPECT_TRUE(t.is_in_orbit(4));
  EXPECT_FALSE(t.is_in_orbit(5));
  EXPECT_FALSE(t.is_in_orbit(6));
}

TEST_F(SchreierTreeTest, PathToRoot) {
  SchreierTree t;
  t.set_root(2);

  PermutationWord w1, w2, w3;
  ASSERT_TRUE(w1.from_string("(0 2)"));
  EXPECT_TRUE(t.add_generator(w1));
  ASSERT_TRUE(w2.from_string("(1 2 3 4)(5 6)"));
  EXPECT_TRUE(t.add_generator(w2));
  ASSERT_TRUE(w3.from_string("(0 2 5)"));
  EXPECT_TRUE(t.add_generator(w3));

  PermutationWord path;

  // not in orbit
  EXPECT_FALSE(t.path_to_root(6, &path));
  EXPECT_EQ(path.to_string(), "()");

  // root
  EXPECT_TRUE(t.path_to_root(2, &path));
  EXPECT_EQ(path.to_string(), "()");

  // single edge
  EXPECT_TRUE(t.path_to_root(0, &path));
  EXPECT_EQ(path.to_string(), "(0 2)");

  // multi-edge, repeated
  EXPECT_TRUE(t.path_to_root(4, &path));
  EXPECT_EQ(path.to_string(), "(1 2 3 4)(5 6)*(1 2 3 4)(5 6)");

  // multi-edge, branching
  EXPECT_TRUE(t.path_to_root(5, &path));
  EXPECT_EQ(path.to_string(), "(0 2 5)*(0 2)");
}

}  // namespace fssw
