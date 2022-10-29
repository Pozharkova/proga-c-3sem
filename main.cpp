
#include <iostream>
#include <string>
#include <vector>

using namespace std;
struct Point //����� ����� �� ������� ������������
{
    unsigned long long const x, y;
    Point(unsigned long long x, unsigned long long y): x(x), y(y) {}

    Point minx(Point const & rha) const
    {
        return Point(rha.x < x ? rha.x : x, y);
    }

    Point miny(Point const & rha) const
    {
        return Point(x, rha.y < y ? rha.y : y);
    }

    Point maxx(Point const & rha) const
    {
        return Point(rha.x > x ? rha.x : x, y);
    }

    Point maxy(Point const & rha) const
    {
        return Point(x, rha.y > y ? rha.y : y);
    }

    void print () const
    {
        cout << '(' << x << ',' << y << ')';
    }
};
struct Rectangle //����� �������������
{
    Point RUpoint; //������� ������� �������� ����
    Rectangle(): RUpoint(Point(0, 0)) {} //����������� ��� ���������� ��� �������� �������������� � ������ ������� ����� � ������ ���������
    Rectangle(Point const & RUpoint): RUpoint(RUpoint) {} //����������� � ����� ���������� ��� �������� �������������� � ������ ������� ����� � �������� �����
    Rectangle operator + (Rectangle const &rha) const // �������� ���������� �������������� � ������������ ������������, ����������� ��� ��������������
    {
        return Rectangle(RUpoint.maxx(rha.RUpoint).maxy(rha.RUpoint));
    }
    Rectangle operator*(Rectangle const &rha) const // �������� ���������� ��������������, ����������� ������������ ���� ���������������
    {
        return Rectangle(RUpoint.minx(rha.RUpoint).miny(rha.RUpoint));
    }
    Rectangle operator=(Rectangle const &rha) const // �������� ������������
    {
        return rha;

    }

    void print () const //������ �� ������ ���������� ������� �������� ���� ��������������
    {
        RUpoint.print();
    }
};

Point parse_point(string s) //����������� ������ ���� (x, y) � Point(x, y)
{
    string curr_s = s;
    while (curr_s.find(' ') != string::npos) //������ ��������
    {
        curr_s.erase(curr_s.find(' '), 1);
    }
    unsigned long long x,y;
    int z_pos = curr_s.find(',');
    x = stoi(curr_s.substr(1, z_pos - 1));
    y = stoi(curr_s.substr(z_pos + 1, curr_s.length() - z_pos - 2));
    return Point(x, y);
}


int main()
{
    string expression;
    getline(cin, expression);
    vector <Rectangle> Rect; //������ ���������������
    vector <int> oper; //������ ��������
    size_t pos1 = 0;
    size_t pos2 = 0;
    while (pos1 < expression.length()) // ��������� ��������� �� �������������� � ��������
    {
        while (expression[pos2] != '+' && expression[pos2] != '*' && pos2 <= expression.length()) //����� ������ ������� �������������� � ��������� (�������� ��� ����� ���������)
            pos2++;
        if (pos2 < expression.length())
        {
            if (expression[pos2] == '*')
                oper.push_back(0);
            else
                oper.push_back(1);
        }
        Rect.push_back(Rectangle(parse_point(expression.substr(pos1, pos2 - pos1))));
        pos2++;
        pos1 = pos2;
    }

    pos1 = 0;while (pos1 < oper.size()) //���������� ���� �������� * (������� ���������)
    {
        if (oper[pos1] == 0)
        {
            vector<Rectangle> Rect1;
            for (int i = 0; i < pos1; i++)
                Rect1.push_back(Rect[i]);
            Rect1.push_back(Rect[pos1] * Rect[pos1 + 1]);
            for(int i = pos1 + 2; i < Rect.size(); i++)
                Rect1.push_back(Rect[i]);
            Rect.erase(Rect.begin(), Rect.end());
            Rect = Rect1;
            oper.erase(oper.begin() + pos1);
            for(int i = 0;  i< oper.size(); i++ )
            {
                Rect[i].print();
                if (oper[i] == 0)
                    cout << "*";
                else
                    cout << "+";
            }
            Rect[Rect.size() - 1]. print();
            cout << endl;
        }

        else pos1++;

    }
    pos1 = 0;
    while (pos1 < oper.size())
    {
        vector<Rectangle> Rect1;
        for (int i = 0; i < pos1; i++)
            Rect1.push_back(Rect[i]);
        Rect1.push_back(Rect[pos1] + Rect[pos1 + 1]);
        for(int i = pos1 + 2; i < Rect.size(); i++)
            Rect1.push_back(Rect[i]);
        Rect.erase(Rect.begin(), Rect.end());
        Rect = Rect1;
        oper.erase(oper.begin() + pos1);
        for(int i = 0;  i< oper.size(); i++ )
            {
                Rect[i].print();
                if (oper[i] == 0)
                    cout << " * ";
                else
                    cout << " + ";
            }
            Rect[Rect.size() - 1]. print();
            cout << endl;

    }

    cout << " = ";
    Rect[0].print(); //����� ����������
    return 0;
}
