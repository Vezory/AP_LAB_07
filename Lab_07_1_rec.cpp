// Lab_07_1_rec.cpp
// Новосад Захарій
// Лабораторна робота № 7.1
// Пошук заданих елементів та впорядкування рядків / стовпчиків матриці (рекурсивний спосіб).
// Варіант 16

#include <iostream>
#include <iomanip>
#include <time.h>

using namespace std;

// Прототипи рекурсивних функцій (розбиття вкладених циклів на пари Rows/Row за методичкою Григоровича)
void CreateRow(int** a, const int rowNo, const int colCount, const int Low, const int High, int colNo);
void CreateRows(int** a, const int rowCount, const int colCount, const int Low, const int High, int rowNo);

void PrintRow(int** a, const int rowNo, const int colCount, int colNo);
void PrintRows(int** a, const int rowCount, const int colCount, int rowNo);

void Change(int** a, const int row1, const int row2, const int colCount, int j);
void SortInner(int** a, const int rowCount, const int colCount, const int i0, int i1);
void SortOuter(int** a, const int rowCount, const int colCount, int i0);

void CalcRow(int** a, const int rowNo, const int colCount, int colNo, int& S, int& k);
void CalcRows(int** a, const int rowCount, const int colCount, int rowNo, int& S, int& k);

#ifndef UNIT_TEST
int main()
{
    // Ініціалізація датчика випадкових чисел
    srand((unsigned)time(NULL));

    // Параметри варіанта 16:
    // Діапазон значень: [11, 67]
    const int Low = 11;
    const int High = 67;

    // Розмірність матриці Z: 7 рядків та 5 стовпчиків
    const int rowCount = 7;
    const int colCount = 5;

    // 1. Динамічне виділення пам'яті для матриці:
    int** a = new int*[rowCount];
    for (int i = 0; i < rowCount; i++)
    {
        a[i] = new int[colCount];
    }

    // 2. Рекурсивне формування матриці (починаємо з 0-го рядка)
    CreateRows(a, rowCount, colCount, Low, High, 0);

    // 3. Рекурсивне виведення початкової матриці на екран
    cout << "Initial matrix (recursive):" << endl;
    PrintRows(a, rowCount, colCount, 0);

    // 4. Рекурсивне впорядкування рядків матриці (метод «бульбашки» без циклів)
    SortOuter(a, rowCount, colCount, 0);

    // 5. Виведення відсортованої матриці
    cout << "Sorted matrix (recursive):" << endl;
    PrintRows(a, rowCount, colCount, 0);

    // 6. Рекурсивне обчислення суми, кількості та заміни елементів нулями
    int S = 0; // накопичувач суми
    int k = 0; // лічильник кількості
    CalcRows(a, rowCount, colCount, 0, S, k);

    // 7. Виведення обчислених параметрів
    cout << "S = " << S << endl;
    cout << "k = " << k << endl;

    // 8. Виведення модифікованої матриці
    cout << "Modified matrix (elements replaced with 0, recursive):" << endl;
    PrintRows(a, rowCount, colCount, 0);

    // 9. Звільнення виділеної динамічної пам'яті
    for (int i = 0; i < rowCount; i++)
        delete[] a[i];
    delete[] a;

    return 0;
}
#endif

// -----------------------------------------------------------------------------
// 1. Рекурсивне формування одного рядка матриці
// Заповнює елементи рядка rowNo від colNo до colCount - 1
// -----------------------------------------------------------------------------
void CreateRow(int** a, const int rowNo, const int colCount, const int Low, const int High, int colNo)
{
    // Генеруємо значення для поточного елемента [rowNo][colNo]
    a[rowNo][colNo] = Low + rand() % (High - Low + 1);

    // Рекурсивний крок: якщо ще не дійшли до останнього стовпчика рядка
    if (colNo < colCount - 1)
        CreateRow(a, rowNo, colCount, Low, High, colNo + 1);
}

// -----------------------------------------------------------------------------
// 1. Рекурсивне формування всіх рядків матриці
// Обходить рядки від rowNo до rowCount - 1
// -----------------------------------------------------------------------------
void CreateRows(int** a, const int rowCount, const int colCount, const int Low, const int High, int rowNo)
{
    // Для поточного рядка rowNo викликаємо рекурсивне заповнення його стовпчиків, починаючи з colNo = 0
    CreateRow(a, rowNo, colCount, Low, High, 0);

    // Рекурсивний крок: переходимо до наступного рядка, якщо не дійшли до кінця
    if (rowNo < rowCount - 1)
        CreateRows(a, rowCount, colCount, Low, High, rowNo + 1);
}

// -----------------------------------------------------------------------------
// 2. Рекурсивне виведення елементів одного рядка матриці
// -----------------------------------------------------------------------------
void PrintRow(int** a, const int rowNo, const int colCount, int colNo)
{
    // Друкуємо поточний елемент з вирівнюванням по ширині
    cout << setw(5) << a[rowNo][colNo];

    // Якщо це не останній стовпчик — рекурсивно друкуємо наступний
    if (colNo < colCount - 1)
        PrintRow(a, rowNo, colCount, colNo + 1);
    else
        cout << endl; // в кінці рядка переводимо курсор на новий рядок
}

