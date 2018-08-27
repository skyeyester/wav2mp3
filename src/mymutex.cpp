#include "mymutex.h"

#include <system_error>

#ifdef _WIN32
#include <time.h>
#define HAVE_STRUCT_TIMESPEC 1
#endif
#include <pthread.h>

using namespace std;

namespace wav2mp3 {
class mymutex::impl
{
public:
  impl()
  {
    if (auto const error_code = pthread_mutex_init(&mutex_, nullptr))
      throw new system_error(error_code, system_category());
  }
  ~impl() { pthread_mutex_destroy(&mutex_); }

  void lock() { pthread_mutex_lock(&mutex_); }
  void unlock() { pthread_mutex_unlock(&mutex_); }

private:
  pthread_mutex_t mutex_;
};

mymutex::mymutex()
  : impl_{ make_unique<impl>() }
{
}

mymutex::~mymutex() = default;

void
mymutex::lock()
{
  impl_->lock();
}

void
mymutex::unlock()
{
  impl_->unlock();
}
}
