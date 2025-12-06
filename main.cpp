#include <iostream>
#include <ctime>
#include <cstdlib>
#include <locale>
#include <limits>
#include <iomanip>
#include <queue>
#include <fstream>
#include <sstream>

using namespace std;

void clearScreen();
void printMatrix(int numG, int** G);
int chooseStartVertex(int numG);

int isInteger(const string& message);
bool isIntegerFile(const string& s, int& value);

void autoGraphGeneration(int& numG, int**& G);
void fileGraphGeneration(int& numG, int**& G);
void keyboardGraphGeneration(int& numG, int**& G);

void clearMatrix(int& numG, int**& G);

int main() {
    setlocale(LC_ALL, "Rus");
    clearScreen();

    srand(time(NULL));

    int numG = 0;
    int** G = nullptr;

    cout << "Как создать граф?\n";
    cout << "1 — Автоматически\n";
    cout << "2 — Из файла\n";
    cout << "3 — Ручной ввод\n";

    int mode;
    while (true) {
        mode = isInteger("Выберите режим: ");

        switch (mode) {
            case 1: autoGraphGeneration(numG, G); break;
            case 2: fileGraphGeneration(numG, G); break;
            case 3: keyboardGraphGeneration(numG, G); break;
            default:
                cout << "Ошибка! Введите 1, 2 или 3\n";
                continue;
        }
        break;
    }

    clearMatrix(numG, G);
    return 0;
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int isInteger(const string& message) {
    int value;
    while (true) {
        cout << message;
        if (!(cin >> value)) {
            cout << "Ошибка: введено не число\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if (cin.peek() != '\n') {
            cout << "Ошибка: введено не целое число\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        return value;
    }
}

bool isIntegerFile(const string& s, int& value) {
    istringstream iss(s);
    if (!(iss >> value)) return false;
    return iss.peek() == EOF;
}

void autoGraphGeneration(int& numG, int**& G) {
    numG = isInteger("Введите количество вершин графа: ");
    while (numG <= 0) {
        cout << "Ошибка! Количество вершин должно быть положительным\n";
        numG = isInteger("Введите количество вершин графа: ");
    }

    clearMatrix(numG, G);

    G = new int*[numG];
    for (int i = 0; i < numG; i++)
        G[i] = new int[numG];

    for (int i = 0; i < numG; i++) {
        for (int j = i; j < numG; j++) {
            G[i][j] = G[j][i] = (i == j ? 0 : rand() % 2);
        }
    }

    printMatrix(numG, G);
    int start = chooseStartVertex(numG);
}

void fileGraphGeneration(int& numG, int**& G) {
    clearMatrix(numG, G);

    while (true) {
        string filename;
        cout << "Введите полный путь к файлу: ";
        cin >> filename;

        ifstream fin(filename);
        if (!fin.is_open()) {
            cout << "Не удалось открыть файл. Попробуйте снова\n";
            continue;
        }

        string token;
        int fileVertexCount;

        if (!(fin >> token) || !isIntegerFile(token, fileVertexCount) || fileVertexCount <= 0) {
            cout << "Ошибка: количество вершин должно быть положительным целым числом\n";
            continue;
        }

        numG = fileVertexCount;

        G = new int*[numG];
        for (int i = 0; i < numG; i++)
            G[i] = new int[numG];

        bool valid = true;

        for (int i = 0; i < numG; i++) {
            for (int j = 0; j < numG; j++) {

                if (!(fin >> token)) {
                    cout << "Ошибка: в файле недостаточно чисел\n";
                    valid = false;
                    break;
                }

                int value;
                if (!isIntegerFile(token, value) || value < 0) {
                    cout << "Ошибка: найдено нецелое число или отрицательное\n";
                    valid = false;
                    break;
                }

                G[i][j] = value;
            }
            if (!valid) break;
        }

        if (!valid) {
            clearMatrix(numG, G);
            cout << "Попробуйте снова\n";
            continue;
        }

        break;
    }

    printMatrix(numG, G);
    int start = chooseStartVertex(numG);
}

void keyboardGraphGeneration(int& numG, int**& G) {
    numG = isInteger("Введите количество вершин графа: ");
    while (numG <= 0) {
        cout << "Ошибка! Количество вершин должно быть положительным\n";
        numG = isInteger("Введите количество вершин графа: ");
    }

    clearMatrix(numG, G);

    G = new int*[numG];
    for (int i = 0; i < numG; i++)
        G[i] = new int[numG];

    cout << "Введите матрицу смежности:\n";

    for (int i = 0; i < numG; i++) {
        for (int j = 0; j < numG; j++) {
            int value;
            while (true) {
                value = isInteger("Элемент [" + to_string(i) + "][" + to_string(j) + "]: ");
                if (value >= 0) {
                    G[i][j] = value;
                    break;
                } else {
                    cout << "Ошибка! Элемент должен быть неотрицательным\n";
                }
            }
        }
    }

    printMatrix(numG, G);
    int start = chooseStartVertex(numG);
}

void printMatrix(int numG, int** G) {
    cout << "\nМатрица смежности:\n";
    for (int i = 0; i < numG; i++) {
        for (int j = 0; j < numG; j++)
            cout << setw(3) << G[i][j];
        cout << "\n";
    }
}

int chooseStartVertex(int numG) {
    int current = isInteger("\nВведите вершину, с которой хотите начать: ");

    while (current < 0 || current >= numG) {
        cout << "Ошибка! Вершина должна быть в диапазоне [0," << numG - 1 << "]\n";
        current = isInteger("Введите вершину: ");
    }

    return current;
}

void clearMatrix(int& numG, int**& G) {
    if (G) {
        for (int i = 0; i < numG; i++)
            delete[] G[i];
        delete[] G;
    }
    G = nullptr;
}