// -----------------------------------------------------------------------------
// 2. Рекурсивне виведення всіх рядків матриці
// -----------------------------------------------------------------------------
void PrintRows(int** a, const int rowCount, const int colCount, int rowNo)
{
    // Якщо починаємо вивід всієї матриці з першого рядка (rowNo == 0) — додаємо відступ
    if (rowNo == 0)
        cout << endl;

    // Друкуємо поточний рядок матриці
    PrintRow(a, rowNo, colCount, 0);

    // Якщо ще є рядки — кличемо себе для наступного рядка rowNo + 1
    if (rowNo < rowCount - 1)
        PrintRows(a, rowCount, colCount, rowNo + 1);
    else
        cout << endl; // після останнього рядка матриці додаємо порожній рядок
}

// -----------------------------------------------------------------------------
// 3. Рекурсивний обмін елементів двох рядків матриці (аналог циклу по j)
// -----------------------------------------------------------------------------
void Change(int** a, const int row1, const int row2, const int colCount, int j = 0)
{
    // Обмін значень елементів у стовпчику j
    int tmp = a[row1][j];
    a[row1][j] = a[row2][j];
    a[row2][j] = tmp;

    // Рекурсивний перехід до наступного стовпчика j + 1
    if (j < colCount - 1)
        Change(a, row1, row2, colCount, j + 1);
}

// -----------------------------------------------------------------------------
// 4. Внутрішній прохід рекурсивного сортування бульбашкою (аналог циклу по i1)
// Порівнює пари рядків i1 та i1 + 1 за трьома ключами спадання (↓ ↓ ↓)
// -----------------------------------------------------------------------------
void SortInner(int** a, const int rowCount, const int colCount, const int i0, int i1)
{
    // Перевірка умови необхідності перестановки рядків:
    if ((a[i1][0] < a[i1 + 1][0]) // 1-й ключ: за спаданням 1-го стовпчика
        ||
        (a[i1][0] == a[i1 + 1][0] && // якщо рівні в 1-му стовпчику
         a[i1][1] < a[i1 + 1][1])    // 2-й ключ: за спаданням 2-го стовпчика
        ||
        (a[i1][0] == a[i1 + 1][0] && // якщо рівні в 1-му та 2-му стовпчиках
         a[i1][1] == a[i1 + 1][1] &&
         a[i1][2] < a[i1 + 1][2]))   // 3-й ключ: за спаданням 3-го стовпчика
    {
        // Рекурсивно міняємо рядки місцями
        Change(a, i1, i1 + 1, colCount, 0);
    }

    // Рекурсивний крок внутрішнього проходу:
    // В ітераційній бульбашці умова циклу: i1 < rowCount - i0 - 1
    // Отже, наступний виклик робимо, поки i1 < rowCount - i0 - 2
    if (i1 < rowCount - i0 - 2)
        SortInner(a, rowCount, colCount, i0, i1 + 1);
}

// -----------------------------------------------------------------------------
// 4. Зовнішній прохід рекурсивного сортування бульбашкою (аналог циклу по i0)
// -----------------------------------------------------------------------------
void SortOuter(int** a, const int rowCount, const int colCount, int i0)
{
    // Для поточного номера проходу i0 запускаємо внутрішнє рекурсивне порівняння з i1 = 0
    SortInner(a, rowCount, colCount, i0, 0);

    // Рекурсивний перехід до наступного проходу i0 + 1:
    // В ітераційному алгоритмі: i0 < rowCount - 1
    // Тому продовжуємо, поки i0 < rowCount - 2
    if (i0 < rowCount - 2)
        SortOuter(a, rowCount, colCount, i0 + 1);
}

// -----------------------------------------------------------------------------
// 5. Рекурсивне опрацювання одного рядка матриці для обчислення суми, кількості та заміни
// Критерій: всі, крім парних кратних 13: !(a[rowNo][colNo] % 2 == 0 && a[rowNo][colNo] % 13 == 0)
// -----------------------------------------------------------------------------
void CalcRow(int** a, const int rowNo, const int colCount, int colNo, int& S, int& k)
{
    // Якщо елемент відповідає критерію
    if (!(a[rowNo][colNo] % 2 == 0 && a[rowNo][colNo] % 13 == 0))
    {
        S += a[rowNo][colNo]; // збільшуємо суму
        k++;                   // збільшуємо лічильник
        a[rowNo][colNo] = 0;   // замінюємо елемент на нуль
    }

    // Рекурсивний перехід до наступного елемента рядка colNo + 1
    if (colNo < colCount - 1)
        CalcRow(a, rowNo, colCount, colNo + 1, S, k);
}

// -----------------------------------------------------------------------------
// 5. Рекурсивне опрацювання всіх рядків матриці
// -----------------------------------------------------------------------------
void CalcRows(int** a, const int rowCount, const int colCount, int rowNo, int& S, int& k)
{
    // Опрацьовуємо поточний рядок rowNo, починаючи з першого стовпчика colNo = 0
    CalcRow(a, rowNo, colCount, 0, S, k);

    // Рекурсивно переходимо до наступного рядка rowNo + 1
    if (rowNo < rowCount - 1)
        CalcRows(a, rowCount, colCount, rowNo + 1, S, k);
}

