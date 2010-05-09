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
  SchreierTree t(0);
}

}  // namespace fssw
