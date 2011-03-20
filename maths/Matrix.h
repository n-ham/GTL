#ifndef MATRIX_H
#define MATRIX_H

#include "../Const.h"
#include <math.h>

namespace GTL
{
    template <class V>
    struct Matrix
    {
        int rows, cols;                 //dimensions
        std::vector<std::vector<V> > matrix;      //matrix

        //constructors
        Matrix<V>()
        {
            rows = cols = 0;
        };

        //constructs an identity matrix
        Matrix<V>(int r, int c)
        {
            rows = r;
            cols = c;

            matrix = std::vector<std::vector<V> >(rows, std::vector<V> (cols, V(0) ));

            int iMax = std::min(rows, cols);
            for(int i=0; i<iMax; i++)
                matrix[i][i] = 1;
        };

        //constructs a matrice with the specified value
        /*
            n = matrix of n's (not including 0)
        */
        Matrix<V>(int r, int c, V value)
        {
            rows = r;
            cols = c;

            matrix = std::vector<std::vector<V> >(rows, std::vector<V>(cols, value));
        };

        //index function
        std::vector<V>& operator[](int r) const
        {
            return (std::vector<V>&)matrix[r];
        };
    };

    //input function
    template <class V>
    std::istream& operator>>(std::istream& is, const Matrix<V> &A)
    {
        for(int r=0; r<A.rows; r++)
            for(int c=0; c<A.cols; c++)
                is >> A[r][c];
        return is;
    };

    //output function
    template <class V>
    std::ostream & operator<<(std::ostream& os, const Matrix<V> &A)
    {
        for(int r=0; r<A.rows; r++)
        {
            for(int c=0; c<A.cols; c++)
                os << A[r][c] << " ";
            os << std::endl;
        }
        return os;
    };

    //matrix addition
    template <class V>
    Matrix<V> operator+(const Matrix<V> &A, const Matrix<V> &B)
    {
        Matrix<V> C(A.rows,B.cols);
        for(int r=0; r<A.rows; r++)
            for(int c=0; c<A.cols; c++)
                C[r][c] = A[r][c] + B[r][c];
        return C;
    };

    //matrix subtraction
    template <class V>
    Matrix<V> operator-(const Matrix<V> &A, const Matrix<V> &B)
    {
        Matrix<V> C(A.rows,B.cols);
        for(int r=0; r<A.rows; r++ )
            for (int c=0; c<B.cols; c++ )
                C[r][c] = A[r][c] - B[r][c];
        return C;
    };

    //matrix multiplication
    template <class V>
    Matrix<V> operator*(const Matrix<V> &A, const Matrix<V> &B)
    {
        Matrix<V> C(A.rows,B.cols);
        for(int r=0; r<A.rows; r++)
            for(int c=0; c<B.cols; c++)
                for(int i=0; i<A.cols; i++)
                    C[r][c] += A[r][i]*B[i][c];
        return C;
    };

    //scalar multiplication
    template <class V>
    Matrix<V> operator*(const V &a, const Matrix<V> &A)
    {
        Matrix<V> C(A.rows, A.cols);
        for(int r=0; r<A.rows; r++)
            for(int c=0; c<A.cols; c++)
                C[r][c] = a*A[r][c];

        return C;
    };

    /* MAKE THIS BETTER!! */
    //power of a matrix
    template <class V>
    Matrix<V> pow(const Matrix<V> &A, int n)
    {
        Matrix<V> C = A;
        for(int p=2; p<=n; p++)
            C = C*A;
        return C;
    };

    /*
        returns the expected value of the payoff matrix given the probability
        of each outcome occuring as given in the probabilities matrix.
    */
    template <class V>
    V sdot(const Matrix<V> &probabilities, const Matrix<V> &payoffs)
    {
        V ans = V(0);
        for(int r=0; r<probabilities.rows; r++)
            for(int c=0; c<probabilities.cols; c++)
                ans += probabilities[r][c] * payoffs[r][c];

        return ans;
    };

    /*
        returns the expected value of each payoff matrix given the probability
        of each outcome occuring as given in the probabilities matrix.
    */
    template <class V>
    std::vector<V> smdot(const Matrix<V> &probabilities, const std::vector<Matrix<V> > &payoffs)
    {
        std::vector<V> ans(payoffs.size(), V(0));
        for(int p=0; p<payoffs.size(); p++)
            for(int r=0; r<probabilities.rows; r++)
                for(int c=0; c<probabilities.cols; c++)
                    ans[p] += probabilities[r][c] * payoffs[p][r][c];

        return ans;
    }


    //returns the transpose of matrix A
    template <class V>
    Matrix<V> transpose(const Matrix<V> &A)
    {
        Matrix<V> C(A.cols, A.rows);

        for(int i=0; i<A.rows; i++)
            for(int j=0; j<A.cols; j++)
                C[j][i] = A[i][j];

        return C;

    };
}

#endif
