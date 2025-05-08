#include "coldestCore.h"
#include <iomanip>
using namespace std;
ColdestCore::ColdestCore(const PerformanceCounters *performanceCounters,
                         int coreRows, int coreColumns,
                         float criticalTemperature)
    : performanceCounters(performanceCounters),
      coreRows(coreRows),
      coreColumns(coreColumns),
      criticalTemperature(criticalTemperature) {}
std::vector<int> ColdestCore::map(String taskName, int taskCoreRequirement,
                                  const std::vector<bool> &availableCoresRO,
                                  const std::vector<bool> &activeCores)
{
    std::vector<bool> availableCores(availableCoresRO);
    std::vector<int> cores;

    // Initialize the previous temperatures queues
    cout << "[Scheduler] [INIT tempqueeus]" << endl;
    for (int c = 0; c < coreRows * coreColumns; c++) {
        std::queue<float> newQueue;
        newQueue.push(0.0);
        tempQueues.push_back(newQueue);
    }

    logTemperatures(availableCores);
    for (; taskCoreRequirement > 0; taskCoreRequirement--)
    {
        int coldestCore = getColdestCore(availableCores);
        if (coldestCore == -1)
        {
            // not enough free cores
            std::vector<int> empty;
            return empty;
        }
        else
        {
            cores.push_back(coldestCore);
            availableCores.at(coldestCore) = false;
        }
    }
    return cores;
}
std::vector<migration> ColdestCore::migrate(
    SubsecondTime time, const std::vector<int> &taskIds,
    const std::vector<bool> &activeCores)
{
    std::vector<migration> migrations;
    std::vector<bool> availableCores(coreRows * coreColumns);
    for (int c = 0; c < coreRows * coreColumns; c++)
    {
        availableCores.at(c) = taskIds.at(c) == -1;
    }
    cout << "RUNNING MIGRATE POLICY" << endl;
    cout << "RUNNING MIGRATE POLICY" << endl;
    cout << "RUNNING MIGRATE POLICY" << endl;
    for (int c = 0; c < coreRows * coreColumns; c++)
    {
        if (activeCores.at(c))
        {
            cout << "Get temp: " << performanceCounters << endl;
            float temperature = (float)performanceCounters->getTemperatureOfCore(c);
            cout << "Get back of queue: " << tempQueues[c].size() << endl;
            float diffTemp = temperature - tempQueues[c].back();
            tempQueues[c].push(temperature);
            if (tempQueues[c].size() > WINDOW_SIZE)
            {
                cout << "TRYING TO DEQUEUE !!!!!!!!!!!!!!!!!" << endl;
                tempQueues[c].pop();
            }
            float avgTemp = this->averageTemperature(c);
            if (avgTemp > criticalTemperature && diffTemp > 0 && temperature > criticalTemperature)
            {
                cout << "[Scheduler][coldestCore-migrate]: core" << c
                     << " too hot (";
                cout << fixed << setprecision(1) << avgTemp
                     << ") -> migrate";
                logTemperatures(availableCores);
                int targetCore = getColdestCore(availableCores);
                if (targetCore == -1)
                {
                    cout << "[Scheduler][coldestCore-migrate]: no target core "
                            "found, cannot migrate "
                         << endl;
                }
                else
                {
                    migration m;
                    m.fromCore = c;
                    m.toCore = targetCore;
                    m.swap = false;
                    migrations.push_back(m);
                    availableCores.at(targetCore) = false;
                }
            }
        }
    }
    return migrations;
}
int ColdestCore::getColdestCore(const std::vector<bool> &availableCores)
{
    int coldestCore = -1;
    float coldestTemperature = 0;
    // iterate all cores to find coldest
    for (int c = 0; c < coreRows * coreColumns; c++)
    {
        if (availableCores.at(c))
        {
            // float temperature = performanceCounters->getTemperatureOfCore(c);
            float temperature = this->averageTemperature(c);
            if ((coldestCore == -1) || (temperature < coldestTemperature))
            {
                coldestCore = c;
                coldestTemperature = temperature;
            }
        }
    }
    return coldestCore;
}
void ColdestCore::logTemperatures(const std::vector<bool> &availableCores)
{
    cout << "[Scheduler][coldestCore-map]: temperatures of available cores:"
         << endl;
    for (int y = 0; y < coreRows; y++)
    {
        for (int x = 0; x < coreColumns; x++)
        {
            if (x > 0)
            {
                cout << " ";
            }
            int coreId = y * coreColumns + x;
            if (!availableCores.at(coreId))
            {
                cout << " - ";
            }
            else
            {
                float temperature =
                    performanceCounters->getTemperatureOfCore(coreId);
                cout << fixed << setprecision(1) << temperature;
            }
        }
        cout << endl;
    }
}

float ColdestCore::averageTemperature(int core) {
    std::queue<float> tempQueue = tempQueues[core];
    std::queue<float> tempQueueCopy = tempQueue;
    cout << "1. temp queue sizes: " << tempQueue.size() << " , " << tempQueueCopy.size() << endl;
    if (tempQueue.size() == 0) {
        return 0.0;
    }
    // Calculate the average temperature
    float sum = 0.0;
    cout << "2. temp queue sizes: " << tempQueue.size() << " , " << tempQueueCopy.size() << endl;
    while (!tempQueueCopy.empty()) {
        sum += tempQueueCopy.front();
        tempQueueCopy.pop();
    }

    cout << "3. temp queue sizes: " << tempQueue.size() << " , " << tempQueueCopy.size() << endl;
    return sum / tempQueue.size();
}