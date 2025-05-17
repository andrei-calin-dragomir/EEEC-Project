#ifndef __FINAL_DVFS_H
#define __FINAL_DVFS_H
#include <vector>

#include "dvfspolicy.h"
#include "performance_counters.h"
class FinalDVFS : public DVFSPolicy {
   public:
    FinalDVFS(const PerformanceCounters *performanceCounters, int coreRows,
              int coreColumns, int minFrequency, int maxFrequency,
              int frequencyStepSize, float upThreshold, float downThreshold,
              float worryTemperature, float noWorryTemperature,
              float dtmCriticalTemperature, float dtmRecoveredTemperature);
    virtual std::vector<int> getFrequencies(
        const std::vector<int> &oldFrequencies,
        const std::vector<bool> &activeCores);

   private:
    const PerformanceCounters *performanceCounters;
    unsigned int coreRows;
    unsigned int coreColumns;
    int minFrequency;
    int maxFrequency;
    int frequencyStepSize;
    float upThreshold;
    float downThreshold;
    float worryTemperature;
    float noWorryTemperature;
    float dtmCriticalTemperature;
    float dtmRecoveredTemperature;
    bool in_throttle_mode = false;
    bool throttle();
};
#endif