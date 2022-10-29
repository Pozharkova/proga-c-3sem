#include <iostream>
#include<cassert>

template <typename T, std :: size_t N>
class Sizes final { //класс с размерностями сетки
public:
    using size_type = unsigned; //тип размерностей
    size_type gsize; //размер по оси N
    Sizes <T, N - 1> subsize; //класс меньшей размерности (вложенный класс)
    Sizes() : gsize(1), subsize(Sizes<T, N - 1>()) {} //пустой конструктор создает класс с размерностями 1х1х1...
    template <typename... Args>
    Sizes(size_type gsize, Args... subsize): gsize(gsize), subsize(subsize...) {} //конструктор
    size_type full_size(){//полный размер сетки, вычисляется рекурсивно
        return gsize * subsize.full_size();
    }
    template <typename... Args>
    size_type get_idx(unsigned idx, Args... subidx){//вычисление индекса точки сетки в одномерном массиве Т - рекурсивно
        return idx + gsize * subsize.get_idx(subidx...);
    }

    T get_t() {return subsize.get_t();} //значение аргумента t - последний элемент аргументов конструктора сетки вида Grid <тип, 3> (2, 3, 4, t);

    size_type get_size(std :: size_t n){ //размер по n-й оси
        auto csize = gsize;
        for (auto i = n; i > 0; i--)
            csize = subsize.get_size(n - 1);
        return csize;
    }
    Sizes(Sizes const &src) = default;//конструкторы и пр.
    Sizes& operator = (Sizes const &src) = default;
    Sizes(Sizes &&) = default;
    Sizes& operator = (Sizes &&) = default;
    ~Sizes() = default;
};


template <typename T> //уточнение класса при N=1 - конец рекурсии
class Sizes <T, 1> final {
public:
    using size_type = unsigned;
    size_type gsize;
    T t;
    Sizes() : gsize(1), t(1) {}
    template <typename... Args>
    Sizes(size_type gsize, T t): gsize(gsize), t(t) {} //конструктор с параметром t
    Sizes(size_type gsize): gsize(gsize), t(0) {} //конструктор без параметра t
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
    Grid(T * data, Sizes <T, N> sizes): data(data), sizes(sizes) {} //исходный конструктор с передачей размеров в виде объекта
    Grid(T const &t) : Grid(new T[1], Sizes <T, N> ()) {*data = t;} //конструктор с параметром t
    Grid(Sizes <T, N> sizes) : Grid(new T[sizes.full_size()], sizes) {} //исходный конструктор без data - для конструкторов копирования
    template <typename... Args>
    Grid(T * data, Args... args): data(data), sizes(Sizes <T, N> (args...)) {} //исходный конструктор с передачей размеров в виде аргументов
    template <typename... Args>
    Grid(Args... args) : Grid(new T[Sizes <T, N> (args...).full_size()], Sizes <T, N> (args...)) { //конструктор с рамерностями в виде параметров
        T t = sizes.get_t();//считывание t из набора параметров
        for (size_type i = 0; i < sizes.full_size(); i++)
            *(data + i) = t;
    }
    ~Grid() {if (data) delete [] data;} //деструктор

    Grid (Grid <T, N> const &src) : Grid(src.sizes) { //конструктор копирования
        for (size_type i = 0; i < sizes.full_size(); i++)
            *(data + i) = *(src.data + i);
    }
    Grid (Grid <T, N> &&src) : Grid(src) { //конструктор перемещения
        std :: swap(this -> data, src.data);
        src.data = nullptr;
    }
    Grid <T, N> & operator = (Grid <T, N> &src) {//копирующее присваивание
        sizes = src.sizes;
        for (size_type i = 0; i < sizes.full_size(); i++)
        {
            *(data + i) = *(src.data + i);
        }
        return * this;
    }
    Grid <T, N> & operator = (Grid <T, N> &&src) {//перемещающее присваивание
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
    //тестовый код из задания
    Grid <float, 3> g3(2, 3, 4, 1.0f);
    assert (1.0f == g3(1, 1, 1));
    Grid <float, 2> g2(2, 5, 2.0f);
    assert (2.0f == g2(1, 1));
    g2 = g3[1];
    assert(1.0f == g2(1, 1));

    //тестовый код для демонстрации
    using gsize_t = Grid <int, 2> :: size_type;
    std :: cout << "Grid <int, 2> test_grid(5, 10, 2);" << std :: endl;
    Grid <int, 2> test_grid(5, 10, 2); //конструктор с 3 параметрами
    for (gsize_t y_idx = 0; y_idx != test_grid.sizes.get_size(0); ++y_idx)
    {
        for (gsize_t x_idx = 0; x_idx != test_grid.sizes.get_size(1); ++x_idx)
            std :: cout << test_grid[y_idx][x_idx] << " ";
        std :: cout << std :: endl;
    }
    Grid <int, 2> copy_grid(test_grid); //конструктор копирования
    std :: cout << "Grid <int, 2> copy_grid(test_grid);" << std :: endl;
    for (gsize_t y_idx = 0; y_idx != copy_grid.sizes.get_size(0); ++y_idx)
    {
        for (gsize_t x_idx = 0; x_idx != copy_grid.sizes.get_size(1); ++x_idx)
            std :: cout << copy_grid[y_idx][x_idx] << " ";
        std :: cout << std :: endl;
    }
    std :: cout << "test_grid[i][j] = (i + 1) * (j + 1);" << std :: endl;//поэлементное присваивание с [][]
    for (gsize_t y_idx = 0; y_idx != test_grid.sizes.get_size(0); ++y_idx)
        for (gsize_t x_idx = 0; x_idx != test_grid.sizes.get_size(1); ++x_idx)
            test_grid[y_idx][x_idx] = (y_idx + 1) * (x_idx + 1);

    for (gsize_t y_idx = 0; y_idx != test_grid.sizes.get_size(0); ++y_idx)
    {
        for (gsize_t x_idx = 0; x_idx != test_grid.sizes.get_size(1); ++x_idx)
            std :: cout << test_grid[y_idx][x_idx] << " ";
        std :: cout << std :: endl;
    }

    std :: cout << "copy_grid = test_grid;" << std :: endl;//оператор копирования
    copy_grid = test_grid;
    for (gsize_t y_idx = 0; y_idx != copy_grid.sizes.get_size(0); ++y_idx)
    {
        for (gsize_t x_idx = 0; x_idx != copy_grid.sizes.get_size(1); ++x_idx)
            std :: cout << copy_grid[y_idx][x_idx] << " ";
        std :: cout << std :: endl;
    }


    return 0;
}
