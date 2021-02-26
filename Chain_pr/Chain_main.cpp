#include <iostream>
#include <math.h>
#include <ctime>

#define K0_const 1e12 //�������^(-1)
#define k_bol 8.625E-5 //��������� ��������� ��/�
#define Temperature 130 // K������

using namespace std;
struct Rate_Catalog
{
	int pos_from; //[0,99]
	double move_left_barieer;   
	double move_right_barieer;   
	double rate_left;
	double rate_right;
};
struct Possible_Events {
	int from;
	int to;
	double rate;
};

void find_rate(bool * chain_bool, int pos, double temperature);

Rate_Catalog * mas_rate = new Rate_Catalog[100];
Possible_Events * events = new Possible_Events[201];
int mem_pos_to = 0;

void find_rang(bool * chain_bool, int pos, double t) {
	int l = 1;
	while (chain_bool[((int)100 + (pos - l)) % 100] != true && l < 5) {// ���� �������� �������� ������� ����� � ���������� �����
		l++;
	}
	int r = 1;
	while (chain_bool[(pos + r) % 100] != true && r < 5) {// ���� �������� �������� ������� ����� � ���������� ������
		r++;
	}
	if (l == 1) { 
		mas_rate[pos].move_left_barieer = -1; 
		if (r == 1) mas_rate[pos].move_right_barieer = -1;
		if (r == 2) mas_rate[pos].move_right_barieer = 0.37;
		if (r >= 3) mas_rate[pos].move_right_barieer = 0.295;
	}
	else {
		if (l == 2) {
			switch (r)
			{
			case 1: 
				mas_rate[pos].move_left_barieer = 0.37;
				mas_rate[pos].move_right_barieer = -1;
				break;
			case 2:
				mas_rate[pos].move_left_barieer = 0.235;
				mas_rate[pos].move_right_barieer = 0.235;
				break;
			case 3:
				mas_rate[pos].move_left_barieer = 0.235;
				mas_rate[pos].move_right_barieer = 0.241;
				break;
			default:
				mas_rate[pos].move_left_barieer = 0.235;
				mas_rate[pos].move_right_barieer = 0.241;
				break;
			}
		}
		else { 
			if (l == 3) {
				switch (r)
				{
				case 1:
					mas_rate[pos].move_left_barieer = 0.295;
					mas_rate[pos].move_right_barieer = -1;
					break;
				case 2:
					mas_rate[pos].move_left_barieer = 0.241;
					mas_rate[pos].move_right_barieer = 0.235;
					break;
				default:
					mas_rate[pos].move_left_barieer = 0.241;
					mas_rate[pos].move_right_barieer = 0.241;
					break;
				}
			}
			else {
				if (l >= 3) {
					switch (r)
					{
					case 1:
						mas_rate[pos].move_left_barieer = 0.295;
						mas_rate[pos].move_right_barieer = -1;
						break;
					case 2:
						mas_rate[pos].move_left_barieer = 0.241;
						mas_rate[pos].move_right_barieer = 0.235;
						break;
					default:
						mas_rate[pos].move_left_barieer = 0.241;
						mas_rate[pos].move_right_barieer = 0.241;
						break;
					}
				}
			}
			
		}
	}
	find_rate(chain_bool, pos, t);
};
void find_rate(bool * chain_bool, int pos, double temperature) {
	if(mas_rate[pos].move_left_barieer > 0)
	mas_rate[pos].rate_left = (double)( K0_const )* exp((double)((-1)* mas_rate[pos].move_left_barieer / (k_bol) / temperature));
	if (mas_rate[pos].move_right_barieer > 0)
	mas_rate[pos].rate_right = (double)( K0_const )* exp((double)((-1)* mas_rate[pos].move_right_barieer / (k_bol) / temperature));
};
void change_rate_catalog(bool * chain_bool) {
	int i = 0;
	find_rang(chain_bool, mem_pos_to, Temperature);
	for (int i = 1; i < 5; i++) {
		if (chain_bool[(100 + mem_pos_to - i) % 100]) {
			find_rang(chain_bool, (100 + mem_pos_to - i) % 100, Temperature);
			break;
		}
	}
	for (int i = 1; i < 5; i++) {
		if (chain_bool[(mem_pos_to + i) % 100]) {
			find_rang(chain_bool, (mem_pos_to + i) % 100, Temperature);
			break;
		}
	}
};

void choose_event(int * chain_int, bool* chain_bool) {
	//
	int k = 1;
	int m = 0;
	while (chain_int[m] >= 0) {
		int pos = (chain_int[m] - 1 + 100) % 100;
		if (!chain_bool[pos]) {
			events[k].from = chain_int[m];
			events[k].to = pos;
			events[k].rate = mas_rate[(pos + 1) % 100].rate_left;
			k++;
		}
		pos = (pos + 2) % 100;
		if (!chain_bool[pos]) {
			events[k].from = chain_int[m];
			events[k].to = pos;
			events[k].rate = mas_rate[(pos - 1 + 100) % 100].rate_right;
			k++;
		}
		m++;
	}
	double sum[201]; //������ ���� ���� ������������

	sum[0] = 2.7;
	int i = 1;
	while (i < k) {
		sum[i] = sum[i - 1] + events[i].rate;
		i++;
	}

	double rand1 = ((double)(rand()) / RAND_MAX) * sum[i - 1];
	int j = 0;
	for (j; sum[j] <= rand1; j++) {}

	if (j == 0) { //������� ���������
		int rand2 = 0;
		do {
			rand2 = (int)(((double)(rand()) / RAND_MAX) * 99); //��������� �������
		} while (chain_bool[rand2]); //�� ��� ��� ���� �� ������� � ������ �������
		chain_int[m] = rand2;
		chain_bool[rand2] = true;
		mem_pos_to = rand2;
	}
	else { //������� ��������
		int t = 0;
		for (t; chain_int[t] != events[j].from && t < 101; t++) {}
		if (t >= 50) cout << "Out of array" << endl;
		chain_int[t] = events[j].to;
		chain_bool[events[j].from] = false;
		chain_bool[events[j].to] = true;
		mem_pos_to = events[j].to;
	}

}


int main()
{
	unsigned int start_time = clock();
	int * chain_int = new int[100];
	bool * chain_bool = new bool[100];
	for (int i = 0; i < 100; i++) {
		chain_int[i] = -1;
		chain_bool[i] = false;
	}

	chain_int[0] = 0;
	chain_bool[0] = true;
	mem_pos_to = 0;
	mas_rate[0].rate_left = 462;
	mas_rate[0].rate_right = 462;
	choose_event(chain_int, chain_bool);
	change_rate_catalog(chain_bool);

	//1 ���� (���������)
	for (int i = 1; i < 10000; i++) {
		if (i % 100 == 0) {
			for (int k = 0; k < 100; k++) {
				if (chain_bool[k]) cout << k << ' ';
			}
			cout << endl;
		}
		choose_event(chain_int, chain_bool);
		change_rate_catalog(chain_bool);
	}


	delete[] chain_bool;
	delete[] chain_int;
	delete[] mas_rate;
	delete[] events;

	unsigned int end_time = clock(); // �������� �����
	unsigned int search_time = end_time - start_time; // ������� �����
	setlocale(LC_ALL, "Russian");
	cout << "����� ���������� ��������� " <<search_time << " ��" << endl;
	system("pause");
	return 0;
}