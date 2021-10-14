#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include <stdbool.h>


 unsigned long long int p,q,t,temp[100],j;

  unsigned long long  int decryptedText[100], encryptedText[100],dec[100],i,len;

bool flag;
char msg[100];
FILE *fpIn, *fpOut;
void encrypt();
void decrypt();
unsigned long long int gcd( unsigned long long  int a, unsigned long long int b);
bool isPrime(unsigned long long int);
int   tinhE();
unsigned long long int tinhD(unsigned long long int);
int main(){
    int chon;
    while(true){
        while(true) {
        printf("\n--------Menu--------\n");
        printf("1. Tao khoa. \n");
        printf("2. Ma hoa.\n");
        printf("3. Giai ma.\n");
        printf("4. Thoat.\n");
        printf("Vui long chon:");
        scanf("%d", &chon);
        switch(chon) {
            case 1:
                do{
                printf("nhap p:");
                scanf("%llu",&p);
                flag = isPrime(p);
                if(flag == false){
                printf("phai nhap so nguyen to");
       } 
    }while(flag==0);
    do{
 
                printf("nhap q:");
                scanf("%llu",&q);
                flag = isPrime(q);
                if(flag == false){
               printf("phai nhap so nguyen to");
       }
    }while(flag==0);
                unsigned long long int n;
                 n= p*q;
                 t = (p-1)*(q-1);
                 int e = tinhE();
                unsigned  long long int d = tinhD(e);
                fpOut = fopen("../key/public_key.txt","w+");
                fprintf(fpOut,"%d \n %llu ",e,n);
                fclose(fpOut);
                fpOut = fopen("../key/private_key.txt","w+");
                fprintf(fpOut,"%llu",d);
                fclose(fpOut);
                printf("\nTao khoa thanh cong!!!");
                break;
            case 2:
                printf("Nhap message can ma hoa:");
                    fflush(stdin);
                fgets(msg, sizeof(msg), stdin);
                    for( i=0; msg[i] != '\0'; i++)
                    {
                    decryptedText[i] = msg[i];
                    }
                    
                     encrypt();
                break;
            case 3:
                    decrypt();
                break;
            case 4:
                return 0;
        }
    }
    }
   
   
    return 0;
}

bool isPrime(unsigned long long int pr){
    unsigned long long  int i;
    j = (unsigned long long int) sqrt(( long double)pr);
    for(i=2; i<=j ; i++){
        if(pr%i==0)
        return false;
    }
    return true;
}
int tinhE(){
    for(i = 5; i<=65537 ; i++ ){
        if(gcd(i,t)==1)
        return i;
    }
    return -1;
}
unsigned long long int tinhD(unsigned long long int x){
    long int k=1;
    while(1){
        k = k+t;
        if(k%x==0)
        return (k/x);
    }
}

void encrypt(){
     int e;
    unsigned long long int n;
    fpIn = fopen("../key/public_key.txt","r");
    while(!feof(fpIn)){
        fscanf(fpIn,"%d",&e);
        fscanf(fpIn,"%llu",&n);
    }
     fclose(fpIn);
   unsigned  long long int pt, ct, key=e,k;
    i=0;
  
    len = strlen(msg);
    while (i != len)
    {
        pt = decryptedText[i];
        
       
        k=1;
        for(j=0; j<key; j++){
            k = k * pt;
            k = k % n;
        }
        temp[i]= k;
      
        ct=k;
        
        encryptedText[i] = ct;
        i++;
    }
    encryptedText[i] = -1;
  
    printf("Messga duoc ma hoa la:");
    for(i=0; i<len; i++){
       putchar(encryptedText[i]);
    }
    
    
}

void decrypt(){
    unsigned  long long int d ,n;
    fpIn = fopen("../key/private_key.txt","r");
    while(!feof(fpIn)){
        fscanf(fpIn,"%llu",&d);}

    fclose(fpIn);
    fpIn = fopen("../key/public_key.txt","r");
    while(!feof(fpIn)){
        fscanf(fpIn,"%llu",&n);
    }
    fclose(fpIn);
 
     
    unsigned long long int pt, ct, key = d, k;
    i=0;
      while (encryptedText[i] !=-1){
        ct = temp[i];
        k=1;
        for(j=0 ; j< key; j++){
            k = k *ct;
            k = k %n;
        }
     
        pt=k;
        decryptedText[i] = pt;
        i++;
    }
    decryptedText[i] = -1;
 
  printf(" \n Massage duoc giai ma la:");
    for(i = 0; i<len; i++)
    
    putchar(decryptedText[i]);
    
}
unsigned long long int gcd(unsigned long long int a,unsigned long long int b) {
  unsigned  long long int t;
   while(1) {
      t= a%b;
      if(t==0)
      return b;
      a = b;
      b = t;
   }
}