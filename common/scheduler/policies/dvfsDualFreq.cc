#include "dvfsDualFreq.h"
#include <iomanip>
#include <iostream>
#include <cstdio>

using namespace std;

DVFSDualFreq::DVFSDualFreq(const PerformanceCounters *performanceCounters, int coreRows, int coreColumns, int freqOne, int freqTwo)
	: performanceCounters(performanceCounters), coreRows(coreRows), coreColumns(coreColumns), freqOne(freqOne), freqTwo(freqTwo) {
	
}

std::vector<int> DVFSDualFreq::getFrequencies(const std::vector<int> &oldFrequencies, const std::vector<bool> &activeCores) {
	std::vector<int> frequencies(coreRows * coreColumns);

	for (unsigned int coreCounter = 0; coreCounter < coreRows * coreColumns; coreCounter++) {
		if (activeCores.at(coreCounter)) {
			float power = performanceCounters->getPowerOfCore(coreCounter);
			float temperature = performanceCounters->getTemperatureOfCore(coreCounter);
			int frequency = oldFrequencies.at(coreCounter);
			float utilization = performanceCounters->getUtilizationOfCore(coreCounter);

			cout << "[Scheduler][DVFS_DUAL_FREQ]: Core " << setw(2) << coreCounter << ":";
			cout << " P=" << fixed << setprecision(3) << power << " W";
			cout << " f=" << frequency << " MHz";
			cout << " T=" << fixed << setprecision(1) << temperature << " °C";
			cout << " utilization=" << fixed << setprecision(3) << utilization << endl;
		}
		
		// Set even cores to freqTwo, odd cores to freqOne
		if (coreCounter % 2 == 0) {
			frequencies.at(coreCounter) = freqTwo;
		} else {
			frequencies.at(coreCounter) = freqOne;
		}
	}
	
	return frequencies;
}