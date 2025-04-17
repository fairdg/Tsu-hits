#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
using namespace std;

struct Node
{
    int index;
    double probability;
    bool iSeagle;
};

void dfs(vector<vector<Node>>& tree, int node, int& targetEagles, double& result, int currentEagles, double currentProbability) 
{
    if (tree[node].empty()) 
    {
        if (currentEagles == targetEagles)
        {
            result += currentProbability;
        }
        return;
    }

    for (Node& n : tree[node])
    {
        int newEagles = currentEagles + n.iSeagle;
        dfs(tree, n.index, targetEagles, result, newEagles, currentProbability * n.probability);
    }
}

int main()
{
    int n, k;
    cin >> n >> k;

    double probability1, probability2, result = 0.0;
    char side1, side2;
    Node input;
    int index = 1;

    vector<vector<Node>> tree(pow(2, n + 1));
    for (int i = 0; i < pow(2, n) - 1; i++)
    {
        cin >> probability1 >> side1 >> probability2 >> side2;

        input.index = index++;
        input.probability = probability1;
        input.iSeagle = (side1 == 'h');
        tree[i].push_back(input);

        input.index = index++;
        input.probability = probability2;
        input.iSeagle = (side2 == 'h');
        tree[i].push_back(input);
    }

    dfs(tree, 0, k, result, 0, 1.0);
    cout << fixed << setprecision(3) << result;
    return 0;
}