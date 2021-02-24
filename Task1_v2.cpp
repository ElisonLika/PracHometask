#include <iostream>
#include <cstring>
using namespace std;
 
class Massiv
{
    char **a;
    int *d, n, u, l;
public:
    Massiv() 
    {
        n = 0;
        a = new char*[n];
        d = new int[n];
    };
    void Create();
    void InputRandom();
    void Concat(const Massiv & n1, const Massiv & n2);
    void Merger(const Massiv & n1, const Massiv & n2);
    void Print() const;
    void PrintFow(int number) const;
    void UseSingleElement(int rec, int field);
    void Clear(int i);
    ~Massiv()
    {
        cout << l << " Meow " << u << endl;
        int i;
        for (i = 0; i < n; i++)
        { 
            delete [] a[i];
        } 
        delete [] a;
        delete [] d;
    }
};

void Massiv::Clear(int i) 
{
    u = i;
}

void Massiv::Create() //Создание массива строк
{
    int i;
    for (i = 0; i < n; i++)
    { 
        delete [] a[i];
    } 
    delete [] a;
    delete [] d;
    cout << "Number of rows = "; cin >> n;
    d=new int[n];
    a=new char*[n];
    for (i = 0; i < n; i++)
    {
        cout << "Number of elements in row number " << i + 1 << " = ";
        cin >> d[i];  
        a[i] = new char[d[i]];
    }
}                                                                                                                        

void Massiv::InputRandom()  // Ввод случайных буковок в массив строк
{
    Create();
    for (int i = 0; i < n; i++)
    { 
        for (int j = 0; j < d[i]; j++)
        {
            a[i][j] = rand() % 26 + 0x61;
        }
    }
}
 
void Massiv::Print() const //Вывод массива
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
 
void Massiv::PrintFow(int number) const //Вывод строк по индексу
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
 
void Massiv::Concat(const Massiv & n1, const Massiv & n2) //Сцепление двух массивов
{
    int m, i;
    for (i = 0; i < n; i++)
    { 
        delete [] a[i];
    } 
    delete [] a;
    delete [] d;
    l=0;
    n = n1.n + n2.n;
    l=1;
    d = new int[n];
    l=2;
    a = new char*[n];
    l=3;
    for(i = 0; i < n1.n; i++)
    {
        d[i] = n1.d[i];
        a[i] = new char[d[i]];
        strcpy(a[i],n1.a[i]);
    }
    l=4;
    for(i = n1.n; i < n; i++)
    {
        d[i] = n2.d[i-n1.n];
        a[i] = new char[d[i]];
        strcpy(a[i],n2.a[i-n1.n]);
    }
    l=5;
}
 
void Massiv::Merger(const Massiv & n1, const Massiv & n2)  //Слияние двух массивов
{
    int m, i, k, j, rep, *badnum;
    for (i = 0; i < n; i++)
    { 
        delete [] a[i];
    } 
    delete [] a;
    delete [] d;
    n = n1.n + n2.n;
    badnum = new int[n];
    rep = 0;
    for(i = n1.n-1; i > 0; i--)
    {
        badnum[i] = 0;
        for (k = i-1; k > -1; k--)
        {
            if(strcmp(n1.a[k],n1.a[i])==0)
            {
                rep++;
                badnum[i]=1;
                break;
            }
        }
    }
    for(i = n2.n-1; i > -1; i--)
    {
        badnum[n1.n+i] = 0;
        for (k = i-1; k > -1; k--)
        {
            if(strcmp(n2.a[k],n2.a[i])==0)
            {
                rep++;
                badnum[i+n1.n]=1;
                break;
            }
        }
        if(badnum[n1.n+i] != 1)
        {
            for (k = n1.n-1; k > -1; k--)
            {
                if(strcmp(n1.a[k],n2.a[i])==0)
                {
                    rep++;
                    badnum[i+n1.n]=1;
                    break;
                }
            }
        }
    }
    n = n - rep;
    m = 0;
    d = new int[n];
    a = new char*[n];
    for(i = 0; i < n1.n; i++)
    {
        if(badnum[i] != 1)
        {
            d[i - m] = n1.d[i];
            a[i - m] = new char[d[i - m]];
            strcpy(a[i-m],n1.a[i]);
        }
        else {m++;}
    }
    for(i = n1.n; i < n1.n+n2.n; i++)
    {
        if(badnum[i] != 1)
        {
            d[i - m] = n2.d[i-n1.n];
            a[i - m] = new char[d[i - m]];
            strcpy(a[i-m],n2.a[i-n1.n]);
        }
        else {m++;}
    }
    delete [] badnum;
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

    for(int i=0; i<10; i++)
    {
        a[i].Clear(i);
    }

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
            break;
        }
    }
    return(0);
}