// UnitTest_07_1.cpp
// Тестування функцій лабораторної роботи 7.1 (Варіант 16)
#include "pch.h"
#include "CppUnitTest.h"

#define UNIT_TEST
#include "Lab_07_1_iter.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest071
{
    TEST_CLASS(UnitTest071)
    {
    public:
        // ---------------------------------------------------------------------
        // Тест 1: Перевірка функції Calc (обчислення суми, кількості та заміни 0)
        // Критерій: всі, крім парних кратних 13 (тобто крім 26, 52, ...)
        // ---------------------------------------------------------------------
        TEST_METHOD(TestMethod_Calc_Iter)
        {
            const int rowCount = 2;
            const int colCount = 3;

            int** a = new int*[rowCount];
            for (int i = 0; i < rowCount; i++)
                a[i] = new int[colCount];

            // Задаємо тестові значення:
            // 26 - парне кратне 13 -> ПРОПУСКАЄТЬСЯ
            // 15 - підходить (+15)
            // 20 - підходить (+20)
            a[0][0] = 26; a[0][1] = 15; a[0][2] = 20;

            // 52 - парне кратне 13 -> ПРОПУСКАЄТЬСЯ
            // 13 - непарне кратне 13 -> підходить (+13)
            // 11 - підходить (+11)
            a[1][0] = 52; a[1][1] = 13; a[1][2] = 11;

            int S = 0;
            int k = 0;
            Calc(a, rowCount, colCount, S, k);

            // Очікувані значення:
            // S = 15 + 20 + 13 + 11 = 59
            // k = 4
            Assert::AreEqual(59, S);
            Assert::AreEqual(4, k);

            // Перевіряємо, що 26 та 52 НЕ змінилися на 0
            Assert::AreEqual(26, a[0][0]);
            Assert::AreEqual(52, a[1][0]);

            // Перевіряємо, що решта елементів обнулилися
            Assert::AreEqual(0, a[0][1]);
            Assert::AreEqual(0, a[0][2]);
            Assert::AreEqual(0, a[1][1]);
            Assert::AreEqual(0, a[1][2]);

            for (int i = 0; i < rowCount; i++)
                delete[] a[i];
            delete[] a;
        }

        // ---------------------------------------------------------------------
        // Тест 2: Перевірка функції Sort (впорядкування рядків за ключами ↓ ↓ ↓)
        // ---------------------------------------------------------------------
        TEST_METHOD(TestMethod_Sort_Iter)
        {
            const int rowCount = 3;
            const int colCount = 3;

            int** a = new int*[rowCount];
            for (int i = 0; i < rowCount; i++)
                a[i] = new int[colCount];

            // Початковий порядок рядків:
            // Рядок 0: { 20, 10, 50 }
            // Рядок 1: { 40, 30, 20 }
            // Рядок 2: { 40, 50, 10 }
            a[0][0] = 20; a[0][1] = 10; a[0][2] = 50;
            a[1][0] = 40; a[1][1] = 30; a[1][2] = 20;
            a[2][0] = 40; a[2][1] = 50; a[2][2] = 10;

            Sort(a, rowCount, colCount);

            // Після сортування за спаданням (↓ ↓ ↓):
            // Рядок 0 має стати: { 40, 50, 10 }
            // Рядок 1 має стати: { 40, 30, 20 }
            // Рядок 2 має стати: { 20, 10, 50 }
            Assert::AreEqual(40, a[0][0]);
            Assert::AreEqual(50, a[0][1]);
            Assert::AreEqual(10, a[0][2]);

            Assert::AreEqual(40, a[1][0]);
            Assert::AreEqual(30, a[1][1]);
            Assert::AreEqual(20, a[1][2]);

            Assert::AreEqual(20, a[2][0]);
            Assert::AreEqual(10, a[2][1]);
            Assert::AreEqual(50, a[2][2]);

            for (int i = 0; i < rowCount; i++)
                delete[] a[i];
            delete[] a;
        }
    };
}

