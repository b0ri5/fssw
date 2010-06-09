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
    for (set<int>::const_iterator it = orbit.begin(); it != orbit.end(); ++it) {
      EXPECT_TRUE(t_ptr->is_in_orbit(*it));
    }

    // check that one plus the last
    EXPECT_FALSE(t_ptr->is_in_orbit(*orbit.rbegin() + 1));
    EXPECT_FALSE(t_ptr->path_to_root(*orbit.rbegin() + 1, &w));

    // check that "t"'s orbit is in "orbit"
    for (SchreierTree::iterator orbit_it = t_ptr->get_orbit_iterator();
         orbit_it.not_at_end(); ++orbit_it) {
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

  EXPECT_TRUE(orbit_it.not_at_end());
  EXPECT_EQ(*orbit_it, 0);

  orbit_it.append(2);

  EXPECT_TRUE(orbit_it.not_at_end());
  EXPECT_EQ(*orbit_it, 0);

  ++orbit_it;

  EXPECT_TRUE(orbit_it.not_at_end());
  EXPECT_EQ(*orbit_it, 2);

  ++orbit_it;

  EXPECT_FALSE(orbit_it.not_at_end());
}

TEST_F(SchreierTreeTest, OrbitIteratorTestReal) {
  SchreierTree t;

  t.set_root(1);
  PermutationWord w;
  w.from_string("(1 5 7)(2 6 8)");
  t.add_generator(w);
  t.build_tree();

  vector<int> v;  // to check what's iterated over

  for (SchreierTree::iterator orbit_it = t.get_orbit_iterator();
      orbit_it.not_at_end(); ++orbit_it) {
    int a = *orbit_it;
    v.push_back(a);

    if (a == 5) {
      orbit_it.append(2);
    }
  }

  ASSERT_EQ(4, v.size());
  EXPECT_EQ(1, v[0]);
  EXPECT_EQ(5, v[1]);
  EXPECT_EQ(2, v[2]);
  EXPECT_EQ(7, v[3]);
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

// found via python unittest and some debugging
// using the generators (1 5 7)(2 6 8) and (1 6 7)(2 5 8)
// does not seem to put 8 in the orbit
TEST_F(SchreierTreeTest, BuildTree1) {
  SchreierTree t;
  set<int> orbit;

  t.set_root(1);
  PermutationWord w1;
  w1.from_string("(1 5 7)(2 6 8)");
  t.add_generator(w1);
  t.build_tree();
  orbit.insert(1);
  orbit.insert(5);
  orbit.insert(7);
  verify_orbit(&t, orbit);

  PermutationWord w2;
  w2.from_string("(1 6 7)(2 5 8)");
  t.add_generator(w2);
  t.build_tree();
  orbit.insert(2);
  orbit.insert(6);
  orbit.insert(8);
  verify_orbit(&t, orbit);

  printf("%s\n", t.to_string().c_str());
}

}  // namespace fssw
