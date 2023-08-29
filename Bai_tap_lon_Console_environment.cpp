#include <iostream>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <fstream>
#include <iomanip>

using namespace std;

#define ENTER 13
#define UP 72
#define DOWN 80
#define ESC 27
#define M 7 //so muc trong menu
#define H 2 //so muc trong menu phan tang


double heSo[20]; //He so cua ham f(x)
double heSo_ddf[20]; //He so cua dao ham bac 2 cua f(x)
int length = -1; //So bac cua ham f(x) + 1
int s = -1; //chu so thap phan can lam tron den
int count = 1; //bien chay menu
int c = 1; //bien chay menu phan tang
HANDLE hc = GetStdHandle(STD_OUTPUT_HANDLE);
ofstream file ("Bai_tap_lon_Console_environment.txt");
double N = 1.234; //Buoc nhay
double delta = 0.00001;

double f(double x){ //ham f(x)
	double tmp = 0;
	for(int i = 0; i < length; i++)
		tmp += heSo[i] * pow(x, length - i - 1);
	return tmp; 
}

double df(double x){ //dao ham cua f(x)
	return (f(x + delta) - f(x - delta)) / (2 * delta);
}

double ddf(double x){ //dao ham bac 2 cua f(x)
	return (f(x + delta) + f(x - delta) - 2 * f(x)) / (delta * delta);
}

void nhapHeSo_f(){
	//Nhap he so cho f(x)
	cout<<"    ---Hay nhap he so cho da thuc f(x)! (Chuc nang 1)---"<<endl;
	file<<"    ---Hay nhap he so cho da thuc f(x)! (Chuc nang 1)---"<<endl;
	cout<<"Nhap bac cua da thuc f(x): "; cin>>length;
	file<<"Nhap bac cua da thuc f(x): "<<length<<endl;
	length++;
	for(int i = 0; i < length; i++){
		cout<<"a"<<length - 1 - i<<" = "; cin>>heSo[i];
		file<<"a"<<length - 1 - i<<" = "<<heSo[i]<<endl;
	}
	
	//Tinh he so cho f''(x)
	for(int i = 0; i < length - 2; i++){
		heSo_ddf[i] = (length - 1 - i) * (length - 2 - i) * heSo[i];
	}
}

void nhapSoThapPhan(){
	cout<<"    ---Hay nhap so chu so thap phan sau dau phay! (Chuc nang 2)---"<<endl;
	file<<"    ---Hay nhap so chu so thap phan sau dau phay! (Chuc nang 2)---"<<endl;
	cout<<"Nhap so chu so thap phan can lam tron den: "; cin>>s;
	file<<"Nhap so chu so thap phan can lam tron den: "<<s<<endl;
}

double roundHM(double number, int s){ //round handmade -> ham lam tron number den s chu so thap phan sau dau phay
    int base = pow(10, s);
    double result = round(number * base) / base; 
    return result;    
}

double mienTQ(){ //Tra ve mien chua tat ca nghiem cua da thuc
	double tmp = fabs(heSo[1]);
	int n = length;
	for(int i = 2; i < n; i++)
		if(fabs(heSo[i]) > tmp)
			tmp = fabs(heSo[i]);
	return 1 + (tmp / fabs(heSo[0]));
}

vector<double> mienChuaNghiem(){ //Tra ve cac khoang phan ly cua da thuc chua trong 1 vector
	file<<"       Thuc hien chuc nang 3:"<<endl;
	vector<double> cacKPL;
	double b, a, i;
	if(length == -1){
		nhapHeSo_f();
		system("cls");
	}
	if(s == -1){
		nhapSoThapPhan();
		system("cls");
	}
	b = mienTQ();
	a = -b;
	i = a;
	while(i + 1/N < b){
		if(f(i) * f(i + 1/N) > 0)
			i = i + 1/N;
		else if(f(i) * f(i + 1/N) < 0){
			cacKPL.push_back(roundHM(i, s));
			cacKPL.push_back(roundHM(i + 1/N, s));
			i = i + 1/N;
		}
		else{
			cacKPL.push_back(roundHM(i + 1/N - delta, s));
			cacKPL.push_back(roundHM(i + 1/N + delta, s));
			if(i + 2/N < b) i = i + 2/N;
			else i = i + 1/N + delta;
		}
	}
	if(cacKPL.size() != 0){
		cout<<"Cac khoang phan ly nghiem tim duoc la: ";
		file<<"Cac khoang phan ly nghiem tim duoc la: ";
		for(int i = 0; i <= cacKPL.size() - 2; i += 2){
			cout<<setprecision(s)<<fixed<<"("<<cacKPL[i]<<", "<<cacKPL[i+1]<<")"<<" ";
			file<<setprecision(s)<<fixed<<"("<<cacKPL[i]<<", "<<cacKPL[i+1]<<")"<<" ";
		}
		cout<<endl;
		file<<endl;
	}
	else{
		cout<<"Khong the tim duoc khoang chua nghiem (Phuong trinh vo nghiem)"<<endl;
		file<<"Khong the tim duoc khoang chua nghiem (Phuong trinh vo nghiem)"<<endl;
	}
	return cacKPL;
}

vector<double> mienChuaNghiem1(){ //La ham mienChuaNghiem nhung khong co cac cau lenh in ra man hinh va viet vao file
	vector<double> cacKPL;
	double b, a, i;
	b = mienTQ();
	a = -b;
	i = a;
	while(i + 1/N < b){
		if(f(i) * f(i + 1/N) > 0)
			i = i + 1/N;
		else if(f(i) * f(i + 1/N) < 0){
			cacKPL.push_back(roundHM(i, s));
			cacKPL.push_back(roundHM(i + 1/N, s));
			i = i + 1/N;
		}
		else{
			if(i + 2/N < b)
				i = i + 2/N;
			if(f(i) == 0)
				i = i - 1/N;
		}
	}
	return cacKPL;
}

void chuanHoa(double a, double b, double tmp[]){ //Chuan hoa khoang (a, b) theo yeu cau va gan vao mang tmp
	double c;
	while(b-a > 0.5 || a-b > 0.5){
		c = roundHM((a + b) / 2, s);
		if(f(a) * f(c) < 0) b = c;
		else if(f(c) * f(b) < 0) a = c;
		else if(f(c) == 0){
			tmp[0] = c - 0.1;
			tmp[1] = c + 0.1;
		}
	}
	tmp[0] = a;
	tmp[1] = b;
}

