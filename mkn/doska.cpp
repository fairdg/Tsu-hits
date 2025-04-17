#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

struct State {
    int index;
    double probability;
    int value;
};

int depthLimit, targetSum;
double successfulPaths = 0.0;

vector<vector<State>> levelGraph;

void explorePaths(const vector<vector<State>>& graph, int currentNode, int depth, int currentSum, double currentProb, int countMatch) {
    if (depth == depthLimit) {
        if (countMatch == 1) {
            successfulPaths += currentProb;
        }
        return;
    }

    for (const State& next : graph[currentNode]) {
        int updatedCount = countMatch + (next.value == targetSum ? 1 : 0);
        explorePaths(graph, next.index, depth + 1, currentSum + next.value, currentProb * next.probability, updatedCount);
    }
}

int main() {
    cin >> depthLimit >> targetSum;

    int nodeIndex = 1;
    vector<int> currentLayer = {0};
    levelGraph.push_back({});

    for (int level = 0; level < depthLimit; ++level) {
        vector<int> nextLayer;

        for (int parent : currentLayer) {
            for (int face = 1; face <= 6; ++face) {
                State newState;
                newState.index = nodeIndex++;
                newState.probability = 1.0 / 6.0;
                newState.value = face;

                int maxSize = max(parent, newState.index);
                if (levelGraph.size() <= maxSize) levelGraph.resize(maxSize + 1);

                levelGraph[parent].push_back(newState);
                nextLayer.push_back(newState.index);
            }
        }
        currentLayer = nextLayer;
    }

    explorePaths(levelGraph, 0, 0, 0, 1.0, 0);

    cout << fixed << setprecision(6) << successfulPaths << endl;

    for (const auto& level : levelGraph) {
        if (level.empty()) {
            cout << endl;
            continue;
        }
        for (const auto& state : level) {
            cout << state.value << " ";
        }
        cout << endl;
    }

    return 0;
}
