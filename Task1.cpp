#include <iostream>
#include <cstdio>
//#include <iomanip>
#include <math.h>
using namespace std;
 
class Massiv
{
    int **a;
public:
    int *d, n;
    void Create();
    void InputRandom();
    void Concat(Massiv n1, Massiv n2);
    void Merger(Massiv n1, Massiv n2);
    void Print();
    void PrintFow(int number);
    void UseSingleElement(int rec, int field);
    void Clear();
};
 
void Massiv::Create() //Создание массива строк
{
    cout << "Number of rows = "; cin >> n;
    d=new int[n];
    a=new int*[n];
    for (int i = 0; i < n; i++)
    {
        cout << "Number of elements in row number " << i + 1 << " = ";
        cin >> d[i];  
        a[i] = new int[d[i]];
    }    
}
void Massiv::Clear() //Очистка динамической памяти
{
    int i;
    for (int i = 0; i < n; i++)
    { 
       delete [] a[i];
    } 
    delete [] a;
    delete [] d;
}

void Massiv::InputRandom()  // Ввод случайных чисел в массив строк
{
    Create();
    for (int i = 0; i < n; i++) 
        for (int j = 0; j < d[i]; j++)
 
        {
            a[i][j] = rand() % 10;
        }
}
 
void Massiv::Print()  //Вывод всех строк
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < d[i]; j++)
        {
            cout << a[i][j];
        }
        cout << endl;
    }
}
 
void Massiv::PrintFow(int number) //Вывод строк по индексу
{
    number--;
    if((number < n) && (number >= 0))
        {
            for (int j = 0; j < d[number]; j++)
            {
                cout << a[number][j];
            }
        }
    else cout << "Unequal argument";
    cout << endl;
}
 
void Massiv::UseSingleElement(int i, int j) //You shall not pass! Или как контролировать выход за пределы?
{
    i--; j--;
    if ((i < n) && (i >= 0))
    {
        if((j < d[j]) && (j >= 0))
        {
            cout << a[i][j];
        }
        else cout << "Unequal arguments";
    }
    else cout << "Unequal arguments";
    cout << endl;
}
 
void Massiv::Concat(Massiv n1, Massiv n2) //Сцепление двух массивов
{
    int m, i ;
    n = n1.n + n2.n;
    d = new int[n];
    a = new int*[n];
    for(i = 0; i < n1.n; i++)
    {
        d[i] = n1.d[i];
        a[i] = new int[d[i]];
        for (int j = 0; j < d[i]; j++) 
        {
            a[i][j] = n1.a[i][j];
        }
    }
    for(i = n1.n; i < n; i++)
    {
        d[i] = n2.d[i-n1.n];
        a[i] = new int[d[i]];
        for (int j = 0; j < d[i]; j++)
        {
            a[i][j] = n2.a[i-n1.n][j];
        }
    }
}
 