vector<double> mienChuaNghiemChuanHoa(){ //Chon khoang phan ly dau tien trong chuc nang 1 va chuan hoa no
	file<<"       Thuc hien chuc nang 4:"<<endl;
	vector<double> cacKPL, tmpCacKPL;
	double tmp[2];
	if(length == -1){
		nhapHeSo_f();
		system("cls");
	}
	if(s == -1){
		nhapSoThapPhan();
		system("cls");
	}
	cacKPL = mienChuaNghiem1();
	if(cacKPL.size() != 0){
		chuanHoa(cacKPL[0], cacKPL[1], tmp);
		tmpCacKPL.push_back(tmp[0]);
		tmpCacKPL.push_back(tmp[1]);
		cout<<"Mot khoang phan ly nghiem (mac dinh la khoang dau tien lay tu chuc nang 1) sau chuan hoa la: ";
		file<<"Mot khoang phan ly nghiem (mac dinh la khoang dau tien lay tu chuc nang 1) sau chuan hoa la: ";
		cout<<setprecision(s)<<fixed<<"("<<tmpCacKPL[0]<<", "<<tmpCacKPL[1]<<")"<<" ";
		file<<setprecision(s)<<fixed<<"("<<tmpCacKPL[0]<<", "<<tmpCacKPL[1]<<")"<<" ";
		cout<<endl;
		file<<endl;
		return tmpCacKPL;
	}
	else{
		cout<<"Khong ton tai khoang phan ly nghiem de chuan hoa"<<endl;
		file<<"Khong ton tai khoang phan ly nghiem de chuan hoa"<<endl;
		return cacKPL;
	}
}

vector<double> mienChuaNghiemChuanHoa1(){ //La ham mienChuaNghiemChuanHoa nhung khong co cac cau lenh in ra man hinh va viet vao file
	vector<double> cacKPL, tmpCacKPL;
	double tmp[2];
	cacKPL = mienChuaNghiem1();
	if(cacKPL.size() != 0){
		chuanHoa(cacKPL[0], cacKPL[1], tmp);
		tmpCacKPL.push_back(tmp[0]);
		tmpCacKPL.push_back(tmp[1]);
		return tmpCacKPL;
	}
	else return cacKPL;
}

double mienTQ_ddf(){ //Ham mienTQ nhung ap dung cho ddf
	if(length - 2 == 0) return 0;
	double tmp = fabs(heSo[1]);
	int n = length - 2;
	for(int i = 2; i < n; i++)
		if(fabs(heSo_ddf[i]) > tmp)
			tmp = fabs(heSo_ddf[i]);
	return 1 + tmp/fabs(heSo_ddf[0]);
}

vector<double> mienChuaNghiem_ddf(){ //Ham mienChuaNghiem1 nhung ap dung cho ddf
	vector<double> cacKPL;
	double b, a, i;
	b = mienTQ_ddf();
	a = -b;
	i = a;
	while(i + 1/N < b){
		if(ddf(i) * ddf(i + 1/N) > 0)
			i = i + 1/N;
		else if(ddf(i) * ddf(i + 1/N) < 0){
			cacKPL.push_back(i);
			cacKPL.push_back(i + 1/N);
			i = i + 1/N;
		}
		else{
			cacKPL.push_back(i+1/N - delta);
			cacKPL.push_back(i+1/N + delta);
			if(i + 2/N < b) i = i + 2/N;
			else i = i + 1/N + delta;
		}
	}
	return cacKPL;
}

vector<double> chiaDoi(){ //Tra ve cuc tri cua df (hay nghiem cua ddf) bang phuong phap chia doi
	vector<double> cucTri_df, cacKPL;
	cacKPL = mienChuaNghiem_ddf();
	double c, a, b;
	if(cacKPL.size() == 0) return cacKPL;
	for(int i = 0; i <= cacKPL.size() - 2; i += 2){
		a = cacKPL[i];
		b = cacKPL[i+1];
		for(int j = 0; j < 100; j++){
			c = (a + b) / 2;
        	if(ddf(a) * ddf(c) < 0)
            	b = c;
        	else if(ddf(a) * ddf(c) > 0)
            	a = c;
        	else{
            	a = b = c;
            	break;
        	}
		}
		c = (a + b) / 2;
		cucTri_df.push_back(c);
	}
	return cucTri_df;
}

double max_df(vector<double> cucTri, double a, double b){ //Tra ve gia tri lon nhat cua df tren khoang (a, b)
	cucTri.push_back(a);
	cucTri.push_back(b);
	int l = cucTri.size();
	double max = df(a);
	for(int i = 0; i < l; i++){
		if(cucTri[i] >= a && cucTri[i] <= b){
			if(df(cucTri[i]) > max) max = df(cucTri[i]);
		}
	}
	return max;
}

double min_df(vector<double> cucTri, double a, double b){ //Tra ve gia tri nho nhat cua df tren khoang (a, b)
	cucTri.push_back(a);
	cucTri.push_back(b);
	int l = cucTri.size();
	double min = df(a);
	for(int i = 0; i < l; i++){
		if(cucTri[i] >= a && cucTri[i] <= b){
			if(df(cucTri[i]) < min) min = df(cucTri[i]);
		}
	}
	return min;
}

double danhgiasaiso1(double x, double y, double min, double max){ //Danh gia sai so cua phuong phap day cung - cach 1
	if(max == 0 || min == 0){
		return -999;
	}
	if(max < 0){
		double tmp = fabs(max);
		max = fabs(min);
		min = tmp;		
	}
	else if(max > 0 && min < 0){
		max = fabs(max);
		min = fabs(min);
		if(min > max){
			double tmp = max;
			max = min;
			min = tmp;
		}
	}
    return ((max - min)/min)*fabs(x-y);
}

double danhgiasaiso2(double x, double min, double max){ //Danh gia sai so cua phuong phap day cung - cach 2
	if(max == 0 || min == 0){
		return -999;
	}
	if(max < 0){
		double tmp = fabs(max);
		max = fabs(min);
		min = tmp;		
	}
	else if(max > 0 && min < 0){
		max = fabs(max);
		min = fabs(min);
		if(min > max){
			double tmp = max;
			max = min;
			min = tmp;
		}
	}
	return fabs(f(x)) / min;
}

bool checkDayCung_df(vector<double> kpl){ //Kiem tra xem df co doi dau tren khoang phan ly khong
	float buocNhay = 20;
	double a = kpl[0], b = kpl[1];
	double i = a;
	double try_var = df(a)*df(i + 1/buocNhay);
	while(try_var > 0 && i <= b){
		i += 1/buocNhay;
		try_var = try_var * df(a) * df(i);
		if(try_var <= 0 && i >= b) return true;
		if(try_var < 0) return false;
		if(try_var <= 0 && i < b) return false;
	}
	return true;
}

bool checkDayCung_ddf(vector<double> kpl){ //Kiem tra xem ddf co doi dau tren khoang phan ly khong
	float buocNhay = 20;
	double a = kpl[0], b = kpl[1];
	double i = a;
	double try_var = ddf(a) * ddf(i + 1/buocNhay);
	while(try_var > 0 && i <= b){
		i += 1/buocNhay;
		try_var = try_var * ddf(a) * ddf(i);
		if(try_var <= 0 && i >= b) return true;
		if(try_var < 0) return false;
		if(try_var <= 0 && i < b) return false;
	}
	return true;
}

