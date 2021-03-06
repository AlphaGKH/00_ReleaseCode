#ifndef TIME_RATE_H_
#define TIME_RATE_H_

#include "time/duration.h"
#include "time/time.h"

class Rate {
 public:
  explicit Rate(double frequency);
  explicit Rate(uint64_t nanoseconds);
  explicit Rate(const Duration&);
  void Sleep();
  void Reset();
  Duration CycleTime() const;
  Duration ExpectedCycleTime() const { return expected_cycle_time_; }

 private:
  Time start_;
  Duration expected_cycle_time_;
  Duration actual_cycle_time_;
};

#endif  // SPIDER_TIME_RATE_H_
