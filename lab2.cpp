#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdlib>
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

const int INF = 1000000000; // "нескінченність" - ребра між вершинами немає

// Читає ціле число з клавіатури, поки користувач не введе коректне
int readInt(string text, int minV, int maxV) {
    while (true) {
        cout << text;
        string line;
        if (!getline(cin, line)) exit(0); // ввід закінчився
        stringstream ss(line);
        int x;
        string rest;
        if (ss >> x && !(ss >> rest) && x >= minV && x <= maxV)
            return x;
        cout << "Помилка: потрібне ціле число від " << minV << " до " << maxV << ".\n";
    }
}

class Graph {
    int n;                     // кількість вершин
    vector<vector<int>> d;     // матриця мінімальних відстаней
    vector<vector<int>> next;  // матриця шляхів: наступна вершина на шляху i -> j

    // Перевіряє один рядок матриці; якщо все добре - записує його в d[i]
    bool readRow(string line, int i) {
        stringstream ss(line);
        vector<int> row;
        string word;
        while (ss >> word) {
            if (word == "inf" || word == "INF") {
                row.push_back(INF);
            }
            else {
                stringstream t(word);
                int x;
                string rest;
                if (!(t >> x) || (t >> rest) || x < -1000 || x > 1000) {
                    cout << "Помилка: \"" << word << "\" - не число від -1000 до 1000 і не inf.\n";
                    return false;
                }
                row.push_back(x);
            }
        }
        if ((int)row.size() != n) {
            cout << "Помилка: кількість значень у рядку має бути " << n << ", а введено " << row.size() << ".\n";
            return false;
        }
        if (row[i] != 0) {
            cout << "Помилка: елемент на діагоналі (" << i + 1 << "," << i + 1 << ") має бути 0.\n";
            return false;
        }
        d[i] = row;
        return true;
    }

    // Виводить число або inf
    void printNum(int x) {
        if (x == INF) cout << setw(5) << "inf";
        else cout << setw(5) << x;
    }

public:
    int size() { return n; }

    void readMatrix() {
        n = readInt("Кількість вершин (від 2 до 10): ", 2, 10);
        d.assign(n, vector<int>(n, INF));
        cout << "Введіть матрицю відстаней розміром " << n << "x" << n
            << " по рядках (числа через пробіл, відсутність ребра - inf).\n";
        for (int i = 0; i < n; i++) {
            while (true) {
                cout << "Рядок " << i + 1 << ": ";
                string line;
                if (!getline(cin, line)) exit(0);
                if (readRow(line, i)) break; // рядок правильний - йдемо далі
            }
        }
    }

    // Алгоритм Флойда-Уоршелла. Повертає false, якщо є цикл від'ємної довжини
    bool floyd() {
        next.assign(n, vector<int>(n, -1));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (d[i][j] != INF) next[i][j] = j; // є пряме ребро (або i == j)

        for (int k = 0; k < n; k++)
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    if (d[i][k] != INF && d[k][j] != INF && d[i][k] + d[k][j] < d[i][j]) {
                        d[i][j] = d[i][k] + d[k][j]; // через k коротше
                        next[i][j] = next[i][k];
                    }

        for (int i = 0; i < n; i++)
            if (d[i][i] < 0) return false; // вершина i лежить на від'ємному циклі
        return true;
    }

    // Режим 1: всі пари вершин - дві матриці
    void printAll() {
        cout << "\nМатриця мінімальних відстаней:\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) printNum(d[i][j]);
            cout << "\n";
        }
        cout << "\nМатриця шляхів (наступна вершина на шляху з i в j, - немає шляху):\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (next[i][j] == -1) cout << setw(5) << "-";
                else cout << setw(5) << next[i][j] + 1;
            }
            cout << "\n";
        }
    }

    // Режим 2: одна пара вершин (a, b - номери з нуля)
    void printPair(int a, int b) {
        if (d[a][b] == INF) {
            cout << "Шляху з " << a + 1 << " в " << b + 1 << " не існує.\n";
            return;
        }
        cout << "Мінімальна відстань: " << d[a][b] << "\nШлях: " << a + 1;
        int v = a;
        while (v != b) {
            v = next[v][b];
            cout << " -> " << v + 1;
        }
        cout << "\n";
    }
};

int main() {
#ifdef _WIN32
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
#endif
    Graph g;
    int mode = 3; // спочатку треба ввести матрицю
    while (mode != 0) {
        if (mode == 3) {
            g.readMatrix();
            if (!g.floyd()) {
                cout << "Помилка: у графі є цикл від'ємної довжини, найкоротших шляхів не існує.\n"
                    << "Введіть іншу матрицю.\n";
                continue; // mode залишається 3 - матрицю просимо ще раз
            }
        }
        else if (mode == 1) {
            g.printAll();
        }
        else if (mode == 2) {
            int a = readInt("Початкова вершина: ", 1, g.size());
            int b = readInt("Кінцева вершина: ", 1, g.size());
            g.printPair(a - 1, b - 1);
        }
        cout << "\n1 - усі пари вершин\n2 - одна пара вершин\n3 - нова матриця\n0 - вихід\n";
        mode = readInt("Ваш вибір: ", 0, 3);
    }
    return 0;
}
