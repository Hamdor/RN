#ifndef RNA1_WORKER_IMPL_HPP
#define RNA1_WORKER_IMPL_HPP

#include "thread_t.hpp"

namespace rna1 {

class worker_impl : public thread_t {
 protected:
  void* exec(void*);
};

} // namespace rna1


#endif // RNA1_WORKER_IMPL_HPP
