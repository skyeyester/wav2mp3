#include <memory>

namespace wav2mp3 {
class mymutex
{
public:
  mymutex();
  mymutex(mymutex const& other) = delete;
  mymutex& operator=(mymutex const& other) = delete;
  ~mymutex();

  void lock();
  void unlock();

private:
  class impl;
  std::unique_ptr<impl> impl_;
};
}
