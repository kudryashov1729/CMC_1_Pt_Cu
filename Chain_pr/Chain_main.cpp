#include <iostream>

#define K0_const 1e12 //секунда^(-1)
#define k_bol 8.625E-5 //константа Больцмана еВ/К
#define Temperature 300 // Kельвин

using namespace std;
struct Rate_Catalog
{
	int pos_from; //[0,99]
	double move_left_barieer;   
	double move_right_barieer;   
	double rate_left;
	double rate_right;
};

Rate_Catalog * mas_rate = new Rate_Catalog[100];
int mem_pos_from = 0;
void find_rang(bool * chain_bool, int pos) {
	int l = 1;
	while (chain_bool[((int)100 + (pos - l)) % 100] != true && l < 5) {// ищем разность позициий данного атома и ближайшего слева
		l++;
	}
	int r = 1;
	while (chain_bool[(pos + r) % 100] != true && r < 5) {// ищем разность позициий данного атома и ближайшего справа
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
	
};
void find_rate(bool * chain_bool, int pos, double temperature) {
	mas_rate[pos].rate_left = (double)(K0_const)* exp((double)((-1)* mas_rate[pos].move_left_barieer / (k_bol) / temperature / 1000));
	mas_rate[pos].rate_right = (double)(K0_const)* exp((double)((-1)* mas_rate[pos].move_right_barieer / (k_bol) / temperature / 1000));
};
void change_rate_catalog() {
	for (int i = mem_pos_from - 3; i < mem_pos_from + 3; i++) {
		int rang[3] = { -1,0,-1 };
		
	}
};
void choose_event() {
	//
	double * sum = new double[201]; //массив сумм всех вероятностей

	sum[0] = mas_rate[chain_int[0]].rate_left; //вероятность прыжка влево
	sum[1] = sum[0] + mas_rate[chain_int[0]].rate_right; // вероятность прыжка вправо
	int i = 1;
	while (chain_int[i] >= 0 && i < 100) {
		sum[2 * i] = sum[2 * i - 1] + mas_rate[chain_int[i]].rate_left;
		sum[2 * i + 1] = sum[2 * i] + mas_rate[chain_int[i]].rate_right;
	}
	sum[2 * i] = sum[2 * i - 1] + 2.7;
	//
	delete[] sum;
	mem_pos_from = -1;
}
void move_atom() {

}

int main()
{
	int * chain_int = new int[100];
	bool * chain_bool = new bool[100];

	chain_bool[97] = true;
	chain_bool[2] = true;
	mem_pos_from = 0;
	
	find_rang(chain_bool, 0);
	cout << mas_rate[0].move_left_barieer << endl;
	cout << mas_rate[0].move_right_barieer << endl;


	//1 ЭТАП (Напыление)
	//create_rate_catalog();
	//choose_event();
	//move_atom();

	

	delete[] chain_bool;
	delete[] chain_int;
	delete[] mas_rate;
	getchar();
	return 0;
}