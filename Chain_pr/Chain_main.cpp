#include <iostream>
#include <math.h>
#include <ctime>
#include <fstream>

#define K0_const 1e12 //секунда^(-1)
#define k_bol 8.625E-5 //константа Больцмана еВ/К
#define Temperature_1 130 // Kельвин начальная температура
#define Temperature_2 120 // конечная температура
#define TAY 100//[секунды] тау в показателе экспаненты изменения температуры
#define ITERATIONS_OF_EXP 1


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
int mem_pos_to = 0; 
int chain_lenth_distribution[100];
double time1;
unsigned int avg_atoms;
unsigned int avg_ot;
unsigned int avg_nap;
double T1;

void file_distribution_output(ofstream& f2out);
void file_chain_output(bool * chain_bool, ofstream& f2out);
void find_rang(bool * chain_bool, int pos, double t) { // содержит вызов find_rate()
	int l = 1;
	while (chain_bool[((int)100 + (pos - l)) % 100] != true && l < 5) {// ищем разность позициий данного атома и ближайшего слева
		l++;
	}
	int r = 1;
	while (chain_bool[(pos + r) % 100] != true && r < 5) {// ищем разность позициий данного атома и ближайшего справа
		r++;
	}
	if (l == 1) { 
		mas_rate[pos].move_left_barieer = 1000; 
		if (r == 1) mas_rate[pos].move_right_barieer = 1000;
		if (r == 2) mas_rate[pos].move_right_barieer = 0.37;
		if (r >= 3) mas_rate[pos].move_right_barieer = 0.295;
	}
	else {
		if (l == 2) {
			switch (r)
			{
			case 1: 
				mas_rate[pos].move_left_barieer = 0.37;
				mas_rate[pos].move_right_barieer = 1000;
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
					mas_rate[pos].move_right_barieer = 1000;
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
						mas_rate[pos].move_right_barieer = 1000;
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
void create_rate_catalog(bool * chain_bool, double t) {
	for (int i = 0; i < 100; i++) {
		if (chain_bool[i]) {
			find_rang(chain_bool, i, t);
		}
	}
};
void change_rate_catalog(bool * chain_bool, double t)
{
	int i = 0;
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
	}
};
void choose_event(int * chain_int, bool* chain_bool, bool new_atoms) {
	//
	int k = 1;
	int m = 0;
	while (chain_int[m] >= 0) {
		int pos = (chain_int[m] - 1 + 100) % 100;
		if (!chain_bool[pos]) { // если слева от атома есть место
			events[k].from = chain_int[m];
			events[k].to = pos;
			events[k].rate = mas_rate[(pos + 1) % 100].rate_left;
			k++;
		}
		pos = (pos + 2) % 100;
		if (!chain_bool[pos]) { // если справа от атома есть место
			events[k].from = chain_int[m];
			events[k].to = pos;
			events[k].rate = mas_rate[(pos - 1 + 100) % 100].rate_right;
			k++;
		}
		m++;
	}
	double sum[201]; //массив сумм всех вероятностей


	if (new_atoms) { // если напыление включено
		sum[0] = 2.7;
	}
	else { //напыление выключено
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

	if (j == 0) { //событие напыления
		if (!new_atoms) {
			std::cout << "Try to add atom" << endl;
		}
		int rand2 = 0;
		if (chain_int[99] >= 0) {
			std::cout << "Too many atoms" << endl;
		}
		do {
			rand2 = (int)(((double)(rand()) / RAND_MAX) * 99); //случайная позиция
		} while (chain_bool[rand2]); //до тех пор пока не попадем в пустую позицию
		chain_int[m] = rand2;
		chain_bool[rand2] = true;
		mem_pos_to = rand2;
	}
	else { //событие движения
		int t = 0;
		for (t; chain_int[t] != events[j].from && t < 100; t++) {}
		if (t >= 100) { 
			std::cout << "Out of array chain_int" << endl;
		}
		chain_int[t] = events[j].to;
		chain_bool[events[j].from] = false;
		chain_bool[events[j].to] = true;
		mem_pos_to = events[j].to; /////не используется
	}
	//Счет времени
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

	int t1 = Temperature_1;
	int t2 = Temperature_2;
	int it_ex = ITERATIONS_OF_EXP;
	double tay = TAY;

	avg_atoms = 0;
	avg_ot = 0;
	avg_nap = 0;
	double avg1_time = 0;
	double avg2_time = 0;
	double avg1_temp = 0;
	double avg2_temp = 0;

	for (int index = 0; index < ITERATIONS_OF_EXP; index++) {
		if (int(ITERATIONS_OF_EXP / 100) != 0) {
			if (int(index % int(ITERATIONS_OF_EXP / 100)) == 0) std::cout << "\r" << int(100 * double(index) / ITERATIONS_OF_EXP) << "%";
			if ((index + 1) % ITERATIONS_OF_EXP == 0) std::cout << endl;
		}

		T1 = Temperature_1;
		time1 = 0;

		f2out << "\\\\\\\\\\\\\\ \t Серия " << index + 1 << "\t \\\\\\\\\\\\\\" << endl;
		srand(index);
		for (int i = 0; i < 100; i++) {
			chain_int[i] = -1;
			chain_bool[i] = false;
		}
		
		//1 ЭТАП (Напыление)
		int i_nap = 1;

		for (i_nap = 1; chain_int[35] < 0; i_nap++) {
			choose_event(chain_int, chain_bool, true);
			change_rate_catalog(chain_bool, T1);
		}

		//вывод в файл
		f2out << "НАПЫЛЕНИЕ" << endl;
		f2out << "Температура после напыления: " << T1 << endl;
		int count_number_of_adatoms = 0;
		for (int i = 0; i < 100; i++) { if (chain_bool[i]) count_number_of_adatoms++; }
		avg_atoms = avg_atoms + count_number_of_adatoms;
		f2out << "Время напыления: " << time1 << endl;
		f2out << "Число итераций напыления: " << i_nap << endl;

		avg1_time += time1;
		avg1_temp += T1;
		avg_nap += i_nap;

		//2 ЭТАП (Отжиг)
		time1 = 0;
		create_rate_catalog(chain_bool, T1);
		int i_ot = 0;
		bool flag = index == 0;
		ofstream fout;
		if (flag) {
			fout.open("output_temperature.txt", ios::out);
		}
		for ( i_ot = 1;  time1 < 1200; i_ot++) {
			if (flag) {
				fout << time1 << " " << T1 << endl;
			}
			choose_event(chain_int, chain_bool, false);
			T1 = (Temperature_1 - Temperature_2) * exp((-1) * time1 / TAY) + Temperature_2;
			create_rate_catalog(chain_bool, T1);
		}
		if (flag) {
			fout.close();
		}

		avg_ot += i_ot;

		f2out << "ОТЖИГ:" << endl;
		f2out << "Длины цепочек:" << endl;
		file_chain_output(chain_bool, f2out); //подсчет распределения
		f2out << "Температура после отжига: " << T1 << endl;
		f2out << "Время отжига: " << time1 << endl;
		f2out << "Число итераций отжига: " << i_ot << endl;
		f2out << endl;

		avg2_time += time1;
		avg2_temp += T1;

	}

	//Вывод результатов в файл
	file_distribution_output(f2out); 

	f2out << -1 << endl;
	f2out << "T1: " << t1 << " ";
	f2out << "T2: " << t2 << endl;
	f2out << "Пов-ий эксп: " << it_ex << " ";
	f2out << "Адатомов: " << avg_atoms / (ITERATIONS_OF_EXP) << "/100" << endl;
	f2out << "Тау: " << tay << " секунд" << endl;

	f2out << "Ср. вр. нап: " << avg1_time / (ITERATIONS_OF_EXP) << "сек" << endl;
	f2out << "Ср. ч-ло ит. нап: " << avg_nap / (ITERATIONS_OF_EXP) << endl;
	f2out << "Ср. т-ра после нап: " << avg1_temp / (ITERATIONS_OF_EXP) << "K" << endl;

	f2out << "Ср. вр. отжига: " << avg2_time / (ITERATIONS_OF_EXP) << "cек" << endl;
	f2out << "Ср. ч-ло ит. отжига: " << avg_ot / (ITERATIONS_OF_EXP) << endl;
	f2out << "Ср. т-ра после отжига: " << avg2_temp / (ITERATIONS_OF_EXP) << endl;
	
	
	

	delete[] chain_bool;
	delete[] chain_int;
	delete[] mas_rate;
	delete[] events;

	unsigned int end_time = clock(); // конечное время
	unsigned int search_time = end_time - start_time; // искомое время
	std::cout << "Время выполнения программы " <<search_time << " мс" << endl;

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
	f2out << "Среднее число напыленных атомов: " << avg_atoms / (ITERATIONS_OF_EXP) << endl;
	f2out << endl << "(Длина, число отсчетов)" << endl;
	for (int i = 0; i < 40; i++) {
		f2out << i << " " << chain_lenth_distribution[i] << endl;
	}
	for (int i = 40; i < 100; i++) {
		if (chain_lenth_distribution[i] != 0) f2out << i << " " << chain_lenth_distribution[i] << endl;
	}
}