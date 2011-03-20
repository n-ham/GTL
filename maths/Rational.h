#ifndef RATIONAL_H_
#define RATIONAL_H_

#include "../Const.h"

namespace GTL
{
    /*
        A struct for implementing rational numbers
    */
    template <class Int>
    struct Rational
    {
        Int n, d;

        /*
            constructers
        */
        Rational<Int>()
        {
            n = d = (Int)1;
        };

        //expects a parameter that can be equated to long long int
        Rational<Int>(const Int &z)
        {
            n = z;
            d = (Int)1;
        };

        Rational<Int>(const Int &numerator, const Int &denominator)
        {
            n = numerator;
            d = denominator;

            factor();
        };

        Rational<Int>(const std::string &r)
        {
            *this = r;
        };

        //factors the numerator and denominator and cleans up the +/-ve's
        void factor()
        {
            Int divisor = gcd(n, d);
            if(divisor > 1)
            {
                n /= divisor;
                d /= divisor;
            }

            //makes the format nicer
            if(d < 0)
            {
                n = -n;
                d = -d;
            }
        };

        //assignment operator
        /*
            this will only read up to max(int) for n and d
        */
        Rational<Int> operator=(const std::string &r)
        {
            std::string part;
            Int i=0,
                m = 1;

            if(r[i] == '-')
            {
                m = -1;
                i++;
            }
            for(;i<(Int)r.size(); i++)
            {
                if(r[i] == '/')
                    break;
                else
                    part += r[i];
            }

            n = m*atoi(part.c_str());

            if(++i < (Int)r.size())
            {
                m = 1;
                if(r[i] == '-')
                {
                    m = -1;
                    i++;
                }

                part = "";
                for(;i<(Int)r.size(); i++)
                    part += r[i];
                d = m*atoi(part.c_str());
            }

            factor();

            return *this;
        };

        //prefix increment operator
        Rational<Int>& operator++()
        {
            n += d;
            factor();

            return *this;
        };

        //postfix increment operator
        Rational<Int>& operator++(int)
        {
            ++ *this;

            return *this;
        };

        //prefix decrement operator
        Rational<Int>& operator--()
        {
            n -= d;
            factor();

            return *this;
        };

        //postfix decrement operator
        Rational<Int>& operator--(int)
        {
            -- *this;

            return *this;
        };

        //minus operator
        Rational<Int>& operator-()
        {
            return Rational(-this->n, this->d);
        };
    };

    //equality relation
    template <class Int>
    bool operator==(const Rational<Int> &r1, const Rational<Int> &r2)
    {
        if(r1.n != r2.n || r1.d != r2.d)
            return 0;
        return 1;
    };

    //inequality relation
    template <class Int>
    bool operator!=(const Rational<Int> &r1, const Rational<Int> &r2)
    {
        if(r1.n != r2.n || r1.d != r2.d)
            return 1;
        return 0;
    };

    //less than relation
    template <class Int>
    bool operator<(const Rational<Int> &r1, const Rational<Int> &r2)
    {
        if(r1.d== r2.d)
        {
            if(r1.n < r2.n)
                return 1;
            else
                return 0;
        }
        else
        {
            Int clcm = lcm(r1.d, r2.d);

            if(r1.n*(clcm/r1.d) < r2.n*(clcm/r2.d))
                return 1;
            else
                return 0;
        }
    };

    //less than or equal to relation
    template <class Int>
    bool operator<=(const Rational<Int> &r1, const Rational<Int> &r2)
    {
        if(r1 == r2 || r1 < r2)
            return 1;
        else
            return 0;
    };

    //greater than relation
    template <class Int>
    bool operator>(const Rational<Int> &r1, const Rational<Int> &r2)
    {
        if(r1.d== r2.d)
        {
            if(r1.n > r2.n)
                return 1;
            else
                return 0;
        }
        else
        {
            Int clcm = lcm(r1.d, r2.d);

            if(r1.n*(clcm/r1.d) > r2.n*(clcm/r2.d))
                return 1;
            else
                return 0;
        }
    };

    //greater than or equal to relation
    template <class Int>
    bool operator>=(const Rational<Int> &r1, const Rational<Int> &r2)
    {
        if(r1 == r2 || r1 > r2)
            return 1;
        else
            return 0;
    };

    /*
        arithmetic operations
    */
    //addition operation
    template <class Int>
    Rational<Int> operator+(const Rational<Int> &r1, const Rational<Int> &r2)
    {
        if(r1.d == r2.d)
            return Rational<Int>(r1.n+r2.n, r1.d);

        Int clcm = lcm(r1.d, r2.d);
        return Rational<Int>(r1.n*(clcm/r1.d) + r2.n*(clcm/r2.d)  , clcm);
    };

    //subtraction operation
    template <class Int>
    Rational<Int> operator-(const Rational<Int> &r1, const Rational<Int> &r2)
    {
        if(r1.d == r2.d)
            return Rational<Int>(r1.n-r2.n, r1.d);

        Int clcm = lcm(r1.d, r2.d);
        return Rational<Int>(r1.n*(clcm/r1.d) - r2.n*(clcm/r2.d)  , clcm);
    };

    //multiplication operation
    template <class Int>
    Rational<Int> operator*(const Rational<Int> &r1, const Rational<Int> &r2)
    {
        return Rational<Int>(r1.n*r2.n, r1.d*r2.d);
    };

    //division operation
    template <class Int>
    Rational<Int> operator/(const Rational<Int> &r1, const Rational<Int> &r2)
    {
        return Rational<Int>(r1.n*r2.d, r1.d*r2.n);
    };

    //addition assignment operation
    template <class Int>
    Rational<Int>& operator+=(Rational<Int> &r1, const Rational<Int> &r2)
    {
        if(r1.d == r2.d)
            r1.n += r2.n;
        else
        {
            Int clcm = lcm(r1.d, r2.d);
            r1.n = r1.n*(clcm/r1.d) + r2.n*(clcm/r2.d);
            r1.d = clcm;
        }

        r1.factor();
        return r1;
    };

    //subtraction assignment operation
    template <class Int>
    Rational<Int>& operator-=(Rational<Int> &r1, const Rational<Int> &r2)
    {
        if(r1.d == r2.d)
            r1.n -= r2.n;
        else
        {
            Int clcm = lcm(r1.d, r2.d);
            r1.n = r1.n*(clcm/r1.d) - r2.n*(clcm/r2.d);
            r1.d = clcm;
        }

        r1.factor();
        return r1;

        return r1;
    };

    //multiplication assignment operation
    template <class Int>
    Rational<Int>& operator*=(Rational<Int> &r1, const Rational<Int> &r2)
    {
        r1.n *= r2.n;
        r1.d *= r2.d;
        r1.factor();
        return r1;
    };

    //division assignment operation
    template <class Int>
    Rational<Int>& operator/=(Rational<Int> &r1, const Rational<Int> &r2)
    {
        r1.n *= r2.d;
        r1.d *= r2.n;
        r1.factor();
        return r1;
    };

    //io functions
    template <class Int>
    std::istream& operator>>(std::istream &is, Rational<Int> &r)
    {
        std::string all, part;
        r.n = r.d = 1;

        is >> all;

        r = all;

        return is;
    };

    //output function
    template <class Int>
    std::ostream& operator<<(std::ostream &os, const Rational<Int> &r)
    {
        if(r.d == 1)
            os << r.n;
        else if(r.d == 0)
            os << "NaN";
        else
            os << toString(r.n) << "/" << toString(r.d);
        return os;
    };
}

#endif //RATIONAL_H_

