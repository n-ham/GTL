#ifndef MATRIX_H
#define MATRIX_H

#include "../Const.h"
#include "../zgame/ZStrategy.h"
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

        //constructs a matrix with the specified value
        /*
            n = matrix of n's (not including 0)
        */
        Matrix<V>(int r, int c, V value)
        {
            rows = r;
            cols = c;

            matrix = std::vector<std::vector<V> >(rows, std::vector<V>(cols, value));
        };

        void clear()
        {
            rows = cols = 0;
            matrix.clear();
        };

        //index function
        std::vector<V>& operator[](int r) const
        {
            return (std::vector<V>&)matrix[r];
        };

        //index function
        V& operator[](const ZStrategy &strategy) const
        {
            return (V&) matrix[strategy[0] ][strategy[1] ];
        };

        //returns the minimum value for a row
        V minr(int row)
        {
            return min(matrix[row]);
        };

        //returns the minimum value for every row
        std::vector<V> minrows()
        {
            std::vector<V> ans;
            for(int r=0; r<rows; r++)
                ans.push_back(minr(r));
            return ans;
        };

        //returns the maximimum value for a row
        V maxr(int row)
        {
            return max(matrix[row]);
        };

        //returns the maximum value for every row
        std::vector<V> maxrows()
        {
            std::vector<V> ans;
            for(int r=0; r<rows; r++)
                ans.push_back(maxr(r));
            return ans;
        };

        //returns the minimum value for a column
        V minc(int col)
        {
            int ans = matrix[0][col];
            for(int r=0; r<rows; r++)
                if(matrix[r][col] < ans)
                    ans = matrix[r][col];
            return ans;
        };

        //returns the minimum value for every column
        std::vector<V> mincols()
        {
            std::vector<V> ans;
            for(int c=0; c<cols; c++)
                ans.push_back(minc(c));
            return ans;
        };

        //returns the maximum value for a column
        V maxc(int col)
        {
            int ans = matrix[0][col];
            for(int r=0; r<rows; r++)
                if(matrix[r][col] > ans)
                    ans = matrix[r][col];
            return ans;
        };

        //returns the maximum value for every column
        std::vector<V> maxcols()
        {
            std::vector<V> ans;
            for(int c=0; c<cols; c++)
                ans.push_back(maxc(c));
            return ans;
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
            os << join(' ', A[r]) << std::endl;
        return os;
    };

    //matrix addition
    template <class V>
    Matrix<V> operator+(const Matrix<V> &A, const Matrix<V> &B)
    {
        Matrix<V> C(A.rows,B.cols, 0);
        for(int r=0; r<A.rows; r++)
            for(int c=0; c<A.cols; c++)
                C[r][c] = A[r][c] + B[r][c];
        return C;
    };

    //matrix subtraction
    template <class V>
    Matrix<V> operator-(const Matrix<V> &A, const Matrix<V> &B)
    {
        Matrix<V> C(A.rows,B.cols, 0);
        for(int r=0; r<A.rows; r++ )
            for (int c=0; c<B.cols; c++ )
                C[r][c] = A[r][c] - B[r][c];
        return C;
    };

    //matrix multiplication
    template <class V>
    Matrix<V> operator*(const Matrix<V> &A, const Matrix<V> &B)
    {
        Matrix<V> C(A.rows,B.cols, 0);
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
        Matrix<V> C(A.rows, A.cols, 0);
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
