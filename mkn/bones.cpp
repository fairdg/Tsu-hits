#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

struct State
{
    int nodeId;
    double probability;
    int value;
};

void explorePaths(const vector<vector<State>>& tree, int currentNode, int targetSum, double& allProbability, int currentSum, double currentProbability, int depth, int &n)
{
    if (depth == n)
    {
        if (currentSum >= targetSum)
        {
            allProbability += currentProbability;
        }
        return;
    }
    for (const State& nextState : tree[currentNode])
    {
        explorePaths(tree, nextState.nodeId, targetSum, allProbability, currentSum + nextState.value, currentProbability * nextState.probability, depth + 1, n);
    }
}

int main()
{
    int n, s;
    cin >> n >> s;
    double result = 0.0;
    State node;
    int number = 1;
    vector<vector<State>> diceList;
    diceList.push_back(vector<State>());
    vector<int> parent = { 0 };
    for (int i = 0; i < n; i++)
    {
        vector<int> nextNode;
        for (auto nod : parent) {
            for (int j = 0; j < 6; j++)
            {
                node.nodeId = number++;
                node.probability = 1.0 / 6.0;
                node.value = j + 1;

                int maxIndex = max(nod, node.nodeId);
                if (diceList.size() <= maxIndex) diceList.resize(maxIndex + 1);
                diceList[nod].push_back(node);
                nextNode.push_back(node.nodeId);
            }
        }
        parent = nextNode;
    }

    explorePaths(diceList, 0, s, result, 0, 1.0, 0, n);

    cout << fixed << setprecision(6) << result << endl;
    for (auto node : diceList)
    {
        if (!node.empty()) {
            for (auto nextState : node)
            {
                cout << nextState.value << " ";
            }
            cout << endl;
        }
    }
    return 0;
}
