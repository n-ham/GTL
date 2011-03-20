#ifndef TENSOR_H_
#define TENSOR_H_

#include "Rational.h"
#include "../ngame/NStrategy.h"

namespace GTL
{
    template <class V>
    struct Tensor
    {
        std::vector<int> dimensions;
        std::vector<V> tensor;

        Tensor<V>()
        {

        };

        Tensor<V>(const std::vector<int> &Dimensions)
        {
            dimensions = Dimensions;
            tensor = std::vector<V>(product(dimensions), (V) 0);
        };

        //index function
        V& operator[](const NStrategy &strategy) const
        {
            return (V&) tensor[strategy.index];
        };
    };

    /*
        returns the expected value of the payoff tensor given the probability
        of each outcome occuring as given in the probabilities tensor.
    */
    template <class V>
    V sdot(const Tensor<V> &probabilities, const Tensor<V> &payoffs)
    {
        V ans = V(0);
        for(int i=0; i<(int)probabilities.tensor.size(); i++)
            ans += probabilities.tensor[i] * payoffs.tensor[i];
        return ans;
    };

    /*
        returns the expected value of each payoff tensor given the probability
        of each outcome occuring as given in the probabilities tensor.
    */
    template <class V>
    std::vector<V> smdot(const Tensor<V> &probabilities, const std::vector<Tensor<V> > &payoffs)
    {
        std::vector<V> ans(probabilities.dimensions.size(), V(0));
        for(int i=0; i<(int)probabilities.tensor.size(); i++)
            for(int p=0; p<(int)probabilities.dimensions.size(); p++)
                ans[p] += probabilities.tensor[i] * payoffs[p].tensor[i];
        return ans;
    };
}

#endif //TENSOR_H_
