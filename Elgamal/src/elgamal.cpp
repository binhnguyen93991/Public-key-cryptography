#include<iostream>
#include<numeric>
#include <ctime>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <sstream>
using namespace std;


long long int   expMod(long long int   x, long long int   puis, long long int   mod);

long long int  intpgcd(long long int a, long long int b);

bool estPremier(long long int n);

long long int  invMod(long long int  a, long long int  b);

long long int  randPremier();

void genGroupe(long long int  *p, long long int  *q, long long int  *g);

void genCle(long long int  q, long long int  g, long long int  *h, long long int  *x, long long int  p);

void encrypt(long long int  p, long long int  g, long long int  m, long long int  h,  long long int  *c1, long long int  *c2, long long int  q);

long long int  decrypt(long long int  p, long long int  x, long long int  c1, long long int  c2);
long long int convert(string s){
    stringstream cv;
    long long int kq;
    cv << s;
    cv>> kq;
    return kq;
}
/*********FONCTIONS MATHEMATIQUES*********/
// tinh luy thua x^puis mod 
long long int  expMod(long long int  x, long long int  puis, long long int  mod){
	long long int  res = 1;

	x = x % mod;

	if(!x){
		puis=0;
		res=0;
	}

	while(puis>0){
		if(puis&1)
			res = (res*x) % mod;

		puis=puis>>1;
		x = (x*x) % mod;
	}
	return res;
}

//tinh mod nghich dao
long long int  invMod(long long int  a, long long int  b){

	long long int  r1=a, r2=b, v1=0, v2=1, u1=1, u2=0;
		r1=b, r2=a;
		long long int  q, rs, vs, us;

	while(r2){
		q = (r1/r2);
	    rs = r1, us = u1, vs = v1,
	    r1 = r2, u1 = u2, v1 = v2,
	    r2 = (rs - (q*r2)), u2 = (us - (q*u2)), v2 = (vs - (q*v2));
	}
	return u1;
}

//kiem tra snt
bool estPremier(long long int  n){
	bool res=1;
	if(n <= 2)
		res = (n==2);
	else if(!(n&1))
		res = 0;
	else{
		for(int i=3; i<=sqrt(n); i+=2){
			if(n%i==0)
				res=0;
		}
	}

	return res;
}

//chon so nguyen to  q
long long int  randPremier(){
	return 953;
}


// /********************FONCTIONS ELGAMAL*************************/

// // Tinh g la phan tu sinh cua Zp
void genGroupe(long long int  *p, long long int  *q, long long int  *g){
	bool b=1;

	while(b){
		*q = randPremier();
		*p = (2*(*q) + 1);
		b = !estPremier(*p);
	}

	unsigned long long int temp;
	do{
	temp = (rand() % *q);
	}while  ((!(expMod(temp, 2, *p)!=1))&(temp!=0));

	if(expMod(temp, *q, *p)==1){
		*g=temp;
	}
	else *g=expMod(temp, 2, *p);


}

//Tinh gia tri ngau nhien x va tinh h
void genCle(long long int  q, long long int  g, long long int  *h, long long int  *x, long long int  p){
	*x = (rand() % q) + 1;
	*h = expMod(g, *x, p);
}

// Ma hoa
void encrypt(long long int  p, long long int  g, long long int  m, long long int  h,  long long int  *c1, long long int  *c2, long long int  q){
	long long int  y = (rand() % q) + 1;

	
	*c1 = expMod (g, y, p);

	long long int  s = expMod(h, y ,p);
	*c2 = (m*s % p);
}

//giai ma
long long int  decrypt(long long int  p, long long int  x, long long int  c1, long long int  c2){
	long long int  s = expMod(c1, x ,p);
	long long int  sInv = invMod(p, s) %p;
	if(sInv<0)
		sInv+=p;
	

	return ((c2*sInv) % p);
}




int main(int argc, char *argv[]){

	 
     int chon;
     ifstream input,input1, input2;
     fstream output, op1,op2;
     string m,dec;
     long long int   c1, c2;
     long long int  p, q, g, h, x;
     
     long long int  Tab1[1000];
     long long int  Tab2[1000];
    do{
        cout<<"--------Menu--------"<<endl;
        cout<<"1. Tao khoa. "<<endl;
        cout<<"2. Ma hoa."<<endl;
        cout<<"3. Giai ma."<<endl;
        cout<<"4. Thoat."<<endl;
        cout<<"Vui long chon:";
        cin>>chon;
        
        switch (chon)
        {
        case 1:
            
            srand((int) time(0));
	        genGroupe(&p, &q, &g);
	        genCle(q, g, &h, &x, p);
            output.open("../key/private_key.txt", ios::out );
            output << q << endl << g << endl << x <<endl;
            output.close();
            output.open("../key/public_key.txt", ios::out );
            output << q << endl << g << endl << h <<endl;
            output.close();
            cout<<"Tao khoa thanh cong!!!"<<endl;
            break;
        case 2:
            input.open("../msg/plaintext.txt");
            
            while(!input.eof())	
            {
                getline(input,m);
            }
            input.close();
            genGroupe(&p, &q, &g);
            input2.open("../key/public_key.txt");
            input2 >> q >> g >>h;
            input2.close();
            cout<<" Message can ma hoa : ";
            // cin.ignore();
            // getline(cin,m);
            cout<<m<<endl;

    
            op1.open("../msg/c1.txt", ios::out );
            op2.open("../msg/c2.txt", ios::out );
            for(int i=0; i<m.length(); i++){
                
                char c=m.at(i);
                encrypt(p, g, (long long int )c, h, &c1, &c2, q);
                Tab1[i]=c1;
                Tab2[i]=c2;
                op1 << c1 <<endl;
                op2 << c2 <<endl;    
            }
            op1.close();
            op2.close();
            cout<<endl<<"Ban ma cua message duoc tao thanh cong!!!"<<endl;
            cout<<endl;
            break;
        case 3:
            genGroupe(&p, &q, &g);
            int i, dem;
            input1.open("../key/private_key.txt");
            input1 >> q >> g >>x;
            input1.close();
            input2.open("../key/public_key.txt");
            input2 >> q >> g >>h;
            input2.close();
            input.open("../msg/c1.txt");
            i=0;
            dem=0;
            while(!input.eof()){
                string chuoi;
                getline(input,chuoi);
                Tab1[i] = convert(chuoi);
                i++;
            }
            dem=i-1;
            input.close();
            input.open("../msg/c2.txt");
            for(i=0;i<dem;i++){
                input>>Tab2[i];
            }
            input.close();
            for( i=0; i<dem; i++){
               
               dec+=(char)decrypt(p, x, Tab1[i], Tab2[i]);
            }
            cout<<"Message sau giai ma la : "<<dec<<endl;
            break;
        case 4:
            return 0;
        }
    }while(chon);

	 
	return 0;
}