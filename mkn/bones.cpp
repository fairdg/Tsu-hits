#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

int depthLimit, targetSum;
long long totalPaths = 0;
long long successfulPaths = 0;

struct State {
    int nodeId;
    double probability;
    int value;
};

vector<vector<State>> treeLevels;

void explorePaths(int currentNode, int depth, int currentSum, double currentProb) {
    if (depth == depthLimit) {
        if (currentSum >= targetSum) {
            successfulPaths += currentProb;  // Учитываем успешный путь
        }
        totalPaths++;  // Увеличиваем количество всех путей
        return;
    }

    for (const State& nextState : treeLevels[currentNode]) {
        explorePaths(nextState.nodeId, depth + 1, currentSum + nextState.value, currentProb * nextState.probability);
    }
}

int main() {
    cin >> depthLimit >> targetSum;

    int nextNodeId = 1;
    vector<int> currentLevel = {0};
    treeLevels.push_back({});

    for (int level = 0; level < depthLimit; ++level) {
        vector<int> nextLevel;

        for (int parent : currentLevel) {
            for (int face = 1; face <= 6; ++face) {
                State newState;
                newState.nodeId = nextNodeId++;
                newState.value = face;
                newState.probability = 1.0 / 6.0;

                int maxSize = max(parent, newState.nodeId);
                if (treeLevels.size() <= maxSize) treeLevels.resize(maxSize + 1);

                treeLevels[parent].push_back(newState);
                nextLevel.push_back(newState.nodeId);
            }
        }

        currentLevel = nextLevel;
    }

    explorePaths(0, 0, 0, 1.0);

    double probability = (double)successfulPaths / totalPaths;
    cout << fixed << setprecision(6) << probability << endl;

    for (const auto& level : treeLevels) {
        if (level.empty()) {
            cout << endl;
            continue;
        }
        for (const auto& node : level) {
            cout << node.value << " ";
        }
        cout << endl;
    }

    return 0;
}
