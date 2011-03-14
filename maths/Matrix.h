#ifndef MATRIX_H
#define MATRIX_H

#include "../Const.h"

using namespace std;

namespace GTL
{
    template <class T>
    struct Matrix
    {
        int rows, cols;                 //dimensions
        vector<vector<T> > matrix;      //matrix

        //constructors
        Matrix<T>()
        {
            rows = cols = 0;
        };

        //constructs an identity matrix
        Matrix<T>(int r, int c)
        {
            rows = r;
            cols = c;

            matrix = vector<vector<T> >(rows, vector<T> (cols, (T) 0));

            for(int i=0; i<min(rows, cols); i++)
                matrix[i][i] = 1;
        };

        //constructs a matrice with the specified value
        /*
            n = matrix of n's (not including 0)
        */
        Matrix<T>(int r, int c, T value)
        {
            rows = r;
            cols = c;

            matrix = vector<vector<T> >(rows, vector<T>(cols, value));
        };

        //index function
        vector<T>& operator[](int i) const
        {
            return (vector<T>&)matrix[i];
        };
    };

    //input function
    template <class T>
    istream& operator>>(istream& is, const Matrix<T> &A)
    {
        for(int r=0; r<A.rows; r++)
            for(int c=0; c<A.cols; c++)
                is >> A[r][c];
        return is;
    };

    //output function
    template <class T>
    ostream & operator<<(ostream& os, const Matrix<T> &A)
    {
        for(int r=0; r<A.rows; r++)
        {
            for(int c=0; c<A.cols; c++)
                os << A[r][c] << " ";
            os << endl;
        }
        return os;
    };

    //matrix addition
    template <class T>
    Matrix<T> operator+(const Matrix<T> &A, const Matrix<T> &B)
    {
        Matrix<T> C(A.rows,B.cols);
        for(int i=0; i<A.rows; i++)
            for(int j = 0; j<A.cols; j++)
                C[i][j] = A[i][j] + B[i][j];
        return C;
    };

    //matrix subtraction
    template <class T>
    Matrix<T> operator-(const Matrix<T> &A, const Matrix<T> &B)
    {
        Matrix<T> C(A.rows,B.cols);
        for(int i=0; i<A.rows; i++ )
            for (int j=0; j<B.cols; j++ )
                C[i][j] = A[i][j] - B[i][j];
        return C;
    };

    //matrix multiplication
    template <class T>
    Matrix<T> operator*(const Matrix<T> &A, const Matrix<T> &B)
    {
        Matrix<T> C(A.rows,B.cols);
        for(int i=0; i<A.rows; i++)
            for(int j=0; j<B.cols; j++)
                for(int k=0; k<A.cols; k++)
                    C[i][j] += A[i][k]*B[k][j];
        return C;
    };

    //scalar multiplication
    template <class T>
    Matrix<T> operator*(const T &a, const Matrix<T> &A)
    {
        Matrix<T> C(A.rows, A.cols);
        for(int i=0; i<A.rows; i++)
            for(int j=0; j<A.cols; j++)
                C[i][j] = a*A[i][j];

        return C;
    };

    /* MAKE THIS BETTER!! */
    //power of a matrix
    template <class T>
    Matrix<T> pow(const Matrix<T> &A, int a)
    {
        Matrix<T> C;

        C = A;
        for(int i=2; i<=a; i++)
        {
            C = C*A;
        }

        return C;
    };

    template <class X, class T>
    X sdot(const Matrix<X> &A, const Matrix<T> &B)
    {
        X ans = (X) 0;
        for(int i=0; i<A.rows; i++)
            for(int j=0; j<A.cols; j++)
                ans += A[i][j] * ((X) B[i][j]);

        return ans;
    };

    template <class X, class T>
    vector<X> smdot(const Matrix<X> &A, const vector<Matrix<T> > &B)
    {
        vector<X> ans(B.size(),(PROB) 0);
        for(int p=0; p<B.size(); p++)
            for(int i=0; i<A.rows; i++)
                for(int j=0; j<A.cols; j++)
                    ans[p] += A[i][j] * ((X) B[p][i][j]);

        return ans;
    }


    //Computes the transpose of a matrix
    template <class Ty>
    Matrix<Ty> T(const Matrix<Ty> &A)
    {
        Matrix<Ty> C(A.cols, A.rows);

        for(int i=0; i<A.rows; i++)
            for(int j=0; j<A.cols; j++)
                C[j][i] = A[i][j];

        return C;

    };
}

#endif