double chucNang3(){ //Tim nghiem bang phuong phap day cung voi so lan lap da biet
	file<<"       Thuc hien chuc nang 5:"<<endl; 
	if(c == 2){
		if(length == -1){
			nhapHeSo_f();
			system("cls");
		}
		if(s == -1){
			nhapSoThapPhan();
			system("cls");
		}
		vector<double> cucTri = chiaDoi();
		vector<double> kpl = mienChuaNghiemChuanHoa1();
		if(kpl.size() != 0){
			if(checkDayCung_df(kpl) && checkDayCung_ddf(kpl)){
				double max, min, d;
				max = max_df(cucTri, kpl[0], kpl[1]);
				min = min_df(cucTri, kpl[0], kpl[1]);
				int lanLap;
				cout<<"Nhap so lan lap: ";
			    cin>>lanLap;
			    file<<"Nhap so lan lap: "<<lanLap<<endl;
			    double x[lanLap+1];
			    
			    //Chon diem fourier
			    if(f(kpl[0]) * ddf(kpl[0]) < 0){
			    	x[0] = kpl[0];
			    	d = kpl[1];
			    }
			    else{
			    	x[0] = kpl[1];
			    	d = kpl[0];
			    }
			    
			    //Lap
			    for(int i = 1; i <= lanLap; i++){
			        x[i] = x[i-1] - ((d - x[i-1]) / (f(d) - f(x[i-1]))) * f(x[i-1]);
			    }
			    if(danhgiasaiso1(x[lanLap], x[lanLap-1], min, max) == -999){
			    	cout<<"Do min|f'(x)| = 0 nen khong the danh gia sai so theo ca 2 phuong phap => Khong the tiep tuc thuc hien"<<endl;
    				file<<"Do min|f'(x)| = 0 nen khong the danh gia sai so theo ca 2 phuong phap => Khong the tiep tuc thuc hien"<<endl;
			    }
			    else{
				    //In 
				    cout<<"x (da lam tron) = "<<setprecision(s)<<fixed<<x[lanLap]<<endl;
				    cout<<"Sai so theo cach 1: "<<setprecision(s)<<fixed<<danhgiasaiso1(x[lanLap], x[lanLap-1], min, max)<<endl; 
				    cout<<"Sai so theo cach 2: "<<setprecision(s)<<fixed<<danhgiasaiso2(x[lanLap], min, max)<<endl;
				    file<<"x (da lam tron) = "<<setprecision(s)<<fixed<<x[lanLap]<<endl;
				    file<<"Sai so theo cach 1: "<<setprecision(s)<<fixed<<danhgiasaiso1(x[lanLap], x[lanLap-1], min, max)<<endl;
				    file<<"Sai so theo cach 2: "<<setprecision(s)<<fixed<<danhgiasaiso2(x[lanLap], min, max)<<endl;
				    return x[lanLap];
			    }
			}
			else{
				cout<<"Khong thoa man dieu kien ap dung phuong phap day cung"<<endl;
				file<<"Khong thoa man dieu kien ap dung phuong phap day cung"<<endl;
			}
		}
		else{
			cout<<"Khong the tim thay khoang phan ly"<<endl;
			file<<"Khong the tim thay khoang phan ly"<<endl;
		}
	}
	else{
		if(length == -1){
			nhapHeSo_f();
			system("cls");
		}
		if(s == -1){
			nhapSoThapPhan();
			system("cls");
		}
		double a, b;
		cout<<"Nhap a = "; cin>>a;
		file<<"Nhap a = "<<a<<endl;
		cout<<"Nhap b = "; cin>>b;
		file<<"Nhap b = "<<b<<endl;
		vector<double> cucTri = chiaDoi();
		vector<double> kpl;
		kpl.push_back(a);
		kpl.push_back(b);
		if(f(a) * f(b) < 0 ){
			if(checkDayCung_df(kpl) && checkDayCung_ddf(kpl)){
				double max, min, d;
				max = max_df(cucTri, kpl[0], kpl[1]);
				min = min_df(cucTri, kpl[0], kpl[1]);
				int lanLap;
				cout<<"Nhap so lan lap: ";
			    cin>>lanLap;
			    file<<"Nhap so lan lap: "<<lanLap<<endl;
			    double x[lanLap+1];
			    
			    //Chon diem fourier
			    if(f(kpl[0]) * ddf(kpl[0]) < 0){
			    	x[0] = kpl[0];
			    	d = kpl[1];
			    }
			    else{
			    	x[0] = kpl[1];
			    	d = kpl[0];
			    }
			    
			    //Lap
			    for(int i = 1; i <= lanLap; i++){
			        x[i] = x[i-1] - ((d - x[i-1]) / (f(d)-f(x[i-1]))) * f(x[i-1]);
			    }
			    if(danhgiasaiso1(x[lanLap], x[lanLap-1], min, max) == -999){
			    	cout<<"Do min|f'(x)| = 0 nen khong the danh gia sai so theo ca 2 phuong phap => Khong the tiep tuc thuc hien"<<endl;
    				file<<"Do min|f'(x)| = 0 nen khong the danh gia sai so theo ca 2 phuong phap => Khong the tiep tuc thuc hien"<<endl;
			    }
			    else{
				    //In 
				    cout<<"x (da lam tron) = "<<setprecision(s)<<fixed<<x[lanLap]<<endl;
				    cout<<"Sai so theo cach 1: "<<setprecision(s)<<fixed<<danhgiasaiso1(x[lanLap], x[lanLap-1], min, max)<<endl; 
				    cout<<"Sai so theo cach 2: "<<setprecision(s)<<fixed<<danhgiasaiso2(x[lanLap], min, max)<<endl;
				    file<<"x (da lam tron) = "<<setprecision(s)<<fixed<<x[lanLap]<<endl;
				    file<<"Sai so theo cach 1: "<<setprecision(s)<<fixed<<danhgiasaiso1(x[lanLap], x[lanLap-1], min, max)<<endl;
				    file<<"Sai so theo cach 2: "<<setprecision(s)<<fixed<<danhgiasaiso2(x[lanLap], min, max)<<endl;
				    return x[lanLap];
				}
			}
			else{
				cout<<"Khong thoa man dieu kien ap dung phuong phap day cung"<<endl;
				file<<"Khong thoa man dieu kien ap dung phuong phap day cung"<<endl;
			}
		}
		else{
			cout<<"(a, b) khong phai khoang phan ly cua f(x) => Khong the tien hanh phuong phap day cung"<<endl;
			file<<"(a, b) khong phai khoang phan ly cua f(x) => Khong the tien hanh phuong phap day cung"<<endl;
		}
	}
}

