#include "finalDVFS.h"

#include <iomanip>
#include <iostream>
using namespace std;
FinalDVFS::FinalDVFS(const PerformanceCounters *performanceCounters,
                            int coreRows, int coreColumns, int minFrequency,
                            int maxFrequency, int frequencyStepSize,
                            float upThreshold, float downThreshold,
                            float worryTemperature, float noWorryTemperature,
                            float dtmCriticalTemperature,
                            float dtmRecoveredTemperature)
    : performanceCounters(performanceCounters),
      coreRows(coreRows),
      coreColumns(coreColumns),
      minFrequency(minFrequency),
      maxFrequency(maxFrequency),
      frequencyStepSize(frequencyStepSize),
      upThreshold(upThreshold),
      downThreshold(downThreshold),
      worryTemperature(worryTemperature),
      noWorryTemperature(noWorryTemperature),
      dtmCriticalTemperature(dtmCriticalTemperature),
      dtmRecoveredTemperature(dtmRecoveredTemperature) {
        for (unsigned int core = 0; core < coreRows * coreColumns; core++) {
            oldTemperatures.push_back(0.0);
        }
      }
std::vector<int> FinalDVFS::getFrequencies(
    const std::vector<int> &oldFrequencies,
    const std::vector<bool> &activeCores) {
        std:vector<bool> isIncreasing (coreRows * coreColumns, false);

        for (unsigned int coreCounter = 0; coreCounter < coreRows * coreColumns; coreCounter++) {
            double temp = performanceCounters->getTemperatureOfCore(coreCounter);
            isIncreasing.at(coreCounter) = temp > oldTemperatures.at(coreCounter);
            oldTemperatures.at(coreCounter) = temp;
        }

        cout << "[Scheduler][ondemand-DTM]: checking for frequencies" << endl;
        if (throttle()) {
            std::vector<int> frequencies(coreRows * coreColumns, minFrequency);
            return frequencies;
        } else {
            std::vector<int> frequencies(coreRows * coreColumns);
            for (unsigned int coreCounter = 0; coreCounter < coreRows * coreColumns; coreCounter++) {
                if (activeCores.at(coreCounter)) {
                    const float core_temp = performanceCounters->getTemperatureOfCore(coreCounter);
                    if (core_temp > worryTemperature && isIncreasing.at(coreCounter)) {
                        frequencies.at(coreCounter) = frequencies.at(coreCounter) - 1;
                    } else {
                        int freq = oldFrequencies.at(coreCounter);
                        frequencies.at(coreCounter) = maxFrequency;
                    }
                } else {
                    frequencies.at(coreCounter) = minFrequency;
                }
            }
            return frequencies;
        }


    }

bool FinalDVFS::throttle() {
    cout << "[Scheduler][ondemand-DTM]: checking for thermal violation"
            << endl;
    if (performanceCounters->getPeakTemperature() > dtmCriticalTemperature) {
        if (!in_throttle_mode) {
            cout << "[Scheduler][ondemand-DTM]: detected thermal violation"
                    << endl;
        } else {
            cout << "[Scheduler][ondemand-DTM]: thermal violation ongoing"
                    << endl;
        }
        in_throttle_mode = true;
    } else if (performanceCounters->getPeakTemperature() <
                dtmRecoveredTemperature) {
        if (in_throttle_mode) {
            cout << "[Scheduler][ondemand-DTM]: thermal violation ended"
                    << endl;
        }
        in_throttle_mode = false;
    }

    cout << "[Scheduler][ondemand-DTM]: in throttle mode: " << in_throttle_mode
            << endl;
    cout << "[Scheduler][ondemand-DTM]: peak temperature: "
            << performanceCounters->getPeakTemperature() << endl;
    cout << "[Scheduler][ondemand-DTM]: dtmCriticalTemperature: "
            << dtmCriticalTemperature << endl;
    cout << "[Scheduler][ondemand-DTM]: dtmRecoveredTemperature: "
            << dtmRecoveredTemperature << endl;
    return in_throttle_mode;
}
