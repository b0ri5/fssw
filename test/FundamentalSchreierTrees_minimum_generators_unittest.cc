/*
    Copyright 2010 Greg Tener and Elena Erbiceanu
    Released under the Lesser General Public License v3.
*/

#include <fssw/FundamentalSchreierTrees.h>

#include <gtest/gtest.h>

#include <fssw/PermutationWord.h>
#include <fssw/SchreierTree.h>

#include <string>
#include <fstream>
#include <sstream>

using std::stringstream;
using std::ifstream;
using std::string;
using std::getline;

namespace fssw {

class FundamentalSchreierTrees_minimum_generatorsTest : public ::testing::Test {
 public:
  void TearDown() {
    MapPermutationAllocator::clear_memory();
  }
};

TEST_F(FundamentalSchreierTrees_minimum_generatorsTest, SchreierSims) {
  string filename =  \
      "test/data/small_groups_minimum_generators_200.txt";

  ifstream in(filename.c_str());

  if (in.fail()) {
    fprintf(stderr, "couldn't open data file %s\n", filename.c_str());
    exit(1);
  }

  // now read each group
  string line;

  getline(in, line);

  // read in the groups, the format is <order>\n<gen1>\n...\n
  while (line != "") {
    long long int order = 0;
    stringstream ss(line);
    ss >> order;

    printf("%lld\n", order);

    FundamentalSchreierTrees t;

    getline(in, line);  // read the first generator

    while (line != "") {
      printf("%s\n", line.c_str());
      t.add_generator(line);
      getline(in, line);
    }

    // call schreier sims
    t.schreier_sims();
    // test that it's strongly generated
    ASSERT_TRUE(t.is_strongly_generated());
    ASSERT_EQ(order, t.order());
    ASSERT_FALSE(t.schreier_sims());  // should not change "t" and return false

    printf("\n");
    getline(in, line);  // read in the next order

    if (order > 100) {
      line = "";
    }
  }
}

}  // namespace fssw