double chucNang4(){ //Tim nghiem bang phuong phap day cung voi sai so da biet
	file<<"       Thuc hien chuc nang 6:"<<endl;
	if(c == 2){
		if(length == -1){
			nhapHeSo_f();
			system("cls");
		}
		if(s == -1){
			nhapSoThapPhan();
			system("cls");
		}
		vector<double> cucTri = chiaDoi(); 
		vector<double> kpl = mienChuaNghiemChuanHoa1();
		if(kpl.size() != 0){
			if(checkDayCung_df(kpl) && checkDayCung_ddf(kpl)){
				double max, min, d, eps, x[100];
				max = max_df(cucTri, kpl[0], kpl[1]);
				min = min_df(cucTri, kpl[0], kpl[1]);
			    
			    //Chon diem fourier
			    if(f(kpl[0]) * ddf(kpl[0]) < 0){
			    	x[0] = kpl[0];
			    	d = kpl[1];
			    }
			    else{
			    	x[0] = kpl[1];
			    	d = kpl[0];
			    }
			    
				//Nhap eps
			    cout<<"Nhap eps = "; cin>>eps;
			    file<<"Nhap eps = "<<eps<<endl;
			    
			    //Lap theo cong thuc sai so 1
			    int i = 1;
    			x[i] = x[i-1] - ((d-x[i-1]) / (f(d) - f(x[i-1]))) * f(x[i-1]);
    			if(danhgiasaiso1(x[i], x[i-1], min, max) == -999){
    				cout<<"Do min|f'(x)| = 0 nen khong the danh gia sai so theo phuong phap 1 => Khong the tiep tuc thuc hien"<<endl;
    				file<<"Do min|f'(x)| = 0 nen khong the danh gia sai so theo phuong phap 1 => Khong the tiep tuc thuc hien"<<endl;
    			}
    			else{
				    while(danhgiasaiso1(x[i], x[i-1], min, max) > eps){
				    	i++;
				        x[i] = x[i-1] - ((d-x[i-1]) / (f(d) - f(x[i-1]))) * f(x[i-1]);
				    }
				    
				    //In ket qua theo cong thuc sai so 1
				    cout<<"Voi sai so cho truoc va ap dung cong thuc sai so 1: x (da lam tron) = "<<setprecision(s)<<fixed<<x[i]<<endl;
				    file<<"Voi sai so cho truoc va ap dung cong thuc sai so 1: x (da lam tron) = "<<setprecision(s)<<fixed<<x[i]<<endl;
			    }
			    
			    //Lap theo cong thuc sai so 2
				i = 1;
    			x[i] = x[i-1] - ((d - x[i-1]) / (f(d) - f(x[i-1]))) * f(x[i-1]);
    			if(danhgiasaiso2(x[i], min, max) == -999){
    				cout<<"Do min|f'(x)| = 0 nen khong the danh gia sai so theo phuong phap 2 => Khong the tiep tuc thuc hien"<<endl;
    				file<<"Do min|f'(x)| = 0 nen khong the danh gia sai so theo phuong phap 2 => Khong the tiep tuc thuc hien"<<endl;
    			}
    			else{
				    while(danhgiasaiso2(x[i], min, max) > eps){
				    	i++;
				        x[i] = x[i-1] - ((d - x[i-1]) / (f(d) - f(x[i-1]))) * f(x[i-1]);
				    }
				    
				    //In ket qua theo cong thuc sai so 2
				    cout<<"Voi sai so cho truoc va ap dung cong thuc sai so 2: x (da lam tron) = "<<setprecision(s)<<fixed<<x[i]<<endl;
				    file<<"Voi sai so cho truoc va ap dung cong thuc sai so 2: x (da lam tron) = "<<setprecision(s)<<fixed<<x[i]<<endl;
			    }
			}
			else{
				cout<<"Khong thoa man dieu kien ap dung phuong phap day cung"<<endl;
				file<<"Khong thoa man dieu kien ap dung phuong phap day cung"<<endl;
			}
		}
		else{
			cout<<"Khong the tim thay khoang phan ly"<<endl;
			file<<"Khong the tim thay khoang phan ly"<<endl;
		}
	}
	else{
		if(length == -1){
			nhapHeSo_f();
			system("cls");
		}
		if(s == -1){
			nhapSoThapPhan();
			system("cls");
		}
		double a, b;
		cout<<"Nhap a = "; cin>>a;
		file<<"Nhap a = "<<a<<endl;
		cout<<"Nhap b = "; cin>>b;
		file<<"Nhap b = "<<b<<endl;
		vector<double> cucTri = chiaDoi();
		vector<double> kpl;
		kpl.push_back(a);
		kpl.push_back(b);
		if(f(a) * f(b) < 0){
			if(checkDayCung_df(kpl) && checkDayCung_ddf(kpl)){
				double max, min, d, eps, x[100];
				max = max_df(cucTri, kpl[0], kpl[1]);
				min = min_df(cucTri, kpl[0], kpl[1]);
			    
			    //Chon diem fourier
			    if(f(kpl[0]) * ddf(kpl[0]) < 0){
			    	x[0] = kpl[0];
			    	d = kpl[1];
			    }
			    else{
			    	x[0] = kpl[1];
			    	d = kpl[0];
			    }
			    
			    //Nhap eps
			    cout<<"Nhap eps = "; cin>>eps;
			    file<<"Nhap eps = "<<eps<<endl;
			    
			    //Lap theo cong thuc sai so 1
			    int i = 1;
    			x[i] = x[i-1] - ((d-x[i-1]) / (f(d) - f(x[i-1]))) * f(x[i-1]);
    			if(danhgiasaiso1(x[i], x[i-1], min, max) == -999){
    				cout<<"Do min|f'(x)| = 0 nen khong the danh gia sai so theo phuong phap 1 => Khong the tiep tuc thuc hien"<<endl;
    				file<<"Do min|f'(x)| = 0 nen khong the danh gia sai so theo phuong phap 1 => Khong the tiep tuc thuc hien"<<endl;
				}
    			else{
				    while(danhgiasaiso1(x[i], x[i-1], min, max) > eps){
				    	i++;
				        x[i] = x[i-1] - ((d-x[i-1]) / (f(d) - f(x[i-1]))) * f(x[i-1]);
				    }
				    
				    //In ket qua theo cong thuc sai so 1
					cout<<"Voi sai so cho truoc va ap dung cong thuc sai so 1: x (da lam tron) = "<<setprecision(s)<<fixed<<x[i]<<endl;
				    file<<"Voi sai so cho truoc va ap dung cong thuc sai so 1: x (da lam tron) = "<<setprecision(s)<<fixed<<x[i]<<endl;
			    }
			    
			    //Lap theo cong thuc sai so 2
			    i = 1;
    			x[i] = x[i-1] - ((d - x[i-1]) / (f(d) - f(x[i-1]))) * f(x[i-1]);
			    if(danhgiasaiso2(x[i], min, max) == -999){
			    	cout<<"Do min|f'(x)| = 0 nen khong the danh gia sai so theo phuong phap 2 => Khong the tiep tuc thuc hien"<<endl;
			    	file<<"Do min|f'(x)| = 0 nen khong the danh gia sai so theo phuong phap 2 => Khong the tiep tuc thuc hien"<<endl;
				}
			    else{
					while(danhgiasaiso2(x[i], min, max) > eps){
				    	i++;
				        x[i] = x[i-1] - ((d - x[i-1]) / (f(d) - f(x[i-1]))) * f(x[i-1]);
				    }
				    
				    //In ket qua theo cong thuc sai so 2
				    cout<<"Voi sai so cho truoc va ap dung cong thuc sai so 2: x (da lam tron) = "<<setprecision(s)<<fixed<<x[i]<<endl;
				    file<<"Voi sai so cho truoc va ap dung cong thuc sai so 2: x (da lam tron) = "<<setprecision(s)<<fixed<<x[i]<<endl;
				}
			}
			else{
				cout<<"Khong thoa man dieu kien ap dung phuong phap day cung"<<endl;
				file<<"Khong thoa man dieu kien ap dung phuong phap day cung"<<endl;
			}
		}
		else{
			cout<<"(a, b) khong phai khoang phan ly cua f(x) => Khong the tien hanh phuong phap day cung"<<endl;
			file<<"(a, b) khong phai khoang phan ly cua f(x) => Khong the tien hanh phuong phap day cung"<<endl;
		}
	}
}

