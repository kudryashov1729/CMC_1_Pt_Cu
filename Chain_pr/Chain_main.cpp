#include <iostream>
#include <math.h>
#include <ctime>
#include <fstream>

#define K0_const 1e12 //�������^(-1)
#define k_bol 8.625E-5 //��������� ��������� ��/�
#define Temperature_1 150 // K������
#define Temperature_2 60 //����������� �� ���
#define TAY 900//[�������] ��� � ���������� ���������� ��������� �����������

#define ITERATIONS_OF_EXP 10

#define ITERATION_OF_STEP_1 100000
#define ITERATION_OF_STEP_2 100000

using namespace std;

struct Rate_Catalog
{
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
int mem_pos_to = 0; /////�� ������������
int chain_lenth_distribution[100];
double time1;
unsigned int avg_atoms;
double T1;

void file_distribution_output(ofstream& f2out);
void file_chain_output(bool * chain_bool, ofstream& f2out);
void find_rang(bool * chain_bool, int pos, double t) { // �������� ����� find_rate()
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
void change_rate_catalog(bool * chain_bool, double t) {

	for (int i = 0; i < 100; i++) {
		if (chain_bool[i]) {
			find_rang(chain_bool, i, t);
		}
	}

	/*int i = 0;
	find_rang(chain_bool, mem_pos_to, t);
	for (int i = 1; i < 5; i++) {
		if (chain_bool[(100 + mem_pos_to - i) % 100]) {
			find_rang(chain_bool, (100 + mem_pos_to - i) % 100, t);
			break;
		}
	}
	for (int i = 1; i < 5; i++) {
		if (chain_bool[(mem_pos_to + i) % 100]) {
			find_rang(chain_bool, (mem_pos_to + i) % 100, t);
			break;
		}
	}*/
};

void choose_event(int * chain_int, bool* chain_bool, bool new_atoms) {
	//
	int k = 1;
	int m = 0;
	while (chain_int[m] >= 0) {
		int pos = (chain_int[m] - 1 + 100) % 100;
		if (!chain_bool[pos]) { // ���� ����� �� ����� ���� �����
			events[k].from = chain_int[m];
			events[k].to = pos;
			events[k].rate = mas_rate[(pos + 1) % 100].rate_left;
			k++;
		}
		pos = (pos + 2) % 100;
		if (!chain_bool[pos]) { // ���� ������ �� ����� ���� �����
			events[k].from = chain_int[m];
			events[k].to = pos;
			events[k].rate = mas_rate[(pos - 1 + 100) % 100].rate_right;
			k++;
		}
		m++;
	}
	double sum[201]; //������ ���� ���� ������������


	if (new_atoms) { // ���� ��������� ��������
		sum[0] = 2.7;
	}
	else { //��������� ���������
		sum[0] = 0;
	}

	int i = 1;
	while (i < k) {
		sum[i] = sum[i - 1] + events[i].rate;
		i++;
	}
	int r1 = rand();
	double rand1 = ((double)(r1) / RAND_MAX) * sum[i - 1];
	int j = new_atoms ? 0 : 1;
	for (j; sum[j] < rand1; j++) {}
	if (j >= k) {
		j = 1;
		std::cout << "Out of array events" << endl;
	}

	if (j == 0) { //������� ���������
		if (!new_atoms) {
			std::cout << "Try to add atom" << endl;
		}
		int rand2 = 0;
		if (chain_int[99] >= 0) {
			std::cout << "Too many atoms" << endl;
		}
		do {
			rand2 = (int)(((double)(rand()) / RAND_MAX) * 99); //��������� �������
		} while (chain_bool[rand2]); //�� ��� ��� ���� �� ������� � ������ �������
		chain_int[m] = rand2;
		chain_bool[rand2] = true;
		mem_pos_to = rand2;
	}
	else { //������� ��������
		int t = 0;
		for (t; chain_int[t] != events[j].from && t < 100; t++) {}
		if (t >= 100) { 
			std::cout << "Out of array chain_int" << endl;
		}
		chain_int[t] = events[j].to;
		chain_bool[events[j].from] = false;
		chain_bool[events[j].to] = true;
		mem_pos_to = events[j].to; /////�� ������������
	}
	//���� �������
	int r2 = rand() + 1;
	double l = log((RAND_MAX + 1) / double(r2));
	time1 = time1 + (1 / sum[i - 1]) * l;
}


int main()
{
	setlocale(LC_ALL, "Russian");
	ofstream f2out;
	f2out.open("output.txt", ios::out);

	unsigned int start_time = clock();

	int * chain_int = new int[100];
	bool * chain_bool = new bool[100];

	for (int i = 0; i < 100; i++) {
		chain_lenth_distribution[i] = 0;
	}

	avg_atoms = 0;
	double avg1_time = 0;
	double avg2_time = 0;
	double avg1_temp = 0;
	double avg2_temp = 0;

	for (int index = 0; index < ITERATIONS_OF_EXP; index++) {

		//if (int(index % int(ITERATIONS_OF_EXP / 100)) == 0) std::cout << "\r" << int(100 * double(index) / ITERATIONS_OF_EXP) <<  "%";
		//if ((index + 1) % ITERATIONS_OF_EXP == 0) std::cout << endl;
		T1 = Temperature_1;
		time1 = 0;

		f2out << "\\\\\\\\\\\\\\ \t ����� " << index + 1 << "\t \\\\\\\\\\\\\\" << endl;
		srand(index + 30);
		for (int i = 0; i < 100; i++) {
			chain_int[i] = -1;
			chain_bool[i] = false;
		}
		
		//1 ���� (���������)
		for (int i = 1; (i < ITERATION_OF_STEP_1) && chain_int[35] < 0; i++) {

			choose_event(chain_int, chain_bool, true);
			change_rate_catalog(chain_bool, T1);

			T1 = Temperature_1 * exp((-1) * time1 / TAY);
		}

		//����� � ����
		f2out << "����� ��c�� ���������:" << endl;
		for (int k = 0; k < 100; k++) {
			if (chain_bool[k]) f2out << k << ' ';
		}
		f2out << endl;
		f2out << "����������� ����� ���������: " << T1 << endl;
		int count_number_of_adatoms = 0;
		for (int i = 0; i < 100; i++) { if (chain_bool[i]) count_number_of_adatoms++; }
		//f2out << "����������� ���������� ������: " << count_number_of_adatoms << endl;
		avg_atoms = avg_atoms + count_number_of_adatoms;
		f2out << "����� ���������: " << time1 << endl;

		avg1_time += time1;
		avg1_temp += T1;

		//2 ���� (�����)
		for (int i = 1; i < ITERATION_OF_STEP_2 && (T1 > Temperature_2); i++) {
			/*if (i % 10000 == 0) {
				int k = 0;
				int cnt = 0;
				for (k; chain_bool[k]; k++) {}
				for (int n = (k + 1) % 100; n != k; n = (n + 1) % 100) {
					if (chain_bool[n]) {
						cnt++;
					}
					else {
						if (cnt != 0) {
							cout << cnt << " | ";
							cnt = 0;
						}
					}
				}
				if (cnt != 0) {
					cout << cnt << endl;
					cnt = 0;
				}
				else {
					cout << "\b\b" << " " << "\b" << endl;
				}

			}*/
			choose_event(chain_int, chain_bool, false);
			change_rate_catalog(chain_bool, Temperature_2);
			T1 = Temperature_1 * exp((-1) * time1 / TAY);
		}
		f2out << "����� ��c�� ������:" << endl;
		for (int k = 0; k < 100; k++) {
			if (chain_bool[k]) f2out << k << ' ';
		}
		f2out << endl << "����� �������:" << endl;
		file_chain_output(chain_bool, f2out); //������� �������������
		f2out << "����������� ����� ������: " << T1 << endl;
		f2out << "����� ������: " << time1 << endl;
		f2out << endl;

		avg2_time += time1;
		avg2_temp += T1;

	}

	//����� ����������� � ����
	file_distribution_output(f2out); 
	int t1 = Temperature_1;
	int t2 = Temperature_2;
	int it_ex = ITERATIONS_OF_EXP;
	int it_s1 = ITERATION_OF_STEP_1;
	int it_s2 = ITERATION_OF_STEP_2;
	float tay = TAY;

	f2out << -1 << endl;
	f2out << "��������� �����������: " << t1 << endl;
	f2out << "���������� ���������� ������������: " << it_ex << endl;
	f2out << "���������� ���������� ������: " << it_s2 << endl;
	f2out << "����� ���������� ������: " << avg_atoms / (ITERATIONS_OF_EXP) << "/100" << endl;
	f2out << "���: " << tay << " ������" << endl;

	f2out << "������� ����� ������: " << avg1_time / (ITERATIONS_OF_EXP) << endl;
	f2out << "������� ����� ������������: " << avg2_time / (ITERATIONS_OF_EXP) << endl;
	f2out << "������� ���������� ����� ���������: " << avg1_temp / (ITERATIONS_OF_EXP) << endl;
	f2out << "������� ����������� ����� ������: " << avg2_temp / (ITERATIONS_OF_EXP) << endl;
	
	
	

	delete[] chain_bool;
	delete[] chain_int;
	delete[] mas_rate;
	delete[] events;

	unsigned int end_time = clock(); // �������� �����
	unsigned int search_time = end_time - start_time; // ������� �����
	std::cout << "����� ���������� ��������� " <<search_time << " ��" << endl;

	f2out.close();
	//system("output.txt");
	std::system("python py_vis.py");
	std::system("pause");
	return 0;
}

void file_chain_output(bool * chain_bool, ofstream& f2out)
{
	int k = 0;
	int cnt = 0;
	for (k; chain_bool[k]; k++) {}
	for (int n = (k + 1) % 100; n != k; n = (n + 1) % 100) {
		if (chain_bool[n]) {
			cnt++;
		}
		else {
			if (cnt != 0) {
				f2out << cnt << " | ";
				chain_lenth_distribution[cnt]++;
				cnt = 0;
			}
		}
	}
	if (cnt != 0) {
		f2out << cnt << endl;
		chain_lenth_distribution[cnt]++;
		cnt = 0;
	}
	else {
		f2out << endl;
	}
}

void file_distribution_output(ofstream& f2out) 
{
	f2out << "������� ����� ���������� ������: " << avg_atoms / (ITERATIONS_OF_EXP) << endl;
	f2out << endl << "(�����, ����� ��������)" << endl;
	for (int i = 0; i < 40; i++) {
		f2out << i << " " << chain_lenth_distribution[i] << endl;
	}
	for (int i = 40; i < 100; i++) {
		if (chain_lenth_distribution[i] != 0) f2out << i << " " << chain_lenth_distribution[i] << endl;
	}
}