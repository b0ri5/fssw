/*
    Copyright 2010 Greg Tener and Elena Erbiceanu
    Released under the Lesser General Public License v3.
*/

#include <fssw/SchreierTree.h>

#include <gtest/gtest.h>

#include <set>

using std::set;

namespace fssw {

class SchreierTreeTest : public ::testing::Test {
 public:
  // verify that "t" has the orbit correct by ensuring that it's equal to
  // orbit
  void verify_orbit(SchreierTree *t_ptr, const set<int> &orbit) {
    PermutationWord w;

    // check one less than the smallest element
    EXPECT_FALSE(t_ptr->is_in_orbit(*orbit.begin() - 1) );
    EXPECT_FALSE(t_ptr->path_to_root(*orbit.begin() - 1, &w));

    // check that "orbit" is in "t"'s orbit
    for (set<int>::iterator it = orbit.begin(); it != orbit.end(); ++it) {
      EXPECT_TRUE(t_ptr->is_in_orbit(*it));
    }

    // check that one plus the last
    EXPECT_FALSE(t_ptr->is_in_orbit(*orbit.rbegin() + 1));
    EXPECT_FALSE(t_ptr->path_to_root(*orbit.rbegin() + 1, &w));

    // check that "t"'s orbit is in "orbit"
    for (SchreierTree::iterator orbit_it = t_ptr->get_orbit_iterator();
         orbit_it.has_next(); ++orbit_it) {
      EXPECT_TRUE(orbit.find(*orbit_it) != orbit.end());

      // check that path_to_root works as expected
      EXPECT_TRUE(t_ptr->path_to_root(*orbit_it, &w));
      EXPECT_EQ(w.get_image(*orbit_it), t_ptr->get_root());
    }
  }
};

TEST_F(SchreierTreeTest, Instantiation) {
  SchreierTree t;
  t.set_root(0);
}

TEST_F(SchreierTreeTest, OrbitIteratorTests) {
  SchreierTree t;

  t.set_root(0);

  SchreierTree::iterator orbit_it = t.get_orbit_iterator();

  EXPECT_TRUE(orbit_it.has_next());
  EXPECT_EQ(*orbit_it, 0);

  ++orbit_it;

  EXPECT_FALSE(orbit_it.has_next());

  orbit_it.append(2);

  EXPECT_TRUE(orbit_it.has_next());
  EXPECT_EQ(*orbit_it, 2);

  ++orbit_it;

  EXPECT_FALSE(orbit_it.has_next());
}

// do a simple test of adding three generators
TEST_F(SchreierTreeTest, BuildTreeSmall) {
  SchreierTree t;
  t.set_root(0);
  set<int> orbit;

  PermutationWord w1, w2, w3;
  ASSERT_TRUE(w1.from_string("(0 1)"));
  ASSERT_TRUE(w2.from_string("(2 3)"));
  ASSERT_TRUE(w3.from_string("(1 2)"));
  t.add_generator(w1);
  t.add_generator(w2);
  t.add_generator(w3);
  t.build_tree();

  for (int i = 0; i <= 3; i++) {
    orbit.insert(i);
  }

  verify_orbit(&t, orbit);
}

// do the same as AddGeneratorSmall but incrementally (build_tree() after each
// add_generator)
TEST_F(SchreierTreeTest, BuildTreeSmallIncremental) {
  SchreierTree t;
  t.set_root(0);
  set<int> orbit;

  PermutationWord w1;
  ASSERT_TRUE(w1.from_string("(0 1)"));
  t.add_generator(w1);
  EXPECT_TRUE(t.build_tree());
  orbit.insert(0);
  orbit.insert(1);

  verify_orbit(&t, orbit);

  PermutationWord w2;
  ASSERT_TRUE(w2.from_string("(2 3)"));
  t.add_generator(w2);
  EXPECT_FALSE(t.build_tree());

  verify_orbit(&t, orbit);

  PermutationWord w3;
  ASSERT_TRUE(w3.from_string("(1 2)"));
  t.add_generator(w3);
  EXPECT_TRUE(t.build_tree());
  orbit.insert(2);
  orbit.insert(3);

  verify_orbit(&t, orbit);
}

}  // namespace fssw
