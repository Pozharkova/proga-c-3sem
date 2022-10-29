#include <iostream>

template <typename T>
struct Comparator {
    virtual bool operator ()(T const &, T const &) const = 0;
};

struct IntComparator final : Comparator<int> {
    bool operator () (int const &lha , int const &rha) const override {
        return lha < rha;
    }
};

struct DoubleComparator final : Comparator<double> {
    bool operator () (double const &lha , double const &rha) const override {
        return lha < rha;
    }
};

struct CharComparator final : Comparator<char> {
    bool operator () (char const &lha , char const &rha) const override {
        return lha < rha;
    }
};

template <typename T>
void qs(T * const first, T * const last, Comparator <T> &comparator)
{
    auto i = first;
    auto j = last;
    auto curr = first + (last - first) / 2;
     do
        {
        while (comparator(*i, *curr))
           i++;
        while (comparator(*curr, *j))
            j--;

        if (i <= j)
          {
            if (comparator(*j, *i))
                {
                    auto tmp = *i;
                    *i = *j;
                    *j = tmp;
                }
            i ++;
            j --;
          }
    }
    while (i <= j);

    if (i < last)
        qs(i, last, comparator);
     if (first < j)
        qs(first, j, comparator);
}

int main()
{

    int vec[] = {-14, 10, -111, 19, 28, 25};
    IntComparator intcomparator = IntComparator{};
    std::cout << "int array:" << std::endl << "Default:" << std::endl;
    for ( int i = 0; i < 6; i++)
        std::cout << vec[i] << " ";
    qs(vec, vec + 5, intcomparator);
    std::cout << std::endl << "Sorted:" << std::endl;
    for ( int i = 0; i < 6; i++)
        std::cout << vec[i] << " ";

    double vec2[] = {1.4, 1.0, -1.1, -1.9, 0.2, 2.5};
    DoubleComparator doublecomparator = DoubleComparator{};
    std::cout << std::endl << std::endl << "double array:" << std::endl << "Default:" << std::endl;
    for ( int i = 0; i < 6; i++)
        std::cout << vec2[i] << " ";

    qs(vec2, vec2 + 5, doublecomparator);
    std::cout << std::endl << "Sorted:" << std::endl;
    for ( int i = 0; i < 6; i++)
        std::cout << vec2[i] << " ";

    char vec3[] = {'a', 'B', '4', 'c', 'Y', '!'};
    CharComparator charcomparator = CharComparator{};
    std::cout << std::endl << std::endl << "char array:" << std::endl << "Default:" << std::endl;
    for ( int i = 0; i < 6; i++)
        std::cout << vec3[i] << " ";

    qs(vec3, vec3 + 5, charcomparator);
    std::cout << std::endl << "Sorted:" << std::endl;
    for ( int i = 0; i < 6; i++)
        std::cout << vec3[i] << " ";
    return 0;
}
