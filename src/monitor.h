#include "lock_guard.h"
#include "mymutex.h"

namespace wav2mp3 {
template <class T>
class monitor
{
public:
  explicit monitor(T t)
    : t_{ t }
  {
  }

  template <class F>
  auto operator()(F f) const
  {
    lock_guard<mymutex> hold{ m_ };
    return f(t_);
  }

private:
  mutable T     t_;
  mutable mymutex m_;
};
}
