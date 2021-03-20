#include <iostream>
using namespace std;

class Matrix
{
protected:
    int **matr, n, m, max = 20;
public:
    virtual int Determ() = 0;
    int Getn(const Matrix &a) {return a.n;};
    int Getm(const Matrix &a) {return a.m;};
    Matrix& operator=(const Matrix &a);
    friend std::ostream& operator<< (std::ostream &out, const Matrix &mat);
    int* operator[](int idx) const;
    void mult(int k);
    void mult(const Matrix &mat2);
    ~Matrix()
    {
        for (int i = 0; i < n; i++)
        { 
            delete [] matr[i];
        } 
        delete [] matr;
    }
};
    Matrix& Matrix::operator=(const Matrix &a)
    {
        for (int i = 0; i < n; i++)
        { 
            delete [] matr[i];
        } 
        delete [] matr;
        n = a.n;
        m = a.m;
        matr = new int*[n];
        for(int i=0; i<n; i++)
        {
            matr[i] = new int[m];
            for(int j=0; j<m; j++)
                matr[i][j]=a.matr[i][j];
        }
        return *this;
    }
    std::ostream& operator<< (std::ostream &out, const Matrix &mat)
    {
        for(int i=0; i<mat.n; i++)
        {
            for(int j=0; j<mat.m; j++)
            {
                out << mat.matr[i][j];
                out << ' ';
            }
            out << endl;
        }
        cout << "Matrix" << endl;
        return out;
    }
    int* Matrix::operator[](int idx) const
    {
        if((idx<0) or (idx>n))
        {
            cout << "Erroneous index";
            exit(-1);
        }
        else return matr[idx];
    }
    void Matrix::mult(int k)
    {
            for(int i=0; i<n; i++)
            {
                for(int j=0; j<m; j++)
                    matr[i][j]*=k;
            }
    }
    void Matrix::mult(const Matrix &mat2)
    {
        int b, **mat1;
        b = m;
        mat1 = new int*[n];
        for(int i=0; i<n; i++)
        {
            mat1[i] = new int[b];
            for(int j=0; j<b; j++)
                mat1[i][j]=matr[i][j];
        }
        if(b==mat2.n)
        {
             for (int i = 0; i < n; i++)
            { 
                delete [] matr[i];
            }
            m = mat2.m;
            for(int i=0; i<n; i++)
            {
                matr[i]= new int[m];
                for(int j=0; j<m; j++)
                {
                    matr[i][j]=0;
                    for(int k=0; k< b; k++)
                        matr[i][j] += mat1[i][k]*mat2.matr[k][j];
                }
            }
        }
        else cout << "Error"<< endl;
        for (int i = 0; i < n; i++)
        { 
            delete [] mat1[i];
        } 
        delete [] mat1;
    }


class SquareMat : public Matrix
{
    public:
        SquareMat();
        SquareMat(const SquareMat &a);
        SquareMat(int k, int *mat);
        SquareMat(int k, int **mat);
        friend std::ostream& operator<< (std::ostream &out, const SquareMat &mat);
        ~SquareMat() {};
        void GetMatr(const SquareMat &temp, int i, int j);
        int Determ();
};
        SquareMat::SquareMat()
        {
            n=0; m=0;
            matr = NULL;;
        }
        SquareMat::SquareMat(const SquareMat &a)
        {
            n = a.n;
            m = n;
            matr = new int*[n];
            for(int i=0; i<n; i++)
            {
                matr[i] = new int[n];
                for(int j=0; j<n; j++)
                    matr[i][j]=a.matr[i][j];
            }
        }
        SquareMat::SquareMat(int k, int *mat)
        {
            if(k<max)
            {
                n = k;
                m = n;
                matr = new int*[n];
                for(int i=0; i<n; i++)
                {
                    matr[i] = new int[n];
                    for(int j=0; j<n; j++)
                        matr[i][j]=mat[i*n + j];
                }
            }
            else 
            {
                cout << "Error" <<endl;
                n = 0;
                matr = NULL;
                exit(-1);
            }
        }
        SquareMat::SquareMat(int k, int **mat)
        {
            if(k<max)
            {
                n = k;
                m = n;
                matr = new int*[n];
                for(int i=0; i<n; i++)
                {
                    matr[i] = new int[n];
                    for(int j=0; j<n; j++)
                        matr[i][j]=mat[i][j];
                }
            }
            else 
            {
                cout << "Error" <<endl;
                n = 0;
                matr = NULL;
                exit(-1);
            }
        }
    std::ostream& operator<< (std::ostream &out, const SquareMat &mat)
    {
        for(int i=0; i<mat.n; i++)
        {
            for(int j=0; j<mat.m; j++)
            {
                out << mat.matr[i][j];
                out << ' ';
            }
            out << endl;
        }
         cout << "Square" << endl;
        return out;
    }
    void SquareMat::GetMatr(const SquareMat &temp, int  i, int j) 
    {
        int ki, kj, di, dj;
        di = 0;
        for (ki = 0; ki<n - 1; ki++) 
        {
            if (ki == i) di = 1;
            dj = 0;
            for (kj = 0; kj<n - 1; kj++) 
            {
                if (kj == j) dj = 1;
                temp[ki][kj] = matr[ki + di][kj + dj];
            }
        }
    }
    int SquareMat::Determ()
    {
        int i, d, k, l;
        d = 0;
        k = 1;
        if (n<1) cout << "Error determinant";
        if (n == 1) 
        {
            d = matr[0][0];
            return(d);
        }
        if (n == 2) 
        {
            d = matr[0][0] * matr[1][1] - (matr[1][0] * matr[0][1]);
            return(d);
        }
        if (n>2) 
        {
            int p[n-1][n-1]={0};
            SquareMat temp(n-1,reinterpret_cast<int *>(p));
            for (i = 0; i < n; i++) 
            {
                GetMatr(temp, i, 0);
                d = d + k * matr[i][0] * temp.Determ();
                k = -k;
            }
        return(d);
        }
    }
 
int main() 
{
    int ma[3][3]={{1, 2, 3},{4, 5, 6},{7, 8, 1}},mk[2][2]={{3,3},{3,3}};
    SquareMat n1,n2;
    n1 = SquareMat(3, reinterpret_cast<int *>(ma));
    n2 = n1;
    cout << n1 << endl;
    cout << n1[1][1] << endl;
    cout << n1.Determ() << endl;
    n2.mult(2);
    cout << n2 << endl;
    n2.mult(n1);
    cout << n2 << endl;
    return(0);
}