#include <iostream>
#include<cassert>

template <typename T, std :: size_t N>
class Sizes final { //����� � ������������� �����
public:
    using size_type = unsigned; //��� ������������
    size_type gsize; //������ �� ��� N
    Sizes <T, N - 1> subsize; //����� ������� ����������� (��������� �����)
    Sizes() : gsize(1), subsize(Sizes<T, N - 1>()) {} //������ ����������� ������� ����� � ������������� 1�1�1...
    template <typename... Args>
    Sizes(size_type gsize, Args... subsize): gsize(gsize), subsize(subsize...) {} //�����������
    size_type full_size(){//������ ������ �����, ����������� ����������
        return gsize * subsize.full_size();
    }
    template <typename... Args>
    size_type get_idx(unsigned idx, Args... subidx){//���������� ������� ����� ����� � ���������� ������� � - ����������
        return idx + gsize * subsize.get_idx(subidx...);
    }

    T get_t() {return subsize.get_t();} //�������� ��������� t - ��������� ������� ���������� ������������ ����� ���� Grid <���, 3> (2, 3, 4, t);

    size_type get_size(std :: size_t n){ //������ �� n-� ���
        auto csize = gsize;
        for (auto i = n; i > 0; i--)
            csize = subsize.get_size(n - 1);
        return csize;
    }
    Sizes(Sizes const &src) = default;//������������ � ��.
    Sizes& operator = (Sizes const &src) = default;
    Sizes(Sizes &&) = default;
    Sizes& operator = (Sizes &&) = default;
    ~Sizes() = default;
};


template <typename T> //��������� ������ ��� N=1 - ����� ��������
class Sizes <T, 1> final {
public:
    using size_type = unsigned;
    size_type gsize;
    T t;
    Sizes() : gsize(1), t(1) {}
    template <typename... Args>
    Sizes(size_type gsize, T t): gsize(gsize), t(t) {} //����������� � ���������� t
    Sizes(size_type gsize): gsize(gsize), t(0) {} //����������� ��� ��������� t
    size_type full_size(){
        return gsize;
    }
    size_type get_size(std :: size_t n){
        return gsize;
    }

    size_type get_idx(unsigned idx){
        return idx;
    }
    T get_t(){return t;}

};

template <typename T, std :: size_t N>
class Grid final {
public:
    using value_type = T;
    using size_type = unsigned;
    Sizes <T, N> sizes;
    T * const data;
    Grid(T * data, Sizes <T, N> sizes): data(data), sizes(sizes) {} //�������� ����������� � ��������� �������� � ���� �������
    Grid(T const &t) : Grid(new T[1], Sizes <T, N> ()) {*data = t;} //����������� � ���������� t
    Grid(Sizes <T, N> sizes) : Grid(new T[sizes.full_size()], sizes) {} //�������� ����������� ��� data - ��� ������������� �����������
    template <typename... Args>
    Grid(T * data, Args... args): data(data), sizes(Sizes <T, N> (args...)) {} //�������� ����������� � ��������� �������� � ���� ����������
    template <typename... Args>
    Grid(Args... args) : Grid(new T[Sizes <T, N> (args...).full_size()], Sizes <T, N> (args...)) { //����������� � ������������ � ���� ����������
        T t = sizes.get_t();//���������� t �� ������ ����������
        for (size_type i = 0; i < sizes.full_size(); i++)
            *(data + i) = t;
    }
    ~Grid() {if (data) delete [] data;} //����������

    Grid (Grid <T, N> const &src) : Grid(src.sizes) { //����������� �����������
        for (size_type i = 0; i < sizes.full_size(); i++)
            *(data + i) = *(src.data + i);
    }
    Grid (Grid <T, N> &&src) : Grid(src) { //����������� �����������
        std :: swap(this -> data, src.data);
        src.data = nullptr;
    }
    Grid <T, N> & operator = (Grid <T, N> &src) {//���������� ������������
        sizes = src.sizes;
        for (size_type i = 0; i < sizes.full_size(); i++)
        {
            *(data + i) = *(src.data + i);
        }
        return * this;
    }
    Grid <T, N> & operator = (Grid <T, N> &&src) {//������������ ������������
        Grid <T, N> tmp(std :: move(src));
        std :: swap(this -> sizes, src.sizes);
        std :: swap(this -> data, src.data);
    }

