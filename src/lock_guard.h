namespace wav2mp3 {
template <class MyMutex>
class lock_guard
{
public:
  explicit lock_guard(MyMutex& m)
    : m_{ m }
  {
    m_.lock();
  }
  ~lock_guard() { m_.unlock(); }

  lock_guard(lock_guard const& other) = delete;
  lock_guard& operator=(lock_guard const& other) = delete;

private:
  MyMutex& m_;
};
}