void Massiv::Merger(Massiv n1, Massiv n2)  //Слияние двух массивов
{
    int m, i, k, j, *string, rep, *badnum;
    Massiv b;
    n = n1.n + n2.n;
    badnum = new int[n];
    rep = 0;
    for(i = n1.n-1; i > 0; i--)
    {
        string = new int[n1.d[i]];
        badnum[i] = 0;
        for (j = 0; j < n1.d[i]; j++)
        {
            string[j] = n1.a[i][j];
        }
        for (k = i-1; k > -1; k--)
        {
            if(n1.d[k] == n1.d[i])
            {
                m = 0;
                for(j = 0; j < n1.d[i]; j++)
                {
                    if(n1.a[k][j] == n1.a[i][j]) {m++;}
                    else break;
                }
                if (m == n1.d[i])
                {
                    rep ++;
                    badnum[i] = 1;
                    break;
                }
            }
        }
    }
    for(i = n2.n-1; i > -1; i--)
    {
        string = new int[n2.d[i]];
        badnum[n1.n+i] = 0;
        for (j = 0; j < n2.d[i]; j++)
        {
            string[j] = n2.a[i][j];
        }
        for (k = i-1; k > -1; k--) //В массиве n2
        {
            if(n2.d[k] == n2.d[i])
            {
                m = 0;
                for(j = 0; j < n2.d[i]; j++)
                {
                    if(n2.a[k][j] == n2.a[i][j]) {m++;}
                    else break;
                }
                if (m == n2.d[i])
                {
                    rep ++;
                    badnum[n1.n+i] = 1;
                    break;
                }
            }
        }
        if(badnum[n1.n+i] != 1) //В массиве n1
        {
            for (k = n1.n-1; k > -1; k--) //В массиве n2
            {
                if((badnum[k] != 1) && (n1.d[k] == n2.d[i]))
                {
                    m = 0;
                    for(j = 0; j < n2.d[i]; j++)
                    {
                        if(n1.a[k][j] == n2.a[i][j]) {m++;}
                        else break;
                    }
                    if (m == n2.d[i])
                    {
                        rep ++;
                        badnum[n1.n+i] = 1;
                        break;
                    }
                }
            }
        }
    }
    n = n - rep;
    m = 0;
    d = new int[n];
    a = new int*[n];
    for(i = 0; i < n1.n; i++)
    {
        if(badnum[i] != 1)
        {
            d[i - m] = n1.d[i];
            a[i - m] = new int[d[i - m]];
            for (int j = 0; j < d[i - m]; j++) 
            {
                a[i - m][j] = n1.a[i][j];
            }
        }
        else {m++;}
    }
    for(i = n1.n; i < n1.n+n2.n; i++)
    {
        if(badnum[i] != 1)
        {
            d[i - m] = n2.d[i-n1.n];
            a[i - m] = new int[d[i - m]];
            for (int j = 0; j < d[i - m]; j++)
            {
                a[i - m][j] = n2.a[i-n1.n][j];
            }
        }
        else {m++;}
    }

}


 
int main()
{
    Massiv a[10];
    int number, punkt, i, j, n1, n2, n;
            cout << "1 - Create array"; cout << endl;
            cout << "2 - Output array"; cout << endl;
            cout << "3 - Output row"; cout << endl;
            cout << "4 - Concatenation of arrays"; cout << endl;
            cout << "5 - Merging arrays"; cout << endl;
            cout << "6 - Output element"; cout << endl;
            cout << "7 - Quite"; cout << endl;
            cout << "Select a menu item "; cin >> punkt;
 
    while (punkt != 7)
    {
        switch (punkt)
        {
        case 1: {
            cout << "Input array number (from 0 to 9): "; cin >> n;
            cout << endl;
            a[n].InputRandom();
            cout << endl;
            a[n].Print();
            cout << endl;
            cout << "1 - Create array"; cout << endl;
            cout << "2 - Output array"; cout << endl;
            cout << "3 - Output row"; cout << endl;
            cout << "4 - Concatenation of arrays"; cout << endl;
            cout << "5 - Merging arrays"; cout << endl;
            cout << "6 - Output element"; cout << endl;
            cout << "7 - Quite"; cout << endl;
            cout << "Select a menu item "; cin >> punkt;
        }
                break;
 
        case 2: {
            cout << "Input array number (from 0 to 9): "; cin >> n;
            cout << endl;
            a[n].Print();
            cout << endl;
            cout << "1 - Create array"; cout << endl;
            cout << "2 - Output array"; cout << endl;
            cout << "3 - Output row"; cout << endl;
            cout << "4 - Concatenation of arrays"; cout << endl;
            cout << "5 - Merging arrays"; cout << endl;
            cout << "6 - Output element"; cout << endl;
            cout << "7 - Quite"; cout << endl;
            cout << "Select a menu item "; cin >> punkt;}
                break;
 
        case 3: {
            cout << "Input array number (from 0 to 9): "; cin >> n;
            cout << endl;
            cout << "Input row number: "; cin >> number;
            cout << endl;
            a[n].PrintFow(number);
            cout << "1 - Create array"; cout << endl;
            cout << "2 - Output array"; cout << endl;
            cout << "3 - Output row"; cout << endl;
            cout << "4 - Concatenation of arrays"; cout << endl;
            cout << "5 - Merging arrays"; cout << endl;
            cout << "6 - Output element"; cout << endl;
            cout << "7 - Quite"; cout << endl;
            cout << "Select a menu item "; cin >> punkt;
        }
                break;
 
        case 4: {
            cout << "Input first array number (from 0 to 9): "; cin >> n1; cout << endl;
            cout << "Input second array number (from 0 to 9): "; cin >> n2; cout << endl;
            cout << "Input result array number (from 0 to 9): "; cin >> n; cout << endl;
            if((n1!=n2) && (n1!=n) && (n2!=n)) { a[n].Concat(a[n1], a[n2]);}
            else {cout << "Wrong numbers";}
            cout << endl;
            cout << "1 - Create array"; cout << endl;
            cout << "2 - Output array"; cout << endl;
            cout << "3 - Output row"; cout << endl;
            cout << "4 - Concatenation of arrays"; cout << endl;
            cout << "5 - Merging arrays"; cout << endl;
            cout << "6 - Output element"; cout << endl;
            cout << "7 - Quite"; cout << endl;
            cout << "Select a menu item "; cin >> punkt; }
                break;
 
        case 5: {
            cout << "Input first array number (from 0 to 9): "; cin >> n1; cout << endl;
            cout << "Input second array number (from 0 to 9): "; cin >> n2; cout << endl;
            cout << "Input result array number (from 0 to 9): "; cin >> n; cout << endl;
            if((n1!=n2) && (n1!=n) && (n2!=n)) { a[n].Merger(a[n1], a[n2]);}
            else {cout << "Wrong numbers";}
            cout << endl;
            cout << "1 - Create array"; cout << endl;
            cout << "2 - Output array"; cout << endl;
            cout << "3 - Output row"; cout << endl;
            cout << "4 - Concatenation of arrays"; cout << endl;
            cout << "5 - Merging arrays"; cout << endl;
            cout << "6 - Output element"; cout << endl;
            cout << "7 - Quite"; cout << endl;
            cout << "Select a menu item "; cin >> punkt; }
                break;
 
        case 6: {
            cout << "Input array number: "; cin >> n;
            cout << endl;
            cout << "Input row number: "; cin >> i;
            cout << endl;
            cout << "Input element number: "; cin >> j;
            cout << endl;
            a[n].UseSingleElement(i, j);
            cout << "1 - Create array"; cout << endl;
            cout << "2 - Output array"; cout << endl;
            cout << "3 - Output row"; cout << endl;
            cout << "4 - Concatenation of arrays"; cout << endl;
            cout << "5 - Merging arrays"; cout << endl;
            cout << "6 - Output element"; cout << endl;
            cout << "7 - Quite"; cout << endl;
            cout << "Select a menu item "; cin >> punkt; }
                break;
        case 7:
            for(i = 0; i < 10; i++)
            {
                a[i].Clear();
            }
            break;
        }
    }
    return(0);
}