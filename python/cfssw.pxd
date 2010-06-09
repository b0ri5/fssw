cdef extern from "<string>" namespace "std":
  cdef cppclass string:
    string()
    string(char *)
    char *c_str()

cdef extern from "fssw/MapPermutation.h" namespace "fssw":
  cdef cppclass MapPermutation:
    bint from_string(string s)
    string to_string()

cdef extern from "fssw/FundamentalSchreierTrees.h" namespace "fssw":
  cdef cppclass FundamentalSchreierTrees:
    bint add_generator(string s)
    void append_to_base(int b)
    bint build_trees()
    bint is_strongly_generated()
    string to_string()
    bint schreier_sims()