#ifndef TENSOR_H_
#define TENSOR_H_

#include "../ngame/NStrategy.h"

namespace GTL
{
    template <class T>
    struct Tensor
    {
        std::vector<int> dimensions;
        std::vector<T> tensor;

        Tensor<T>()
        {

        };

        //constructs a tensor of type T with dimensions d where all elements are 0
        Tensor<T>(const std::vector<int> &Dimensions)
        {
            dimensions = Dimensions;

            tensor = std::vector<T>(product(dimensions), (T) 0);
        };

        //index function
        T& operator[](const NStrategy &strategy) const
        {
            return (T&) tensor[strategy.index];
        };
    };

    template <class T, class M>
    T sdot(const Tensor<T> &A, const Tensor<M> &B)
    {
        T ans = (T) 0;
        for(int i=0; i<(int)A.tensor.size(); i++)
            ans += A.tensor[i] * ((T) B.tensor[i]);

        return ans;
    };

    template <class T, class M>
    std::vector<T> smdot(const Tensor<T> &A, const std::vector<Tensor<M> > &B)
    {
        int m = A.dimensions.size();
        std::vector<T> ans(m,(T) 0);

        for(int i=0; i<(int)A.tensor.size(); i++)
        {
            for(int j=0; j<m; j++)
            {
                ans[j] += A.tensor[i] * ((T) B[j].tensor[i]);
            }
        }

        return ans;
    };
}

#endif //TENSOR_H_