double chucNang5(){ //Tim nghiem bang phuong phap day cung thoa man yeu cau de bai
	file<<"       Thuc hien chuc nang 7:"<<endl;
	if(c == 2){
		if(length == -1){
			nhapHeSo_f();
			system("cls");
		}
		if(s == -1){
			nhapSoThapPhan();
			system("cls");
		}
		vector<double> cucTri = chiaDoi(); 
		vector<double> kpl = mienChuaNghiemChuanHoa1();
		if(kpl.size() != 0){
			if(checkDayCung_df(kpl) && checkDayCung_ddf(kpl)){
				double max, min, d, eps, x[100];
				max = max_df(cucTri, kpl[0], kpl[1]);
				min = min_df(cucTri, kpl[0], kpl[1]);
			    
			    //Chon diem fourier
			    if(f(kpl[0]) * ddf(kpl[0]) < 0){
			    	x[0] = kpl[0];
			    	d = kpl[1];
			    }
			    else{
			    	x[0] = kpl[1];
			    	d = kpl[0];
			    }
			    
				//Nhap eps
			    cout<<"Nhap e = "; cin>>eps;
			    file<<"Nhap e = "<<eps<<endl;
			    
			    //Lap
			    int i = 1;
    			x[i] = x[i-1] - ((d - x[i-1]) / (f(d) - f(x[i-1]))) * f(x[i-1]);
			    while(fabs(x[i] - x[i-1]) > eps){
			    	i++;
			        x[i] = x[i-1] - ((d - x[i-1]) / (f(d) - f(x[i-1]))) * f(x[i-1]);
			    }
			    
			    //In
			    cout<<"Voi e nhap tu ban phim: x (da lam tron) = "<<setprecision(s)<<fixed<<x[i]<<endl;
			    file<<"Voi e nhap tu ban phim: x (da lam tron) = "<<setprecision(s)<<fixed<<x[i]<<endl;
			}
			else{
				cout<<"Khong thoa man dieu kien ap dung phuong phap day cung"<<endl;
				file<<"Khong thoa man dieu kien ap dung phuong phap day cung"<<endl;
			}
		}
		else{
			cout<<"Khong the tim thay khoang phan ly"<<endl;
			file<<"Khong the tim thay khoang phan ly"<<endl;
		}
	}
	else{
		if(length == -1){
			nhapHeSo_f();
			system("cls");
		}
		if(s == -1){
			nhapSoThapPhan();
			system("cls");
		}
		double a, b;
		cout<<"Nhap a = "; cin>>a;
		file<<"Nhap a = "<<a<<endl;
		cout<<"Nhap b = "; cin>>b;
		file<<"Nhap b = "<<b<<endl;
		vector<double> cucTri = chiaDoi();
		vector<double> kpl;
		kpl.push_back(a);
		kpl.push_back(b);
		if(f(a)*f(b) < 0){
			if(checkDayCung_df(kpl) && checkDayCung_ddf(kpl)){
				double max, min, d, eps, x[100];
				max = max_df(cucTri, kpl[0], kpl[1]);
				min = min_df(cucTri, kpl[0], kpl[1]);
			    
			    //Chon diem fourier
			    if(f(kpl[0]) * ddf(kpl[0]) < 0){
			    	x[0] = kpl[0];
			    	d = kpl[1];
			    }
			    else{
			    	x[0] = kpl[1];
			    	d = kpl[0];
			    }
			    
			    //Nhap eps
			    cout<<"Nhap e = "; cin>>eps;
			    file<<"Nhap e = "<<eps<<endl;
			    
			    //Lap
			    int i = 1;
    			x[i] = x[i-1] - ((d - x[i-1]) / (f(d) - f(x[i-1]))) * f(x[i-1]);
			    while(fabs(x[i] - x[i-1]) > eps){
			    	i++;
			        x[i] = x[i-1] - ((d - x[i-1]) / (f(d) - f(x[i-1]))) * f(x[i-1]);
			    }
			    
			    //In
			    cout<<"Voi e nhap tu ban phim: x (da lam tron) = "<<setprecision(s)<<fixed<<x[i]<<endl;
			    file<<"Voi e nhap tu ban phim: x (da lam tron) = "<<setprecision(s)<<fixed<<x[i]<<endl;
			}
			else{
				cout<<"Khong thoa man dieu kien ap dung phuong phap day cung"<<endl;
				file<<"Khong thoa man dieu kien ap dung phuong phap day cung"<<endl;
			}
		}
		else{
			cout<<"(a, b) khong phai khoang phan ly cua f(x) => Khong the tien hanh phuong phap day cung"<<endl;
			file<<"(a, b) khong phai khoang phan ly cua f(x) => Khong the tien hanh phuong phap day cung"<<endl;
		}
	}
}


