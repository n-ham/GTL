#ifndef CONST_H_
#define CONST_H_

#include <cstdlib>
#include <iostream>
#include <stdarg.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <stack>


namespace GTL
{
    /*
        These are so you can easily change the data type for utility and
        probability values, I did have templates for games, but having
        everything in .h files was far too messy for me to bear.

        If you change them to something that can't be passed to a
        stringstream, you will break any output function using toString.
    */
    #define UTIL double    //type for utility values
    #define PROB double    //type for probability values

    //converts a variable to a string
    template <class C>
    std::string toString(const C &c)
    {
        std::ostringstream oss;
        oss << c;
        return oss.str();
    };

    /*
        template vector functions
    */
    //creates a vector with specified elements (must be a 1d vector)
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
