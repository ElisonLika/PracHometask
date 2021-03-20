#include <iostream>
using namespace std;

class Matrix
{
    int **matr, n, m;
public:
    static int max;
    Matrix();
    Matrix(const Matrix &a);
    Matrix(int k, int l, int *a);
    Matrix(int k, int l, int **a);
    int Getn(const Matrix &a) {return a.n;};
    int Getm(const Matrix &a) {return a.m;};
    Matrix& operator=(const Matrix &a);
    friend std::ostream& operator<< (std::ostream &out, const Matrix &mat);
    int* operator[](int idx) const;
    virtual Matrix mult(const Matrix &mat, int k);
    virtual Matrix mult(const Matrix &mat1, const Matrix &mat2);
    ~Matrix()
    {
        for (int i = 0; i < n; i++)
        { 
            delete [] matr[i];
        } 
        delete [] matr;
    }
};
    Matrix::Matrix() 
    {
        n=0;
        matr = new int*[n];
    };
    Matrix::Matrix(const Matrix &a)
    {
        n = a.n;
        m = a.m;
        matr = new int*[n];
        for(int i=0; i<n; i++)
        {
            matr[i] = new int[m];
            for(int j=0; j<m; j++)
                matr[i][j]=a.matr[i][j];
        }
    }
    Matrix::Matrix(int k, int l, int *a)
    {
        if((k<max)&&(l<max))
        {
            n = k;
            m = l;
            matr = new int*[n];
            for(int i=0; i<n; i++)
            {
                matr[i] = new int[m];
                for(int j=0; j<m; j++)
                    matr[i][j]=a[i*m + j];
            }
        }
        else 
        {
            cout << "Error" <<endl;
            n = 0;
            matr = new int*[n];
        }
    }
    Matrix::Matrix(int k, int l, int **a)
    {
        if((k<max)&&(l<max))
        {
            n = k;
            m = l;
            matr = new int*[n];
            for(int i=0; i<n; i++)
            {
                matr[i] = new int[m];
                for(int j=0; j<m; j++)
                    matr[i][j]=a[i][j];
            }
        }
        else 
        {
            cout << "Error" <<endl;
            n = 0;
            matr = new int*[n];
        }
    }
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
        }
        else return matr[idx];
    }
    Matrix Matrix::mult(const Matrix &mat, int k)
    {
        for (int i = 0; i < n; i++)
        { 
            delete [] matr[i];
        } 
        delete [] matr;
            //cout << mat <<endl; 
            n = mat.n;
            m = mat.m;
            matr = new int*[n];
            for(int i=0; i<n; i++)
            {
                matr[i] = new int[m];
                for(int j=0; j<m; j++)
                    matr[i][j]=mat.matr[i][j]*k;
            }
        return *this;
    }
    Matrix Matrix::mult(const Matrix &mat1, const Matrix &mat2)
    {
        if(mat1.m==mat2.n)
        {
            for (int i = 0; i < n; i++)
            { 
                delete [] matr[i];
            } 
            delete [] matr;
            n = mat1.n; m = mat2.m;
            matr = new int*[n];
            for(int i=0; i<n; i++)
            {
                matr[i]= new int[m];
                for(int j=0; j<m; j++)
                {
                    matr[i][j]=0;
                    for(int k=0; k< mat1.m; k++)
                        matr[i][j] += mat1.matr[i][k]*mat2.matr[k][j];
                }
            }
            return *this;

        }
        else cout << "Error"<< endl;
    }



class SquareMat : public Matrix
{
    int **matr, n, m;
    public:
        static int max;
        SquareMat();
        SquareMat(const SquareMat &a);
        SquareMat(const Matrix &a);
        SquareMat(int k, int *mat);
        int* operator[](int idx) const
        {
            if((idx<0) or (idx>n))
            {
                cout << "Erroneous index";
            }
            else return matr[idx];
        }
        SquareMat& operator=(const SquareMat &a)
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
        SquareMat mult(const SquareMat &mat1, const SquareMat &mat2)
        {
            Matrix a(mat1.n, mat1.n, mat1.matr),b(mat2.n, mat2.n, mat2.matr);
            return static_cast<SquareMat>(Matrix::mult(a,b));
        };
        SquareMat mult(const SquareMat &mat, int k)
        {
            Matrix a(mat.n, mat.n, mat.matr);
            return static_cast<SquareMat>(Matrix::mult(a,k));
        };
        friend std::ostream& operator<< (std::ostream &out, const SquareMat &mat);
        ~SquareMat()
        {
            for (int i = 0; i < n; i++)
            { 
                delete [] matr[i];
            } 
            delete [] matr;
        };
        void GetMatr(const SquareMat &temp, int i, int j);
        int Determ();
};
        SquareMat::SquareMat()
        {
            n=0;
            matr = new int*[n];
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
        SquareMat::SquareMat(const Matrix &a)
        {
            if(Getn(a)==Getm(a))
            {
                n = Getn(a);
                m = n;
                matr = new int*[n];
                for(int i=0; i<n; i++)
                {
                    matr[i] = new int[n];
                    for(int j=0; j<n; j++)
                        matr[i][j]=a[i][j];
                }
            }
            else
            {
                cout << "Error" <<endl;
                n = 0;
                matr = new int*[n];
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
                matr = new int*[n];
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
        int p[n-1][n-1]={0};
        SquareMat temp(n-1,reinterpret_cast<int *>(p));
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
            for (i = 0; i < n; i++) 
            {
                GetMatr(temp, i, 0);
                d = d + k * matr[i][0] * temp.Determ();
                k = -k;
            }
        return(d);
        }
    }


int Matrix::max=20;
int SquareMat::max=20;

 
int main() 
{
    int ma[3][3]={{1, 2, 3},{4, 5, 6},{7, 8, 1}},mk[2][2]={{3,3},{3,3}};
    //Matrix m1, m3;
    SquareMat n1, n2;//(SquareMat(2, reinterpret_cast<int *>(mk))), n2;
    n1 = SquareMat(3, reinterpret_cast<int *>(ma));
    cout << n1[1][1] << endl;
    cout << n1.Determ() << endl;
    //cout << n2.mult(n1,2) << endl;
    cout << n2.mult(n1,2) << endl;
    //m1 = Matrix(2, 2, reinterpret_cast<int *>(ma));
    //cout << "New" <<static_cast<Matrix>(n1)<< endl;
    //cout << "Old" <<static_cast< SquareMat>(m1)<< endl;
    /*Matrix m2(m1);
    m3 = Matrix(2, 1, reinterpret_cast<int *>(ma));
    cout << m1 << endl;
    cout << m2 << endl;
    cout << m3 << endl;
    cout << m2[1][1] << endl;
    cout << m3.mult(m2,2) << endl;
    cout << m3.mult(m2,m1) << endl;*/
    return(0);
}