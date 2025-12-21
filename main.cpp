#define END_INPUT_SYMBOL "YES"

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <locale>
#include <limits>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

void clearScreen();
void printMatrix(int numG, int** G);

int isInteger(const string& message);
bool isIntegerFile(const string& s, int& value);

void autoGraphGeneration(int& numG, int**& G, bool isOriented, bool isWeighted);
void fileGraphGeneration(int& numG, int**& G);

bool isIndependent(int** G, int* vertexSet, int setSize);
void findMIS(int** G, int numG, int* currentSet, int currentSize, int* remaining, int remainingSize, int* maxSet, int& maxSize);
void printMIS(int* maxSet, int maxSize);

int* colorGraph(int** G, int numG);
void processColoring(int numG, int** G, int*& outColors, int& outChromaticNumber, int*& outMisSet, int& outMisSize);

void clearMatrix(int& numG, int**& G);

void normalizeGraph(int numG, int** G);

void saveResultsToFile(int** G, int numG, const string& filename, int* colors, int chromaticNumber, int* misSet, int misSize);
void saveMatrixToFile(int** G, int numG, const string& filename);

int main() {
    setlocale(LC_ALL, "Rus");

    while(true){
        clearScreen();

        srand(time(NULL));

        int numG = 0;
        int** G = nullptr;

        int* colors = nullptr;
        int chromaticNumber = 0;
        int* misSet = nullptr;
        int misSize = 0;

        cout << "Алгоритм раскрашивания графа, основанный на поиске наибольшего независимого множества\n\n";
        cout << "Как создать граф?\n";
        cout << "1 — Автоматически\n";
        cout << "2 — Из файла\n";
        cout << "3 — Выход из программы\n";
        

        int mode;
        while (true) {
            mode = isInteger("Выберите режим: ");

            switch (mode) {
                case 1:
                {
                    numG = isInteger("Введите количество вершин графа: ");
                    while (numG <= 0) {
                        cout << "Ошибка! Количество вершин должно быть положительным\n";
                            numG = isInteger("Введите количество вершин графа: ");
                    }

                    cout << "\nТип графа:\n";
                    cout << "1 — Ориентированный\n";
                    cout << "2 — Неориентированный\n";
                    int orientChoice = isInteger("Выберите тип (1 или 2): ");
                    while (orientChoice != 1 && orientChoice != 2) {
                        cout << "Ошибка! Введите 1 или 2\n";
                        orientChoice = isInteger("Выберите тип (1 или 2): ");
                    }
                    bool isOriented = (orientChoice == 1);

                    cout << "\nВзвешенность графа:\n";
                    cout << "1 — Взвешенный\n";
                    cout << "2 — Невзвешенный\n";
                    int weightChoice = isInteger("Выберите тип (1 или 2): ");
                    while (weightChoice != 1 && weightChoice != 2) {
                        cout << "Ошибка! Введите 1 или 2\n";
                        weightChoice = isInteger("Выберите тип (1 или 2): ");
                    }
                    bool isWeighted = (weightChoice == 1);

                    autoGraphGeneration(numG, G, isOriented, isWeighted);
                    break;
                }

                case 2:
                    fileGraphGeneration(numG, G);
                    break;

                case 3: 
                    clearScreen(); 
                    cout << "Работа кода завершена\n";
                    exit(0);

                default:
                    cout << "Ошибка! Введите 1, 2 или 3\n";
                    continue;
            }
            break;
        }

        if (numG > 0 && G != nullptr) {
            cout << "\nИCХОДНЫЙ ГРАФ";
            printMatrix(numG, G);

            normalizeGraph(numG, G);

            cout << "\nНОРМАЛИЗОВАННЫЙ ГРАФ";
            printMatrix(numG, G);

            processColoring(numG, G, colors, chromaticNumber, misSet, misSize);

            cout << "\nРаскраска завершена. Если хотите сохранить данные в файл, введите \"" << END_INPUT_SYMBOL << "\" или любое другое значение для завершения работы программы: ";
            string input;
            cin >> input;

            if (input == END_INPUT_SYMBOL) {
                clearScreen();
                cout << "Какую часть данных вы хотите сохранить?\n";
                cout << "1 — Сохранить матрицу\n";
                cout << "2 — Сохранить матрицу и результаты раскрашивания\n";
                cout << "3 — Выход из программы\n";
                int mode2;
                while (true) {
                    mode2 = isInteger("Выберите из списка: ");

                    switch (mode2) {
                        case 1:
                        {
                            string filename;
                            cout << "Введите полный путь к файлу для сохранения матрицы: ";
                            cin >> filename;
                            saveMatrixToFile(G, numG, filename);
                            break;
                        }

                        case 2:
                        {
                            string filename;
                            cout << "Введите полный путь к файлу для сохранения результатов: ";
                            cin >> filename;
                            saveResultsToFile(G, numG, filename, colors, chromaticNumber, misSet, misSize);
                            break;
                        }

                        case 3: 
                            clearScreen();
                            cout << "Работа кода завершена\n";
                            exit(0);

                        default:
                            cout << "Ошибка! Введите 1, 2 или 3\n";
                            continue;
                    }
                    break;
                }
                
                if (colors){ 
                    delete[] colors;
                    colors = nullptr;
                }
                if (misSet){
                    delete[] misSet;
                    misSet = nullptr;
                }
                chromaticNumber = 0;
                misSize = 0;
                continue;
            }
        }

        clearMatrix(numG, G);

        if (colors) { delete[] colors; colors = nullptr; }
        if (misSet) { delete[] misSet; misSet = nullptr; }
        chromaticNumber = 0;
        misSize = 0;

        clearScreen();
        cout << "Работа кода завершена\n";
        break;
    }

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
            cout << "Ошибка: введено не целое число или не число\n";
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

void autoGraphGeneration(int& numG, int**& G, bool isOriented, bool isWeighted) {
    G = new int*[numG];
    for (int i = 0; i < numG; i++)
        G[i] = new int[numG];

    for (int i = 0; i < numG; i++) {
        for (int j = 0; j < numG; j++) {
            if (i == j) {
                G[i][j] = 0;
            } else {
                if (isWeighted) {
                    int edgeProb = rand() % 2;
                    if (edgeProb != 0) {
                        G[i][j] = rand() % 10 + 1;
                    } else {
                        G[i][j] = 0;
                    }
                } else {
                    G[i][j] = rand() % 2;
                }

                if (!isOriented) {
                    G[j][i] = G[i][j];
                }
            }
        }
    }

    if (!isOriented) {
        for (int i = 0; i < numG; i++) {
            for (int j = i + 1; j < numG; j++) {
                G[j][i] = G[i][j];
            }
        }
    }
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
            fin.close();
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
                    cout << "Ошибка: найден некорректный символ\n";
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
}

void printMatrix(int numG, int** G) {
    cout << "\nМатрица смежности:\n";
    for (int i = 0; i < numG; i++) {
        for (int j = 0; j < numG; j++)
            cout << setw(3) << G[i][j];
        cout << "\n";
    }
}

void clearMatrix(int& numG, int**& G) {
    if (G) {
        for (int i = 0; i < numG; i++)
            delete[] G[i];
        delete[] G;
    }
    G = nullptr;
    numG = 0;
}
// Является ли множество независимым
bool isIndependent(int** G, int* vertexSet, int setSize) {
    for (int i = 0; i < setSize; i++)
        for (int j = i + 1; j < setSize; j++)
            if (G[vertexSet[i]][vertexSet[j]] != 0) return false;
    return true;
}

// Рекурсивный поиск наибольшего независимого множества, обычный перебор
void findMIS(int** G, int numG, int* currentSet, int currentSize,
             int* remaining, int remainingSize, int* maxSet, int& maxSize) {

    if (currentSize > maxSize) {
        maxSize = currentSize;
        for (int i = 0; i < maxSize; i++) {
            maxSet[i] = currentSet[i];
        }
    }

    for (int i = 0; i < remainingSize; i++) {
        currentSet[currentSize] = remaining[i];

        // Формируем новое множество оставшихся вершин, не смежных с currentSet[currentSize]
        int* newRemaining = new int[remainingSize];
        int newSize = 0;
        for (int j = i + 1; j < remainingSize; j++) {
            if (G[remaining[i]][remaining[j]] == 0) {
                newRemaining[newSize++] = remaining[j];
            }
        }

        findMIS(G, numG, currentSet, currentSize + 1, newRemaining, newSize, maxSet, maxSize);
        delete[] newRemaining;
    }
}

void printMIS(int* maxSet, int maxSize) {
    cout << "Размер: " << maxSize << "\nВершины: ";
    for (int i = 0; i < maxSize; i++)
        cout << maxSet[i] << " ";
    cout << "\n";
}

int* colorGraph(int** G, int numG) {
    int* colors = new int[numG]{0};

    // Множество ещё не раскрашенных вершин
    int* remaining = new int[numG];
    for (int i = 0; i < numG; i++)
        remaining[i] = i;

    int remainingSize = numG;
    int* currentSet = new int[numG];
    int* maxSet = new int[numG];
    int color = 1;

    while (remainingSize > 0) {
        int maxSize = 0;

        findMIS(G, numG, currentSet, 0, remaining, remainingSize, maxSet, maxSize);

        // 1 цвет всем вершинам множества
        for (int i = 0; i < maxSize; i++)
            colors[maxSet[i]] = color;

        // Удаляем раскрашенные вершины из оставшихся
        int* newRemaining = new int[remainingSize];
        int newSize = 0;
        for (int i = 0; i < remainingSize; i++) {
            bool used = false;
            for (int j = 0; j < maxSize; j++) {
                if (remaining[i] == maxSet[j]) {
                    used = true;
                    break;
                }
            }
            if (!used)
                newRemaining[newSize++] = remaining[i];
        }

        delete[] remaining;
        remaining = newRemaining;
        remainingSize = newSize;
        color++;
    }

    delete[] currentSet;
    delete[] maxSet;
    return colors;
}

void processColoring(int numG, int** G, int*& outColors, int& outChromaticNumber, int*& outMisSet, int& outMisSize) {
    if (numG <= 0 || G == nullptr) {
        outColors = nullptr;
        outChromaticNumber = 0;
        outMisSet = nullptr;
        outMisSize = 0;
        return;
    }

    int* currentSet = new int[numG];
    int* remaining = new int[numG];
    int* maxSet = new int[numG];
    int currentSize = 0, remainingSize = numG, maxSize = 0;

    for (int i = 0; i < numG; i++)
        remaining[i] = i;

    findMIS(G, numG, currentSet, currentSize, remaining, remainingSize, maxSet, maxSize);

    cout << "\nНаибольшее независимое множество:\n";
    printMIS(maxSet, maxSize);

    outMisSize = maxSize;
    outMisSet = new int[maxSize];
    for (int i = 0; i < maxSize; i++) {
        outMisSet[i] = maxSet[i];
    }

    delete[] currentSet;
    delete[] remaining;
    delete[] maxSet;

    int* colors = colorGraph(G, numG);

    int chromaticNumber = 0;
    for (int i = 0; i < numG; i++) {
        if (colors[i] > chromaticNumber) {
            chromaticNumber = colors[i];
        }
    }

    cout << "\nРаскраска графа:\n";
    for (int i = 0; i < numG; i++){
        cout << "Вершина " << i << " -> Цвет " << colors[i] << "\n";
    }

    cout <<"\nХроматическое число: " << chromaticNumber << "\n";

    outColors = colors;
    outChromaticNumber = chromaticNumber;
}

void normalizeGraph(int numG, int** G) {
    if (numG <= 0 || G == nullptr) {
        return;
    }

    for (int i = 0; i < numG; i++) {
        for (int j = 0; j < numG; j++) {
            if (i == j) {
                G[i][j] = 0;
            } else {
                if (G[i][j] > 0 || G[j][i] > 0) {
                    G[i][j] = G[j][i] = 1;
                } else {
                    G[i][j] = G[j][i] = 0;
                }
            }
        }
    }
}

void saveResultsToFile(int** G, int numG, const string& filename, int* colors, int chromaticNumber, int* misSet, int misSize) {
    ofstream fout(filename);
    if (!fout.is_open()) {
        cout << "Не удалось открыть файл " << filename << " для записи.\n";
        return;
    }

    fout << numG << "\n";
    for (int i = 0; i < numG; i++) {
        for (int j = 0; j < numG; j++) {
            fout << G[i][j];
            if (j < numG - 1) fout << " ";
        }
        fout << "\n";
    }

    if (colors && misSet) {
        fout << "\nНаибольшее независимое множество:\n";
        fout << "Размер: " << misSize << "\nВершины: ";
        for (int i = 0; i < misSize; i++) {
            fout << misSet[i] << " ";
        }
        fout << "\n";

        fout << "\nРаскраска графа:\n";
        for (int i = 0; i < numG; i++){
            fout << "Вершина " << i << " -> Цвет " << colors[i] << "\n";
        }

        fout << "\nХроматическое число: " << chromaticNumber << "\n";
    } 
    else {
        fout << "\nРезультаты раскраски не найдены или не были вычислены.\n";
    }

    fout.close();
    cout << "Результаты успешно сохранены в файл " << filename << "\n";
}

void saveMatrixToFile(int** G, int numG, const string& filename) {
    ofstream fout(filename);
    if (!fout.is_open()) {
        cout << "Не удалось открыть файл " << filename << " для записи.\n";
        return;
    }

    fout << numG << "\n";
    for (int i = 0; i < numG; i++) {
        for (int j = 0; j < numG; j++) {
            fout << G[i][j];
            if (j < numG - 1) fout << " ";
        }
        fout << "\n";
    }
    fout.close();
    cout << "Матрица успешно сохранена в файл " << filename << "\n";
}