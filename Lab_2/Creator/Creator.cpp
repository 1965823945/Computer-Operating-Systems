#include <iostream>
#include <fstream>
using namespace std;

struct employee {
	int num;
	char name[10];
	double hours;
};

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		cout << "����Ĳ������� ���֧��ѧӧڧݧ�ߧ�� �ܧ�ݧڧ�֧��ӧ� ��ѧ�ѧާ֧����: \n";
		return 1;
	}

	const int numberOfEmployes = atoi(argv[2]);

	ofstream binFile(argv[1], ios::binary | ios::out);
	for (int i = 0; i < numberOfEmployes; i++)
	{
		employee node;
		cout << "\n�������� ���ӧ֧էڧ�� �ߧ�ާ֧�: ";
		cin >> node.num;
		cin.get();

		cout << "\n�����Ա������ ���ӧ֧էڧ�� �ڧާ� ������էߧڧܧ�: ";
		cin.getline(node.name, 10);
		
		cout << "\nEnter amount of hours: ";
		cin >> node.hours;
		cout << "\n=============================" << endl;
		binFile.write((char *)&node, sizeof(employee));
	}

	binFile.close();
}
