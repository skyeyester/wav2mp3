#include "lock_guard.h"
#include "mymutex.h"
#include "thread.h"
#include "gtest/gtest.h"

#include <memory>

using namespace std;
using namespace wav2mp3;

void
delay()
{
  for (int i = 0; i < 100; ++i)
    auto mem = make_unique<char[]>(256);
}

TEST(MyMutex, IsLockable)
{
  mymutex m;

  wav2mp3::lock_guard<mymutex> guard(m);
}

TEST(MyMutex, IsOftenLockable)
{
  mymutex m;

  auto guard = make_unique<wav2mp3::lock_guard<mymutex>>(m);
  for (int i = 0; i < 10; ++i) {
    guard.reset();
    guard = make_unique<wav2mp3::lock_guard<mymutex>>(m);
  }
}

TEST(MyMutex, SynchronizesReadingThread)
{
  mymutex m;

  auto value{ 0 };
  auto guard{ make_unique<wav2mp3::lock_guard<mymutex>>(m) };

  thread t{ [&]() {
    auto wait = make_unique<wav2mp3::lock_guard<mymutex>>(m);
    ASSERT_EQ(value, 1);
  } };

  delay();
  ++value;
  guard.reset();
}

TEST(MyMutex, SynchronizesWritingThread)
{
  mymutex m;

  auto value{ 0 };
  auto guard{ make_unique<wav2mp3::lock_guard<mymutex>>(m) };

  thread t{ [&]() {
    delay();

    ASSERT_EQ(value, 0);
    guard.reset();
  } };

  auto wait = make_unique<wav2mp3::lock_guard<mymutex>>(m);
  ++value;
}
