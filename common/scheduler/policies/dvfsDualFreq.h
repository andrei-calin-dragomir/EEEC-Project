/**
 * This header implements the dual frequency DVFS policy
 */

#ifndef __DVFS_DUALFREQ_H
#define __DVFS_DUALFREQ_H

#include <vector>
#include "dvfspolicy.h"

class DVFSDualFreq : public DVFSPolicy {
public:
    DVFSDualFreq(const PerformanceCounters *performanceCounters, int coreRows, int coreColumns, int freqOne, int freqTwo);
    virtual std::vector<int> getFrequencies(const std::vector<int> &oldFrequencies, const std::vector<bool> &activeCores);

private:
    const PerformanceCounters *performanceCounters;
    unsigned int coreRows;
    unsigned int coreColumns;
    int freqOne;
    int freqTwo;
};

#endif