    T * operator[](size_type idx) {
        return data + idx * sizes.subsize.full_size();
    }

    T * operator[] (size_type idx) const {
        size_type n = sizes.subsize.full_size();
        T * res = new T[n];
        for (size_type i = 0; i < n; i++)
        {
            *(res + i) = *(data + idx * n + i);
        }
        return res;
    }
    template <typename... Args>
    T operator()(Args... idx) const {
        return data[sizes.get_idx(idx...)];
    }
    template <typename... Args>
    T & operator()(Args... idx){
        return data[sizes.get_idx(idx...)];
    }

    Grid <T, N> & operator =(T const & t){
    for (
        auto it = data, end = data + sizes.full_size();
        it != end; ++it
        ) *it = t;
    return * this;
    }

    Grid <T, N> & operator =(T const * t){
        for (size_type i = 0; i < sizes.full_size(); i++)
            *(data + i)= *(t + i);
    return * this;
    }
};


int main() {
    //�������� ��� �� �������
    Grid <float, 3> g3(2, 3, 4, 1.0f);
    assert (1.0f == g3(1, 1, 1));
    Grid <float, 2> g2(2, 5, 2.0f);
    assert (2.0f == g2(1, 1));
    g2 = g3[1];
    assert(1.0f == g2(1, 1));

    //�������� ��� ��� ������������
    using gsize_t = Grid <int, 2> :: size_type;
    std :: cout << "Grid <int, 2> test_grid(5, 10, 2);" << std :: endl;
    Grid <int, 2> test_grid(5, 10, 2); //����������� � 3 �����������
    for (gsize_t y_idx = 0; y_idx != test_grid.sizes.get_size(0); ++y_idx)
    {
        for (gsize_t x_idx = 0; x_idx != test_grid.sizes.get_size(1); ++x_idx)
            std :: cout << test_grid[y_idx][x_idx] << " ";
        std :: cout << std :: endl;
    }
    Grid <int, 2> copy_grid(test_grid); //����������� �����������
    std :: cout << "Grid <int, 2> copy_grid(test_grid);" << std :: endl;
    for (gsize_t y_idx = 0; y_idx != copy_grid.sizes.get_size(0); ++y_idx)
    {
        for (gsize_t x_idx = 0; x_idx != copy_grid.sizes.get_size(1); ++x_idx)
            std :: cout << copy_grid[y_idx][x_idx] << " ";
        std :: cout << std :: endl;
    }
    std :: cout << "test_grid[i][j] = (i + 1) * (j + 1);" << std :: endl;//������������ ������������ � [][]
    for (gsize_t y_idx = 0; y_idx != test_grid.sizes.get_size(0); ++y_idx)
        for (gsize_t x_idx = 0; x_idx != test_grid.sizes.get_size(1); ++x_idx)
            test_grid[y_idx][x_idx] = (y_idx + 1) * (x_idx + 1);

    for (gsize_t y_idx = 0; y_idx != test_grid.sizes.get_size(0); ++y_idx)
    {
        for (gsize_t x_idx = 0; x_idx != test_grid.sizes.get_size(1); ++x_idx)
            std :: cout << test_grid[y_idx][x_idx] << " ";
        std :: cout << std :: endl;
    }

    std :: cout << "copy_grid = test_grid;" << std :: endl;//�������� �����������
    copy_grid = test_grid;
    for (gsize_t y_idx = 0; y_idx != copy_grid.sizes.get_size(0); ++y_idx)
    {
        for (gsize_t x_idx = 0; x_idx != copy_grid.sizes.get_size(1); ++x_idx)
            std :: cout << copy_grid[y_idx][x_idx] << " ";
        std :: cout << std :: endl;
    }


    return 0;
}
