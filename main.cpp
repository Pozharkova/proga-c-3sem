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
class PriorityQueue //элемент очереди
{
private:
    size_t HeapSize;//размер кучи
    T * First; //указатель на вершину кучи
    Comparator <T> & comparator; //компаратор
    T * parent(T * x) //возвращает указатель на предка элемента x
    {
        return First + (x - First - 1) / 2;
    }
    T * left(T * x) //возвращает указатель на левого потомка элемента x
    {
        return First + (x - First) * 2 + 1;
    }
    T * right(T * x) //возвращает указатель на правого потомка элемента x
    {
        return First + (x - First) * 2 + 2;
    }

    void heapify(T * x) // перестройка кучи при удалении элемента
    {
        if (!is_empty())
        {
            auto curr = x;//текущий узел
            auto l = left(x);
            auto r = right(x);
            if (l - First < HeapSize - 1 && comparator(*l, *curr))
                curr = l;
            if (r - First < HeapSize - 1 && comparator(*r, *curr))
                curr = r;
            if (curr != x)
            {
                auto tmp = *x;
                *x = *curr;
                *curr = tmp;
                heapify(curr);
            }
        }
    }
public:
        PriorityQueue(Comparator <T> & comparator): First(nullptr), HeapSize(0), comparator(comparator) {}//конструктор
        void push(T x) //добавление элемента
        {
            if (is_empty())
            {
                First = (T *)malloc(sizeof(T));
                * First = x;
                HeapSize = 1;
            }
            else
            {
                HeapSize++;
                First = (T *)realloc(First, HeapSize * sizeof(T));
                *(First + HeapSize - 1) = x;
                auto i = First + HeapSize - 1;
                while (i > First && comparator(*i, *(parent(i))))
                {
                    auto tmp = *i;
                    *i = *(parent(i));
                    *(parent(i)) = tmp;
                    i = parent(i);
                }
            }
        }
        bool is_empty() //пуста ли очередь
        {
            return HeapSize==0;
        }
        T peek() //чтение верхнего элемента
        {
            if (is_empty())
            {
                std::cout << "Queue is empty";
            }
            return *First;
        }
        void poll() // удаление верхнего элемента
        {
            if (!is_empty())
            {
                *First = *(First + HeapSize - 1); //Присвоение верхнему элементу очереди значения последнего
                HeapSize--;
                First = (T *)realloc(First, HeapSize * sizeof(T));
                heapify(First);
            }
        }
        void free()
        {
            while (!is_empty())
                poll();
        }
};

int main()
{

    int vec[] = {-14, 10, -111, 19, 28, 25};
    IntComparator intcomparator = IntComparator{};
    PriorityQueue <int> PQ =  PriorityQueue<int>(intcomparator);
    std::cout << "Int Queue:" << std::endl;
    for (int i = 0; i < 6; i++)
    {
        std::cout << "push(" << vec[i] << ");" << "peek():";
        PQ.push(vec[i]);
        std::cout << PQ.peek() << std::endl;
    }
    for (int i = 0; i < 6; i++)
    {
        std::cout << "peek():" << PQ.peek() << "; poll(); peek():";
        PQ.poll();
        if (!PQ.is_empty())
            std::cout << PQ.peek() << std::endl;
        else
            std::cout << "Queue is empty" << std::endl;
    }
    PQ.free();



    double vec2[] = {1.4, 1.0, -1.1, -1.9, 0.2, 2.5};
    DoubleComparator doublecomparator = DoubleComparator{};
    PriorityQueue <double> PQ2 =  PriorityQueue<double>(doublecomparator);
    std::cout << "Double Queue:" << std::endl;
    for (int i = 0; i < 6; i++)
    {
        std::cout << "push(" << vec2[i] << ");" << "peek():";
        PQ2.push(vec2[i]);
        std::cout << PQ2.peek() << std::endl;
    }
    for (int i = 0; i < 6; i++)
    {
        std::cout << "peek():" << PQ2.peek() << "; poll(); peek():";
        PQ2.poll();
        if (!PQ2.is_empty())
            std::cout << PQ2.peek() << std::endl;
        else
            std::cout << "Queue is empty" << std::endl;
    }
    PQ2.free();

    char vec3[] = {'a', 'B', '4', 'c', 'Y', '!'};
    CharComparator charcomparator = CharComparator{};
    PriorityQueue <char> PQ3 =  PriorityQueue<char>(charcomparator);
    std::cout << "Char Queue:" << std::endl;
    for (int i = 0; i < 6; i++)
    {
        std::cout << "push(" << vec3[i] << ");" << "peek():";
        PQ3.push(vec3[i]);
        std::cout << PQ3.peek() << std::endl;
    }
    for (int i = 0; i < 6; i++)
    {
        std::cout << "peek():" << PQ3.peek() << "; poll(); peek():";
        PQ3.poll();
        if (!PQ3.is_empty())
            std::cout << PQ3.peek() << std::endl;
        else
            std::cout << "Queue is empty" << std::endl;
    }
    PQ3.free();

    return 0;
}
