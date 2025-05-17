/**
* This header implements a policy that maps new applications to the coldest
core
* and migrates threads from hot cores to the coldest cores.
*/
#ifndef __FINALMIGRATION_H
#define __FINALMIGRATION_H
#include <vector>
#include <queue>
#include "mappingpolicy.h"
#include "migrationpolicy.h"
#include "performance_counters.h"
class FinalMigration : public MappingPolicy, public MigrationPolicy
{
public:
    FinalMigration(const PerformanceCounters *performanceCounters, 
                int coreRows, int coreColumns,
                float worryTemperature, int maxFrequency,
                float criticalTemperature);
    virtual std::vector<int> map(String taskName, int taskCoreRequirement,
                                 const std::vector<bool> &availableCores,
                                 const std::vector<bool> &activeCores);
    virtual std::vector<migration> migrate(
        SubsecondTime time, const std::vector<int> &taskIds,
        const std::vector<bool> &activeCores);

private:
    const PerformanceCounters *performanceCounters;
    static const int WINDOW_SIZE = 5;
    unsigned int coreRows;
    unsigned int coreColumns;
    float criticalTemperature;
    float worryTemperature;
    int maxFrequency;
    std::vector<std::queue<float>> tempQueues;
    int getColdestCore(const std::vector<bool> &availableCores);
    void logTemperatures(const std::vector<bool> &availableCores);
    float averageTemperature(int core);
};
#endif
