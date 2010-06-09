cimport cfssw
cimport python_exc

cdef class MapPermutation:
  cdef cfssw.MapPermutation *this_ptr
  
  def __cinit__(self):
    self.this_ptr = new cfssw.MapPermutation()
    if self.this_ptr is NULL:
      python_exc.PyErr_NoMemory()
      
  def __dealloc__(self):
    if not self.this_ptr is NULL:
      del self.this_ptr
      
  def to_string(self):
    return self.this_ptr.to_string().c_str()
  
  def from_string(self, s):
    cdef char *sz = s
    return self.this_ptr.from_string(cfssw.string(sz))

cdef class FundamentalSchreierTrees:
  cdef cfssw.FundamentalSchreierTrees *this_ptr
  
  def __cinit__(self):
    self.this_ptr = new cfssw.FundamentalSchreierTrees()
    if self.this_ptr is NULL:
      python_exc.PyErr_NoMemory()
      
  def __dealloc__(self):
    if not self.this_ptr is NULL:
      del self.this_ptr
      
  def add_generator(self, s):
    sz = s
    return self.this_ptr.add_generator(cfssw.string(sz))
  
  def to_string(self):
    return self.this_ptr.to_string().c_str()
  
  def append_to_base(self, b):
    self.this_ptr.append_to_base(b)
  
  def build_trees(self):
    return self.this_ptr.build_trees()
  
  def is_strongly_generated(self):
    return self.this_ptr.is_strongly_generated()
  
  def schreier_sims(self):
    return self.this_ptr.schreier_sims()