#include <iostream>

using namespace std;
void swap_elem(void * lha, void * rha, size_t size_elem)
{
    void * tmp = malloc(size_elem);
    memcpy(tmp, lha, size_elem);
    memcpy(lha, rha, size_elem );
    memcpy(rha, tmp, size_elem);
    free(tmp);
}

void qs(void *first, size_t number, size_t size_elem, int ( * comparator )(const void *, const void *))
{
    if (number <= 0)
        return;
    auto x =  (number - 1) / 2;
    auto i = first;
    auto j = ( char *) first + size_elem * (number - 1);
    auto curr = (char *) first + size_elem * x;


     do
        {
            while (comparator(i, curr) < 0)
                i = (char *)i + size_elem;
            while (comparator(j, curr) > 0)
                j = (char *)j - size_elem;

            if(i <= j)
            {
                if (comparator(i, j) > 0)
                    swap_elem(i, j, size_elem);
                i = (char *)i + size_elem;
                j = (char *)j - size_elem;
            }
        }
    while (comparator(i, j) <= 0);

    if (i < (char *) first + (number - 1)* size_elem )
        {
            size_t num =((char *) first +  size_elem * (number - 1) - (char *) i) / size_elem + 1;
            qs(i, num, size_elem, comparator);
        }
    if (first < j)
        {
            size_t num =((char *) j - (char * ) first) / size_elem + 1;
            qs(first, num, size_elem, comparator);
        }
}
int compare(const void *x1, const void *x2)
{
    return (*(int*)x1 - *(int*)x2);
}

int compare_double(const void * x1, const void * x2) //функция сравнения двух чисел double
{
    if (*(double *)x1 > *(double *)x2) return 1;
    if (*(double *)x1 < *(double *)x2) return -1;
    if (*(double *)x1 == *(double *)x2) return 0;

}

int compare_char(const void * x1, const void * x2) //функция сравнения двух символов
{
    if (*(char *)x1 > *(char *)x2) return 1;
    if (*(char *)x1 < *(char *)x2) return -1;
    if (*(char *)x1 == *(char *)x2) return 0;

}
int main()
{
    int vector[] = {-14, 10, -111, 19, 28, 250};
    cout << "int array:" << endl << "Default:" << endl;
    for ( int i = 0; i < 6; i++)
        cout << vector[i] << " ";

    qs(vector, 6, sizeof(int), compare);
    cout << endl << "Sorted:" << endl;
    for ( int i = 0; i < 6; i++)
        cout << vector[i] << " ";

    double vector2[] = {1.4, 1.0, -1.1, -1.9, 0.2, 2.5};
    cout << endl << endl << "double array:" << endl << "Default:" << endl;
    for ( int i = 0; i < 6; i++)
        cout << vector2[i] << " ";

    qs(vector2, 6, sizeof(double), compare_double);
    cout << endl << "Sorted:" << endl;
    for ( int i = 0; i < 6; i++)
        cout << vector2[i] << " ";

    char vector3[] = {'a', 'B', '4', 'c', 'Y', '!'};
    cout << endl << endl << "char array:" << endl << "Default:" << endl;
    for ( int i = 0; i < 6; i++)
        cout << vector3[i] << " ";

    qs(vector3, 6, sizeof(char), compare_char);
    cout << endl << "Sorted:" << endl;
    for ( int i = 0; i < 6; i++)
        cout << vector3[i] << " ";

    int vector4[] = {-14, 10, -111, 19, 28, 250};
    cout << endl << endl << "int array (sort by first of pair):" << endl << "Default:" << endl;
    for ( int i = 0; i < 6; i++)
        cout << vector4[i] << " ";

    qs(vector4, 6/2, 2 * sizeof(int), compare);
    cout << endl << "Sorted:" << endl;
    for ( int i = 0; i < 6; i += 2)
        cout << "(" << vector4[i] << ";" << vector4[i + 1] << ") ";

    return 0;
}