void menu(){ //Ve menu 
	SetConsoleTextAttribute(hc, 0x7);
	cout<<"                               --------MENU--------"<<endl;
	cout<<"_____________________________________________________________________________________"<<endl;
	if(count%M == 1){
		cout<<"|";
		SetConsoleTextAttribute(hc, 0xd);
		cout<<"1. Nhap da thuc f(x)                                                               ";
		SetConsoleTextAttribute(hc, 0x7);
		cout<<"|"<<endl;
		cout<<"|2. Nhap so chu so thap phan can lam tron                                           |"<<endl;
		cout<<"|3. Tim cac khoang phan ly nghiem                                                   |"<<endl;
		cout<<"|4. Tim khoang phan ly nghiem (a, b) thoa man |a-b| <= 0.5                          |"<<endl;
		cout<<"|5. Tim nghiem gan dung voi so lan lap cho truoc                                    |"<<endl;
		cout<<"|6. Tim nghiem gan dung voi sai so e cho truoc                                      |"<<endl;
		cout<<"|7. Tim nghiem gan dung Xn trong khoang (a, b) thoa man |Xn - Xn-1| <= e (cho truoc)|"<<endl;
	}
	else if(count%M == 2){
		SetConsoleTextAttribute(hc, 0x7);
		cout<<"|1. Nhap da thuc f(x)                                                               |"<<endl;
		cout<<"|";
		SetConsoleTextAttribute(hc, 0xd);
		cout<<"2. Nhap so chu so thap phan can lam tron                                           ";
		SetConsoleTextAttribute(hc, 0x7);
		cout<<"|"<<endl;
		cout<<"|3. Tim cac khoang phan ly nghiem                                                   |"<<endl;
		cout<<"|4. Tim khoang phan ly nghiem (a, b) thoa man |a-b| <= 0.5                          |"<<endl;
		cout<<"|5. Tim nghiem gan dung voi so lan lap cho truoc                                    |"<<endl;
		cout<<"|6. Tim nghiem gan dung voi sai so e cho truoc                                      |"<<endl;
		cout<<"|7. Tim nghiem gan dung Xn trong khoang (a, b) thoa man |Xn - Xn-1| <= e (cho truoc)|"<<endl;
	}
	else if(count%M == 3){
		SetConsoleTextAttribute(hc, 0x7);
		cout<<"|1. Nhap da thuc f(x)                                                               |"<<endl;
		cout<<"|2. Nhap so chu so thap phan can lam tron                                           |"<<endl;
		cout<<"|";
		SetConsoleTextAttribute(hc, 0xd);
		cout<<"3. Tim cac khoang phan ly nghiem                                                   ";
		SetConsoleTextAttribute(hc, 0x7);
		cout<<"|"<<endl;
		cout<<"|4. Tim khoang phan ly nghiem (a, b) thoa man |a-b| <= 0.5                          |"<<endl;
		cout<<"|5. Tim nghiem gan dung voi so lan lap cho truoc                                    |"<<endl;
		cout<<"|6. Tim nghiem gan dung voi sai so e cho truoc                                      |"<<endl;
		cout<<"|7. Tim nghiem gan dung Xn trong khoang (a, b) thoa man |Xn - Xn-1| <= e (cho truoc)|"<<endl;
	}
	else if(count%M == 4){
		SetConsoleTextAttribute(hc, 0x7);
		cout<<"|1. Nhap da thuc f(x)                                                               |"<<endl;
		cout<<"|2. Nhap so chu so thap phan can lam tron                                           |"<<endl;
		cout<<"|3. Tim cac khoang phan ly nghiem                                                   |"<<endl;
		cout<<"|";
		SetConsoleTextAttribute(hc, 0xd);
		cout<<"4. Tim khoang phan ly nghiem (a, b) thoa man |a-b| <= 0.5                          ";
		SetConsoleTextAttribute(hc, 0x7);
		cout<<"|"<<endl;
		cout<<"|5. Tim nghiem gan dung voi so lan lap cho truoc                                    |"<<endl;
		cout<<"|6. Tim nghiem gan dung voi sai so e cho truoc                                      |"<<endl;
		cout<<"|7. Tim nghiem gan dung Xn trong khoang (a, b) thoa man |Xn - Xn-1| <= e (cho truoc)|"<<endl;
	}
	else if(count%M == 5){
		SetConsoleTextAttribute(hc, 0x7);
		cout<<"|1. Nhap da thuc f(x)                                                               |"<<endl;
		cout<<"|2. Nhap so chu so thap phan can lam tron                                           |"<<endl;
		cout<<"|3. Tim cac khoang phan ly nghiem                                                   |"<<endl;
		cout<<"|4. Tim khoang phan ly nghiem (a, b) thoa man |a-b| <= 0.5                          |"<<endl;
		cout<<"|";
		SetConsoleTextAttribute(hc, 0xd);
		cout<<"5. Tim nghiem gan dung voi so lan lap cho truoc                                    ";
		SetConsoleTextAttribute(hc, 0x7);
		cout<<"|"<<endl;
		cout<<"|6. Tim nghiem gan dung voi sai so e cho truoc                                      |"<<endl;
		cout<<"|7. Tim nghiem gan dung Xn trong khoang (a, b) thoa man |Xn - Xn-1| <= e (cho truoc)|"<<endl;	
	}
	else if(count%M == 6){
		SetConsoleTextAttribute(hc, 0x7);
		cout<<"|1. Nhap da thuc f(x)                                                               |"<<endl;
		cout<<"|2. Nhap so chu so thap phan can lam tron                                           |"<<endl;
		cout<<"|3. Tim cac khoang phan ly nghiem                                                   |"<<endl;
		cout<<"|4. Tim khoang phan ly nghiem (a, b) thoa man |a-b| <= 0.5                          |"<<endl;
		cout<<"|5. Tim nghiem gan dung voi so lan lap cho truoc                                    |"<<endl;
		cout<<"|";
		SetConsoleTextAttribute(hc, 0xd);
		cout<<"6. Tim nghiem gan dung voi sai so e cho truoc                                      ";
		SetConsoleTextAttribute(hc, 0x7);
		cout<<"|"<<endl;
		cout<<"|7. Tim nghiem gan dung Xn trong khoang (a, b) thoa man |Xn - Xn-1| <= e (cho truoc)|"<<endl;	
	}
	else if(count%M == 0){
		SetConsoleTextAttribute(hc, 0x7);
		cout<<"|1. Nhap da thuc f(x)                                                               |"<<endl;
		cout<<"|2. Nhap so chu so thap phan can lam tron                                           |"<<endl;
		cout<<"|3. Tim cac khoang phan ly nghiem                                                   |"<<endl;
		cout<<"|4. Tim khoang phan ly nghiem (a, b) thoa man |a-b| <= 0.5                          |"<<endl;
		cout<<"|5. Tim nghiem gan dung voi so lan lap cho truoc                                    |"<<endl;
		cout<<"|6. Tim nghiem gan dung voi sai so e cho truoc                                      |"<<endl;
		cout<<"|";
		SetConsoleTextAttribute(hc, 0xd);
		cout<<"7. Tim nghiem gan dung Xn trong khoang (a, b) thoa man |Xn - Xn-1| <= e (cho truoc)";
		SetConsoleTextAttribute(hc, 0x7);
		cout<<"|"<<endl;		
	}
	SetConsoleTextAttribute(hc, 0x7);
	cout<<"|___________________________________________________________________________________|"<<endl;
}	

