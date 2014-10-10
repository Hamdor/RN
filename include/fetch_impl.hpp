
#ifndef RNA1_FETCH_IMPL_HPP
#define RNA1_FETCH_IMPL_HPP

#include "thread_t.hpp"

namespace rna1 {

class fetch_impl : public thread_t {
 protected:
  void* exec(void*);
};

} // namespace rna1


#endif // RNA1_FETCH_IMPL_HPP
