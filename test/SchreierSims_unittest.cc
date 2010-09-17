/*
    Copyright 2010 Greg Tener and Elena Erbiceanu
    Released under the Lesser General Public License v3.
*/

/*
 * This unittest tests the schreier sims portions of FundamentalSchreierTrees.
 * It uses external data in the "data" directory as ground truth.
 */

#include <limits.h>

#include <fssw/FundamentalSchreierTrees.h>

#include <gtest/gtest.h>
#include <gflags/gflags.h>

#include <fssw/PermutationWord.h>
#include <fssw/SchreierTree.h>

#include <string>
#include <fstream>
#include <sstream>

using std::stringstream;
using std::ifstream;
using std::string;
using std::getline;

// gflags flags
DEFINE_int64(max_order, -1,
  "The maximum order of the groups considered");

namespace fssw {

static bool is_comment(const string &line) {
  int pos = line.find_first_not_of(" \t\n\r", 0);

  return pos != string::npos && line[pos] == '#';
}

class SchreierSimsTest : public ::testing::Test {
 public:
  void TearDown() {
    MapPermutationAllocator::clear_memory();
  }

  void verify_datafile(string filename,
      int max_count = -1) {
    ifstream in(filename.c_str());

    if (in.fail()) {
      fprintf(stderr, "couldn't open data file %s, ", filename.c_str());
      fprintf(stderr, "be sure to run from fssw's root\n");
      exit(1);
    }

    // now read each group
    string line;
    int count;

    getline(in, line);

    // read in the groups, the format is <order>\n<gen1>\n...\n
    while (line != "") {
      // if we're reading in a comment, print it and read the next line
      while (is_comment(line)) {
        printf("%s\n", line.c_str());
        getline(in, line);
      }

      // we assume we've read the order now
      stringstream ss(line);
      long long int order = 0;
      ss >> order;

      // if the order is too large for C++'s puny fixed-precision integers
      if (ss.fail()) {
        order = -1;
        ss.clear();
      }

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
      ASSERT_FALSE(t.schreier_sims());  // should not change "t" and be false

      printf("\n");
      getline(in, line);  // read in the first line of the next "group"

      // quit if we've gone too far
      if (max_count >= 0 && count > max_count) {
        line = "";
      }

      // quit if we've reached too high of an order
      if (FLAGS_max_order >= 0 && order > FLAGS_max_order) {
        line = "";
      }
    }
  }
};

TEST_F(SchreierSimsTest, SmallGroupsAll) {
  verify_datafile("test/data/small_groups_minimum_generators_200.txt");
}

TEST_F(SchreierSimsTest, Atlasrep10000) {
  verify_datafile("test/data/atlasrep_10000.txt");
}

}  // namespace fssw

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  google::ParseCommandLineFlags(&argc, &argv, true);

  return RUN_ALL_TESTS();
}