void phanTangCN3(int tmp){ //Ve menu phan tang cua CN3
	if(tmp%2 == 1){
		c = 1;
		SetConsoleTextAttribute(hc, 0x7);
		cout<<"                               --------MENU--------"<<endl;
		cout<<"_____________________________________________________________________________________"<<endl;
		SetConsoleTextAttribute(hc, 0x7);
		cout<<"|1. Nhap da thuc f(x)                                                               |"<<endl;
		cout<<"|2. Nhap so chu so thap phan can lam tron                                           |"<<endl;
		cout<<"|3. Tim cac khoang phan ly nghiem                                                   |"<<endl;
		cout<<"|4. Tim khoang phan ly nghiem (a, b) thoa man |a-b| <= 0.5                          |"<<endl;
		cout<<"|5. Tim nghiem gan dung voi so lan lap cho truoc                                    |"<<endl;
		cout<<"|";
		SetConsoleTextAttribute(hc, 0xd);
		cout<<"     -> Nhap vao khoang (a, b)                                                     ";
		SetConsoleTextAttribute(hc, 0x7);
		cout<<"|"<<endl;
		cout<<"|        Lay khoang (a, b) tu chuc nang 4                                           |"<<endl;
		cout<<"|6. Tim nghiem gan dung voi sai so e cho truoc                                      |"<<endl;
		cout<<"|7. Tim nghiem gan dung Xn trong khoang (a, b) thoa man |Xn - Xn-1| <= e (cho truoc)|"<<endl;	
	}
	else{
		c = 2;
		SetConsoleTextAttribute(hc, 0x7);
		cout<<"                               --------MENU--------"<<endl;
		cout<<"_____________________________________________________________________________________"<<endl;
		SetConsoleTextAttribute(hc, 0x7);
		cout<<"|1. Nhap da thuc f(x)                                                               |"<<endl;
		cout<<"|2. Nhap so chu so thap phan can lam tron                                           |"<<endl;
		cout<<"|3. Tim cac khoang phan ly nghiem                                                   |"<<endl;
		cout<<"|4. Tim khoang phan ly nghiem (a, b) thoa man |a-b| <= 0.5                          |"<<endl;
		cout<<"|5. Tim nghiem gan dung voi so lan lap cho truoc                                    |"<<endl;
		cout<<"|        Nhap vao khoang (a, b)                                                     |"<<endl;
		cout<<"|";
		SetConsoleTextAttribute(hc, 0xd);
		cout<<"     -> Lay khoang (a, b) tu chuc nang 4                                           ";
		SetConsoleTextAttribute(hc, 0x7);
		cout<<"|"<<endl;
		cout<<"|6. Tim nghiem gan dung voi sai so e cho truoc                                      |"<<endl;
		cout<<"|7. Tim nghiem gan dung Xn trong khoang (a, b) thoa man |Xn - Xn-1| <= e (cho truoc)|"<<endl;	
	}
	SetConsoleTextAttribute(hc, 0x7);
	cout<<"|___________________________________________________________________________________|"<<endl;
}

int menuPhanTangCN3(){ //Kiem soat len xuong trong menu phan tang cua CN3
	int tmp = 1;
	char ch;
	system("cls");
	phanTangCN3(tmp);
	while(1){
		ch = getch();
		if(ch == ENTER){
			system("cls");
			return 0;			
		}
		if(ch == DOWN){
			tmp++;
			system("cls");
			phanTangCN3(tmp);
		}
		if(ch == UP){
			if(tmp == 1){
				tmp = 2;
				system("cls");
				phanTangCN3(tmp);
			}
			else{
				tmp--;
				system("cls");
				phanTangCN3(tmp);
			}
		}
	}
}

void phanTangCN4(int tmp){ //Ve menu phan tang cua CN4
	if(tmp%2 == 1){
		c = 1;
		SetConsoleTextAttribute(hc, 0x7);
		cout<<"                               --------MENU--------"<<endl;
		cout<<"_____________________________________________________________________________________"<<endl;
		SetConsoleTextAttribute(hc, 0x7);
		cout<<"|1. Nhap da thuc f(x)                                                               |"<<endl;
		cout<<"|2. Nhap so chu so thap phan can lam tron                                           |"<<endl;
		cout<<"|3. Tim cac khoang phan ly nghiem                                                   |"<<endl;
		cout<<"|4. Tim khoang phan ly nghiem (a, b) thoa man |a-b| <= 0.5                          |"<<endl;
		cout<<"|5. Tim nghiem gan dung voi so lan lap cho truoc                                    |"<<endl;
		cout<<"|6. Tim nghiem gan dung voi sai so e cho truoc                                      |"<<endl;
		cout<<"|";
		SetConsoleTextAttribute(hc, 0xd);
		cout<<"     -> Nhap vao khoang (a, b)                                                     ";
		SetConsoleTextAttribute(hc, 0x7);
		cout<<"|"<<endl;
		cout<<"|        Lay khoang (a, b) tu chuc nang 4                                           |"<<endl;
		cout<<"|7. Tim nghiem gan dung Xn trong khoang (a, b) thoa man |Xn - Xn-1| <= e (cho truoc)|"<<endl;	
	}
	else{
		c = 2;
		SetConsoleTextAttribute(hc, 0x7);
		cout<<"                               --------MENU--------"<<endl;
		cout<<"_____________________________________________________________________________________"<<endl;
		SetConsoleTextAttribute(hc, 0x7);
		cout<<"|1. Nhap da thuc f(x)                                                               |"<<endl;
		cout<<"|2. Nhap so chu so thap phan can lam tron                                           |"<<endl;
		cout<<"|3. Tim cac khoang phan ly nghiem                                                   |"<<endl;
		cout<<"|4. Tim khoang phan ly nghiem (a, b) thoa man |a-b| <= 0.5                          |"<<endl;
		cout<<"|5. Tim nghiem gan dung voi so lan lap cho truoc                                    |"<<endl;
		cout<<"|6. Tim nghiem gan dung voi sai so e cho truoc                                      |"<<endl;
		cout<<"|        Nhap vao khoang (a, b)                                                     |"<<endl;
		cout<<"|";
		SetConsoleTextAttribute(hc, 0xd);
		cout<<"     -> Lay khoang (a, b) tu chuc nang 4                                           ";
		SetConsoleTextAttribute(hc, 0x7);
		cout<<"|"<<endl;
		cout<<"|7. Tim nghiem gan dung Xn trong khoang (a, b) thoa man |Xn - Xn-1| <= e (cho truoc)|"<<endl;	
	}
	SetConsoleTextAttribute(hc, 0x7);
	cout<<"|___________________________________________________________________________________|"<<endl;
}

