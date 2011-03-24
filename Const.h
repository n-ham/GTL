#ifndef CONST_H_
#define CONST_H_

#include <cstdlib>
#include <iostream>
#include <stdarg.h>
#include <math.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <stack>

#include <limits.h>


namespace GTL
{
    //converts a variable to a string
    template <class C>
    std::string toString(const C &c)
    {
        std::ostringstream oss;
        oss << c;
        return oss.str();
    };

    template <class C>
    std::string toString(C &c)
    {
        std::ostringstream oss;
        oss << c;
        return oss.str();
    };

    //returns te length of an integer variable
    template <class Int>
    Int len(Int n)
    {
        if(n == 0)
            return 1;
        else if(n < 0)
            n = -n;

        Int length = 1;
        while((n/=10) > 0)
            length++;
        return length;
    };

    //returns the greatest common divisor of a and b
    /*
    while b:
        a, b = b, a%b
    return a
    */
    template <class Int>
    Int gcd(Int x, Int y)
    {
        x = abs(x);
        y = abs(y);
        Int a = min(x, y),
            b = max(x, y),
            c;
        while(b)
        {
            c = a;
            a = b;
            b = c%b;
        }
        return a;
    };

    //returns the lowest common multiple of a and b
    template <class I>
    I lcm(const I &a, const I &b)
    {
        if(a == (I) 0 || b == (I) 0)
            return (I)0;
        else
            return abs(a*b)/gcd(a,b);
    };

    /*
        template vector functions
    */
    //joins a s
    template <class V>
    std::string join(char c, const std::vector<V> &v)
    {
        std::string s = "";
        if(v.size())
            s += toString(v[0]);

        for(int i=1; i<(int)v.size(); i++)
            s += c + toString(v[i]);

        return s;
    };

    template <class V>
    std::string join(const std::string &c, const std::vector<V> &v)
    {
        std::string s = "";
        if(v.size())
            s += toString(v[0]);

        for(int i=1; i<(int)v.size(); i++)
            s += c + toString(v[i]);

        return s;
    };

    //returns a vector with specified elements (must be a 1d vector)
    template <class V>
    std::vector<V> Vector(int noElements, V e1, ...)
    {
        std::vector<V> v(1, e1);

        va_list list;
        va_start(list, e1);

        for(int e=1; e<noElements; e++)
            v.push_back(va_arg(list, V));

        va_end(list);

        return v;
    };

    //sum of elements in the vector
    template <class V>
    V sum(const std::vector<V> &v)
    {
        V ans = 0;
        for(int i=0; i<(int)v.size(); i++)
            ans += v[i];
        return ans;
    };

    //product of elements in the vector
    template <class V>
    V product(const std::vector<V> &v)
    {
        V ans = 1;
        for(int i=0; i<(int)v.size(); i++)
            ans *= v[i];
        return ans;
    };

    //returns the minimum value in a vector
    template <class V>
    V min(const std::vector<V> &v)
    {
        if(v.size() == 0)
            return (V)0;

        V ans = v[0];
        for(int i=1; i<(int)v.size(); i++)
            if(v[i] < ans)
                ans = v[i];
        return ans;
    };

    //returns the maximum value in a vector
    template <class V>
    V max(const std::vector<V> &v)
    {
        if(v.size() == 0)
            return (V)0;

        V ans = v[0];
        for(int i=1; i<(int)v.size(); i++)
            if(v[i] > ans)
                ans = v[i];
        return ans;
    };

    //returns the indexes of the values that are minimal
    template <class V>
    std::vector<int> amin(const std::vector<V> &v)
    {
        if(v.size() == 0)
            return std::vector<int>();

        std::vector<int> ans(1, 0);
        V smallest = v[0];
        for(int i=1; i<(int)v.size(); i++)
        {
            if(v[i] == smallest)
                ans.push_back(i);
            else if(v[i] < smallest)
            {
                ans = std::vector<int>(1,i);
                smallest = v[i];
            }
        }
        return ans;
    };

    //returns the indexes of the values that are maximal
    template <class V>
    std::vector<int> amax(const std::vector<V> &v)
    {
        if(v.size() == 0)
            return std::vector<int>();

        std::vector<int> ans(1, 0);
        V largest = v[0];
        for(int i=1; i<(int)v.size(); i++)
        {
            if(v[i] == largest)
                ans.push_back(i);
            else if(v[i] > largest)
            {
                ans = std::vector<int>(1,i);
                largest = v[i];
            }
        }
        return ans;
    };

    //returns [v1 | v2]
    template <class V>
    std::vector<V> operator|(const std::vector<V> &v1, const std::vector<V> &v2)
    {
        std::vector<V> v = v1;
        for(int i=0; i<(int)v2.size(); i++)
            v.push_back(v2[i]);
        return v;
    };

    //appends v2 to v1
    template <class V>
    std::vector<V>& operator|=(std::vector<V> &v1, const std::vector<V> &v2)
    {
        for(int i=0; i<(int)v2.size(); i++)
            v1.push_back(v2[i]);
        return v1;
    };

    //template vector input function
    template <class V>
    std::istream& operator>>(std::istream &is, std::vector<V> &v)
    {
        for(int i=0; i<(int)v.size(); i++)
            is >> v[i];
        return is;
    };

    //template vector output function
    template <class V>
    std::ostream& operator<<(std::ostream &os, const std::vector<std::vector<V> > &v)
    {
        for(int i=0; i<(int)v.size(); i++)
            os << v[i] << std::endl;

        return os;
    };

    template <class V>
    std::ostream& operator<<(std::ostream &os, const std::vector<V> &v)
    {
        if(v.size() > 0)
            os << v[0];
        for(int i=1; i<(int)v.size(); i++)
            os << " " << v[i];

        return os;
    };


}

#endif //CONST_H_
