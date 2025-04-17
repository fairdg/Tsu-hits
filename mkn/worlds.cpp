#include <iostream>
#include <cmath>
#include <vector>
#include <queue>

using namespace std;

// Функция для расчета вероятности
double calculate_probability(int N, int k) {
    double favorable_outcomes = N * pow(5, N - 1);
    double total_outcomes = pow(6, N);
    return favorable_outcomes / total_outcomes;
}

// Функция для построения дерева решений
void build_decision_tree(int N, int k) {
    queue<vector<int>> q;
    q.push(vector<int>(N, 0));  // Начинаем с первого уровня (пустая кость)

    while (!q.empty()) {
        vector<int> current = q.front();
        q.pop();
        
        // Печать текущего уровня
        for (int i = 0; i < N; ++i) {
            cout << current[i] << " ";
        }
        cout << endl;

        // Генерация следующих уровней (возможные значения для каждой кости)
        for (int i = 0; i < N; ++i) {
            if (current[i] == 0) {  // Если на этой кости еще не выбрано значение
                for (int v = 1; v <= 6; ++v) {
                    if (v == k) continue;  // Пропускаем значение k
                    vector<int> next = current;
                    next[i] = v;
                    q.push(next);
                }
            }
        }
    }
}

int main() {
    int N, k;
    cin >> N >> k;

    // Расчет вероятности
    double probability = calculate_probability(N, k);
    cout.precision(10);
    cout << probability << endl;

    // Построение дерева решений
    build_decision_tree(N, k);

    return 0;
}