int menuPhanTangCN4(){ //Kiem soat len xuong trong menu phan tang cua CN4
	int tmp = 1;
	char ch;
	system("cls");
	phanTangCN4(tmp);
	while(1){
		ch = getch();
		if(ch == ENTER){
			system("cls");
			return 0;			
		}
		if(ch == DOWN){
			tmp++;
			system("cls");
			phanTangCN4(tmp);
		}
		if(ch == UP){
			if(tmp == 1){
				tmp = 2;
				system("cls");
				phanTangCN4(tmp);
			}
			else{
				tmp--;
				system("cls");
				phanTangCN4(tmp);
			}
		}
	}
}

void phanTangCN5(int tmp){ //Ve menu phan tang cua CN5
	if(tmp%2 == 1){
		c = 1;
		SetConsoleTextAttribute(hc, 0x7);
		cout<<"                               --------MENU--------"<<endl;
		cout<<"_____________________________________________________________________________________"<<endl;
		SetConsoleTextAttribute(hc, 0x7);
		cout<<"|1. Nhap da thuc f(x)                                                               |"<<endl;
		cout<<"|2. Nhap so chu so thap phan can lam tron                                           |"<<endl;
		cout<<"|3. Tim cac khoang phan ly nghiem                                                   |"<<endl;
		cout<<"|4. Tim khoang phan ly nghiem (a, b) thoa man |a-b| <= 0.5                          |"<<endl;
		cout<<"|5. Tim nghiem gan dung voi so lan lap cho truoc                                    |"<<endl;
		cout<<"|6. Tim nghiem gan dung voi sai so e cho truoc                                      |"<<endl;
		cout<<"|7. Tim nghiem gan dung Xn trong khoang (a, b) thoa man |Xn - Xn-1| <= e (cho truoc)|"<<endl;
		cout<<"|";
		SetConsoleTextAttribute(hc, 0xd);
		cout<<"     -> Nhap vao khoang (a, b)                                                     ";
		SetConsoleTextAttribute(hc, 0x7);
		cout<<"|"<<endl;
		cout<<"|        Lay khoang (a, b) tu chuc nang 4                                           |"<<endl;	
	}
	else{
		c = 2;
		SetConsoleTextAttribute(hc, 0x7);
		cout<<"                               --------MENU--------"<<endl;
		cout<<"_____________________________________________________________________________________"<<endl;
		SetConsoleTextAttribute(hc, 0x7);
		cout<<"|1. Nhap da thuc f(x)                                                               |"<<endl;
		cout<<"|2. Nhap so chu so thap phan can lam tron                                           |"<<endl;
		cout<<"|3. Tim cac khoang phan ly nghiem                                                   |"<<endl;
		cout<<"|4. Tim khoang phan ly nghiem (a, b) thoa man |a-b| <= 0.5                          |"<<endl;
		cout<<"|5. Tim nghiem gan dung voi so lan lap cho truoc                                    |"<<endl;
		cout<<"|6. Tim nghiem gan dung voi sai so e cho truoc                                      |"<<endl;
		cout<<"|7. Tim nghiem gan dung Xn trong khoang (a, b) thoa man |Xn - Xn-1| <= e (cho truoc)|"<<endl;
		cout<<"|        Nhap vao khoang (a, b)                                                     |"<<endl;
		cout<<"|";
		SetConsoleTextAttribute(hc, 0xd);
		cout<<"     -> Lay khoang (a, b) tu chuc nang 4                                           ";
		SetConsoleTextAttribute(hc, 0x7);
		cout<<"|"<<endl;	
	}
	SetConsoleTextAttribute(hc, 0x7);
	cout<<"|___________________________________________________________________________________|"<<endl;
}

int menuPhanTangCN5(){ //Kiem soat len xuong trong menu phan tang cua CN5
	int tmp = 1;
	char ch;
	system("cls");
	phanTangCN5(tmp);
	while(1){
		ch = getch();
		if(ch == ENTER){
			system("cls");
			return 0;			
		}
		if(ch == DOWN){
			tmp++;
			system("cls");
			phanTangCN5(tmp);
		}
		if(ch == UP){
			if(tmp == 1){
				tmp = 2;
				system("cls");
				phanTangCN5(tmp);
			}
			else{
				tmp--;
				system("cls");
				phanTangCN5(tmp);
			}
		}
	}
}

void thucThi(){ //Thuc thi cac chuc nang tren menu
	if(count%M == 1){
		system("cls");
		SetConsoleTextAttribute(hc, 0x7);
		nhapHeSo_f();
	}
	else if(count%M == 2){
		system("cls");
		SetConsoleTextAttribute(hc, 0x7);
		nhapSoThapPhan();
	}
	else if(count%M == 3){
		system("cls");
		SetConsoleTextAttribute(hc, 0x7);
		mienChuaNghiem();
	}
	else if(count%M == 4){
		system("cls");
		SetConsoleTextAttribute(hc, 0x7);
		mienChuaNghiemChuanHoa();	
	}
	else if(count%M == 5){
		system("cls");
		SetConsoleTextAttribute(hc, 0x7);
		menuPhanTangCN3();
		chucNang3();	
	}
	else if(count%M == 6){
		system("cls");
		SetConsoleTextAttribute(hc, 0x7);
		menuPhanTangCN4();
		chucNang4();	
	}
	else if(count%M == 0){
		system("cls");
		SetConsoleTextAttribute(hc, 0x7);
		menuPhanTangCN5();
		chucNang5();	
	}
}

void ghiChu(){ //Ghi chu quy tac su dung menu
	cout<<"------Quy tac su dung------\n1. Bam mui ten len xuong de di chuyen tren menu\n2. Bam Enter neu muon thuc hien chuc nang\n3. Trong qua trinh thuc hien chuc nang khong the thoat chuong trinh\n4. Thuc hien xong chuc nang, bam phim bat ky de quay lai menu\n5. Tai man hinh menu, bam Esc neu muon thoat chuong trinh\n.....Vui long bam phim bat ky de tiep tuc!";
}

int main(){
	char m;
	ghiChu();
	getch();
	//Kiem soat len xuong trong menu
	system("cls");
	menu();
	while(true){
		m = getch();
		if(m == ESC)	break;
		if(m == ENTER){
			thucThi();
			getch();
			system("cls");
			menu();
		}
		if(m == DOWN){
			count++;
			system("cls");
			menu();
		}
		if(m == UP){
			if(count == 1){
				count = M;
				system("cls");
				menu();
			}
			else{
				count--;
				system("cls");
				menu();
			}
		}
	}
	SetConsoleTextAttribute(hc, 0x7);
	cout<<endl;
	cout<<"                          -----Ket thuc chuong trinh-----";
	cout<<endl;
	file.close();
}

