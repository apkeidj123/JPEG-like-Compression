#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <string>
#include <memory.h>
#include <iomanip>
#include <math.h>
#include <vector>
#include <bitset>
#include <fstream>
#include "dct2.c"

#define height 512
#define width  512

using namespace std;

string DC_bitstring[4096]={""};
string AC_bitstring[4096]={""};
string DC_AC_bits="";
string instr="";
//string deDC[4096]={""};
//string deAC[4096][65]={""};
//unsigned long
int AC_zigarr[64]={0};
int AC_testarr[64]={0};
int index_DC=0;
int index_AC=0;
int err_cnt=0;
int deDCval[4096]={0};
int deACval[4096][129]={1024};
int deAC_zigarr[4096][64]={0};
int idx_AC=0;
int idx_DC=0;
int index =0;
int QFC=90;

/**(int)pow**/

int Power(int x, int y){
    int tmp = x;
    if(y==0){
        x=1;
    }
    else{
        for(int i = 1 ; i < y ; i++){
            x*=tmp;
        }
    }
    return x;
}

/**Quantization**/

void quantize(float **x ,int n,int QF){
    int Lumin[n][n];
    int Chrom[n][n];
    float factor=0.0;

    Lumin[0][0]=16;Lumin[0][1]=11;Lumin[0][2]=10;Lumin[0][3]=16;
    Lumin[0][4]=24;Lumin[0][5]=40;Lumin[0][6]=51;Lumin[0][7]=61;
    Lumin[1][0]=12;Lumin[1][1]=12;Lumin[1][2]=14;Lumin[1][3]=19;
    Lumin[1][4]=26;Lumin[1][5]=58;Lumin[1][6]=60;Lumin[1][7]=55;
    Lumin[2][0]=14;Lumin[2][1]=13;Lumin[2][2]=16;Lumin[2][3]=24;
    Lumin[2][4]=40;Lumin[2][5]=57;Lumin[2][6]=69;Lumin[2][7]=56;
    Lumin[3][0]=14;Lumin[3][1]=17;Lumin[3][2]=22;Lumin[3][3]=29;
    Lumin[3][4]=51;Lumin[3][5]=87;Lumin[3][6]=80;Lumin[3][7]=62;
    Lumin[4][0]=18;Lumin[4][1]=22;Lumin[4][2]=37;Lumin[4][3]=56;
    Lumin[4][4]=68;Lumin[4][5]=109;Lumin[4][6]=103;Lumin[4][7]=77;
    Lumin[5][0]=24;Lumin[5][1]=36;Lumin[5][2]=55;Lumin[5][3]=64;
    Lumin[5][4]=81;Lumin[5][5]=104;Lumin[5][6]=113;Lumin[5][7]=92;
    Lumin[6][0]=49;Lumin[6][1]=64;Lumin[6][2]=78;Lumin[6][3]=87;
    Lumin[6][4]=103;Lumin[6][5]=121;Lumin[6][6]=120;Lumin[6][7]=101;
    Lumin[7][0]=72;Lumin[7][1]=92;Lumin[7][2]=95;Lumin[7][3]=98;
    Lumin[7][4]=112;Lumin[7][5]=100;Lumin[7][6]=103;Lumin[7][7]=99;

    if(QF<50){
        factor=round(5000.0/QF);
    }
    else{
        factor=round(200.0-2*QF);
    }
    if(factor==0){
        factor==1;
    }
    //cout<<"factor = "<<factor;

    for(int i = 0 ; i < n ; i++){
        for(int j = 0 ; j < n ; j++){
            Chrom[i][j]=round(Lumin[i][j]*factor/100);

            if(Chrom[i][j]!=0){
                x[i][j]=ceil(x[i][j]/Chrom[i][j]);
            }
            else{
                    //cout<<"fuck";
                x[i][j]=round(x[i][j]);
            }

        }
    }

}

/**DeQuantization**/

void dequantize(float **x ,int n,int QF){
    int Lumin[n][n];
    int Chrom[n][n];
    float factor=0.0;

    Lumin[0][0]=16;Lumin[0][1]=11;Lumin[0][2]=10;Lumin[0][3]=16;
    Lumin[0][4]=24;Lumin[0][5]=40;Lumin[0][6]=51;Lumin[0][7]=61;
    Lumin[1][0]=12;Lumin[1][1]=12;Lumin[1][2]=14;Lumin[1][3]=19;
    Lumin[1][4]=26;Lumin[1][5]=58;Lumin[1][6]=60;Lumin[1][7]=55;
    Lumin[2][0]=14;Lumin[2][1]=13;Lumin[2][2]=16;Lumin[2][3]=24;
    Lumin[2][4]=40;Lumin[2][5]=57;Lumin[2][6]=69;Lumin[2][7]=56;
    Lumin[3][0]=14;Lumin[3][1]=17;Lumin[3][2]=22;Lumin[3][3]=29;
    Lumin[3][4]=51;Lumin[3][5]=87;Lumin[3][6]=80;Lumin[3][7]=62;
    Lumin[4][0]=18;Lumin[4][1]=22;Lumin[4][2]=37;Lumin[4][3]=56;
    Lumin[4][4]=68;Lumin[4][5]=109;Lumin[4][6]=103;Lumin[4][7]=77;
    Lumin[5][0]=24;Lumin[5][1]=36;Lumin[5][2]=55;Lumin[5][3]=64;
    Lumin[5][4]=81;Lumin[5][5]=104;Lumin[5][6]=113;Lumin[5][7]=92;
    Lumin[6][0]=49;Lumin[6][1]=64;Lumin[6][2]=78;Lumin[6][3]=87;
    Lumin[6][4]=103;Lumin[6][5]=121;Lumin[6][6]=120;Lumin[6][7]=101;
    Lumin[7][0]=72;Lumin[7][1]=92;Lumin[7][2]=95;Lumin[7][3]=98;
    Lumin[7][4]=112;Lumin[7][5]=100;Lumin[7][6]=103;Lumin[7][7]=99;

    if(QF<50){
        factor=round(5000.0/QF);
    }
    else{
        factor=round(200.0-2*QF);
    }
    if(factor==0){
        factor==1;
    }

    for(int i = 0 ; i < n ; i++){
        for(int j = 0 ; j < n ; j++){
            Chrom[i][j]=round(Lumin[i][j]*factor/100);
            x[i][j]=round(x[i][j]*Chrom[i][j]);
        }
    }

}

/**DC Category**/

int getCategoryOfDCTCoefficient(int x){
    if(x==0)
        return 0;
    else if(x==-1 || x==1)
        return 1;
    else if((x>=-3 && x<=-2)||(x>=2 && x<=3))
        return 2;
    else if((x>=-7 && x<=-4)||(x>=4 && x<=7))
        return 3;
    else if((x>=-15 && x<=-8)||(x>=8 && x<=15))
        return 4;
    else if((x>=-31 && x<=-16)||(x>=16 && x<=31))
        return 5;
    else if((x>=-63 && x<=-32)||(x>=32 && x<=63))
        return 6;
    else if((x>=-127 && x<=-64)||(x>=64 && x<=127))
        return 7;
    else if((x>=-255 && x<=-128)||(x>=128 && x<=255))
        return 8;
    else if((x>=-511 && x<=-256)||(x>=256 && x<=511))
        return 9;
    else if((x>=-1023 && x<=-512)||(x>=512 && x<=1023))
        return 10;
    else if((x>=-2047 && x<=-1024)||(x>=1024 && x<=2047))
        return 11;
    else if((x>=-4095 && x<=-2048)||(x>=2048 && x<=4095))
        return 12;
    else if((x>=-8191 && x<=-4096)||(x>=4096 && x<=8191))
        return 13;
    else if((x>=-16383 && x<=-8192)||(x>=8192 && x<=16383))
        return 14;
    else if((x>=-32767 && x<=-16384)||(x>=16384 && x<=32767))
        return 15;
    else
        return -1;
}

/**DC DIFF value code word**/

void DC_Diffcode(int DC_Diff,int DC_Category,int DC_index){
    int t=Power(2,DC_Category-1);//min positive number
    int DC_value=Power(2,DC_Category)-1;//right border
    int idx=0;
    string bstr="";

    if(DC_Category==1){
        if(DC_Diff>=t){
            bitset<1> b(DC_Diff);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            DC_bitstring[DC_index]+=bstr;
        }
        else{
            idx=DC_Diff+DC_value;
            bitset<1> b(idx);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            DC_bitstring[DC_index]+=bstr;
        }
    }
    else if(DC_Category==2){
        if(DC_Diff>=t){
            bitset<2> b(DC_Diff);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            DC_bitstring[DC_index]+=bstr;
        }
        else{
            idx=DC_Diff+DC_value;
            bitset<2> b(idx);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            DC_bitstring[DC_index]+=bstr;
        }
    }
    else if(DC_Category==3){
        if(DC_Diff>=t){
            bitset<3> b(DC_Diff);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            DC_bitstring[DC_index]+=bstr;
        }
        else{
            idx=DC_Diff+DC_value;
            bitset<3> b(idx);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            DC_bitstring[DC_index]+=bstr;
        }
    }
    else if(DC_Category==4){
        if(DC_Diff>=t){
            bitset<4> b(DC_Diff);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            DC_bitstring[DC_index]+=bstr;
        }
        else{
            idx=DC_Diff+DC_value;
            bitset<4> b(idx);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            DC_bitstring[DC_index]+=bstr;
        }
    }
    else if(DC_Category==5){
        if(DC_Diff>=t){
            bitset<5> b(DC_Diff);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            DC_bitstring[DC_index]+=bstr;
        }
        else{
            idx=DC_Diff+DC_value;
            bitset<5> b(idx);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            DC_bitstring[DC_index]+=bstr;
        }
    }
    else if(DC_Category==6){
        if(DC_Diff>=t){
            bitset<6> b(DC_Diff);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            DC_bitstring[DC_index]+=bstr;
        }
        else{
            idx=DC_Diff+DC_value;
            bitset<6> b(idx);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            DC_bitstring[DC_index]+=bstr;
        }
    }
    else if(DC_Category==7){
        if(DC_Diff>=t){
            bitset<7> b(DC_Diff);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            DC_bitstring[DC_index]+=bstr;
        }
        else{
            idx=DC_Diff+DC_value;
            bitset<7> b(idx);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            DC_bitstring[DC_index]+=bstr;
        }
    }
    else if(DC_Category==8){
        if(DC_Diff>=t){
            bitset<8> b(DC_Diff);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            DC_bitstring[DC_index]+=bstr;
        }
        else{
            idx=DC_Diff+DC_value;
            bitset<8> b(idx);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            DC_bitstring[DC_index]+=bstr;
        }
    }
    else if(DC_Category==9){
        if(DC_Diff>=t){
            bitset<9> b(DC_Diff);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            DC_bitstring[DC_index]+=bstr;
        }
        else{
            idx=DC_Diff+DC_value;
            bitset<9> b(idx);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            DC_bitstring[DC_index]+=bstr;
        }
    }
    else if(DC_Category==10){
        if(DC_Diff>=t){
            bitset<10> b(DC_Diff);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            DC_bitstring[DC_index]+=bstr;
        }
        else{
            idx=DC_Diff+DC_value;
            bitset<10> b(idx);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            DC_bitstring[DC_index]+=bstr;
        }
    }
    else if(DC_Category==11){
        if(DC_Diff>=t){
            bitset<11> b(DC_Diff);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            DC_bitstring[DC_index]+=bstr;
        }
        else{
            idx=DC_Diff+DC_value;
            bitset<11> b(idx);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            DC_bitstring[DC_index]+=bstr;
        }
    }
    else{
        cout<<"Are you kidding me?"<<endl;
        err_cnt++;
    }
}

/**JPEG DC code word = Category code word + DIFF value code word**/

void DC_codeword(int DC_Diff,int DC_Category,int DC_index){
    if(DC_Category==0){
        DC_bitstring[DC_index]+="00";//Category code word
    }
    else if(DC_Category==1){
        DC_bitstring[DC_index]+="010";
        /*if(DC_Diff==-1)DC_bitstring[DC_index]+="0";
        else if(DC_Diff==1)DC_bitstring[DC_index]+="1";*/
        DC_Diffcode(DC_Diff,DC_Category,DC_index);
    }
    else if(DC_Category==2){
        DC_bitstring[DC_index]+="011";
        DC_Diffcode(DC_Diff,DC_Category,DC_index);
    }
    else if(DC_Category==3){
        DC_bitstring[DC_index]+="100";
        DC_Diffcode(DC_Diff,DC_Category,DC_index);
    }
    else if(DC_Category==4){
        DC_bitstring[DC_index]+="101";
        DC_Diffcode(DC_Diff,DC_Category,DC_index);
    }
    else if(DC_Category==5){
        DC_bitstring[DC_index]+="110";
        DC_Diffcode(DC_Diff,DC_Category,DC_index);
    }
    else if(DC_Category==6){
        DC_bitstring[DC_index]+="1110";
        DC_Diffcode(DC_Diff,DC_Category,DC_index);
    }
    else if(DC_Category==7){
        DC_bitstring[DC_index]+="11110";
        DC_Diffcode(DC_Diff,DC_Category,DC_index);
    }
    else if(DC_Category==8){
        DC_bitstring[DC_index]+="111110";
        DC_Diffcode(DC_Diff,DC_Category,DC_index);
    }
    else if(DC_Category==9){
        DC_bitstring[DC_index]+="1111110";
        DC_Diffcode(DC_Diff,DC_Category,DC_index);
    }
    else if(DC_Category==10){
        DC_bitstring[DC_index]+="11111110";
        DC_Diffcode(DC_Diff,DC_Category,DC_index);
    }
    else if(DC_Category==11){
        DC_bitstring[DC_index]+="111111110";
        DC_Diffcode(DC_Diff,DC_Category,DC_index);
    }

    index_DC++;
}

/**ZigZagCoding 2D to 1D**/

void AC_zigzag(float **x){
    int i=0,j=1,k=1;

    AC_zigarr[0]=x[0][0];

    while(1){
        while(j!=0 && i!=7){//Going upside down until j!=0
            AC_zigarr[k++]=x[i][j];
           // k++;
            i=i+1;
            j=j-1;
        }
        AC_zigarr[k++]=x[i][j];//Take the edge element
       // k++;

        if(i<7)//If not last row, increment i
            i=i+1;

        else if(i==7)//If we hit the last row, we go right one place
            j=j+1;

        while(i!=0 && j!=7){//Going bottom up
            AC_zigarr[k++]=x[i][j];
            i=i-1;
            j=j+1;
        }
        AC_zigarr[k++]=x[i][j];//Take the edge element
        if(j<7)//If we didn't hit the edge, increment j
            j=j+1;

        else if(j==7)//If we hit the last element, go down one place
            i=i+1;

        if(i>=7 && j>=7)//If we hit last element matrix[8][8] exit
            break;
    }


}

/**VLC Table for Luminance AC Coefficients in JPEG**/

void AC_Coefficient(int Run,int Size,int AC_index){

    if(Run==0){
        if(Size==0){      AC_bitstring[AC_index]+="1010";}    /**EOB**/
        else if(Size==1){ AC_bitstring[AC_index]+="00";}
        else if(Size==2){ AC_bitstring[AC_index]+="01";}
        else if(Size==3){ AC_bitstring[AC_index]+="100";}
        else if(Size==4){ AC_bitstring[AC_index]+="1011";}
        else if(Size==5){ AC_bitstring[AC_index]+="11010";}
        else if(Size==6){ AC_bitstring[AC_index]+="1111000";}
        else if(Size==7){ AC_bitstring[AC_index]+="11111000";}
        else if(Size==8){ AC_bitstring[AC_index]+="1111110110";}
        else if(Size==9){ AC_bitstring[AC_index]+="1111111110000010";}
        else if(Size==10){AC_bitstring[AC_index]+="1111111110000011";}
        else{cout<<"Size= "<<Size<<" Run0fuck\n";err_cnt++;}
    }
    else if(Run==1){
        if(Size==1){      AC_bitstring[AC_index]+="1100";}
        else if(Size==2){ AC_bitstring[AC_index]+="11011";}
        else if(Size==3){ AC_bitstring[AC_index]+="1111001";}
        else if(Size==4){ AC_bitstring[AC_index]+="111110110";}
        else if(Size==5){ AC_bitstring[AC_index]+="11111110110";}
        else if(Size==6){ AC_bitstring[AC_index]+="1111111110000100";}
        else if(Size==7){ AC_bitstring[AC_index]+="1111111110000101";}
        else if(Size==8){ AC_bitstring[AC_index]+="1111111110000110";}
        else if(Size==9){ AC_bitstring[AC_index]+="1111111110000111";}
        else if(Size==10){AC_bitstring[AC_index]+="1111111110001000";}
        else{cout<<"Size= "<<Size<<" Run1fuck\n";err_cnt++;}
    }
    else if(Run==2){
        if(Size==1){      AC_bitstring[AC_index]+="11100";}
        else if(Size==2){ AC_bitstring[AC_index]+="11111001";}
        else if(Size==3){ AC_bitstring[AC_index]+="1111110111";}
        else if(Size==4){ AC_bitstring[AC_index]+="111111110100";}
        else if(Size==5){ AC_bitstring[AC_index]+="1111111110001001";}
        else if(Size==6){ AC_bitstring[AC_index]+="1111111110001010";}
        else if(Size==7){ AC_bitstring[AC_index]+="1111111110001011";}
        else if(Size==8){ AC_bitstring[AC_index]+="1111111110001100";}
        else if(Size==9){ AC_bitstring[AC_index]+="1111111110001101";}
        else if(Size==10){AC_bitstring[AC_index]+="1111111110001110";}
        else{cout<<"Size= "<<Size<<" Run2fuck\n";err_cnt++;}
    }
    else if(Run==3){
        if(Size==1){      AC_bitstring[AC_index]+="111010";}
        else if(Size==2){ AC_bitstring[AC_index]+="111110111";}
        else if(Size==3){ AC_bitstring[AC_index]+="111111110101";}
        else if(Size==4){ AC_bitstring[AC_index]+="1111111110001111";}
        else if(Size==5){ AC_bitstring[AC_index]+="1111111110010000";}
        else if(Size==6){ AC_bitstring[AC_index]+="1111111110010001";}
        else if(Size==7){ AC_bitstring[AC_index]+="1111111110010010";}
        else if(Size==8){ AC_bitstring[AC_index]+="1111111110010011";}
        else if(Size==9){ AC_bitstring[AC_index]+="1111111110010100";}
        else if(Size==10){AC_bitstring[AC_index]+="1111111110010101";}
        else{cout<<"Size= "<<Size<<" Run3fuck\n";err_cnt++;}
    }
    else if(Run==4){
        if(Size==1){      AC_bitstring[AC_index]+="111011";}
        else if(Size==2){ AC_bitstring[AC_index]+="1111111000";}
        else if(Size==3){ AC_bitstring[AC_index]+="1111111110010110";}
        else if(Size==4){ AC_bitstring[AC_index]+="1111111110010111";}
        else if(Size==5){ AC_bitstring[AC_index]+="1111111110011000";}
        else if(Size==6){ AC_bitstring[AC_index]+="1111111110011001";}
        else if(Size==7){ AC_bitstring[AC_index]+="1111111110011010";}
        else if(Size==8){ AC_bitstring[AC_index]+="1111111110011011";}
        else if(Size==9){ AC_bitstring[AC_index]+="1111111110011100";}
        else if(Size==10){AC_bitstring[AC_index]+="1111111110011101";}
        else{cout<<"Size= "<<Size<<" Run4fuck\n";err_cnt++;}
    }
    else if(Run==5){
        if(Size==1){      AC_bitstring[AC_index]+="1111010";}
        else if(Size==2){ AC_bitstring[AC_index]+="11111110111";}
        else if(Size==3){ AC_bitstring[AC_index]+="1111111110011110";}
        else if(Size==4){ AC_bitstring[AC_index]+="1111111110011111";}
        else if(Size==5){ AC_bitstring[AC_index]+="1111111110100000";}
        else if(Size==6){ AC_bitstring[AC_index]+="1111111110100001";}
        else if(Size==7){ AC_bitstring[AC_index]+="1111111110100010";}
        else if(Size==8){ AC_bitstring[AC_index]+="1111111110100011";}
        else if(Size==9){ AC_bitstring[AC_index]+="1111111110100100";}
        else if(Size==10){AC_bitstring[AC_index]+="1111111110100101";}
        else{cout<<"Size= "<<Size<<" Run5fuck\n";err_cnt++;}
    }
    else if(Run==6){
        if(Size==1){      AC_bitstring[AC_index]+="1111011";}
        else if(Size==2){ AC_bitstring[AC_index]+="111111110110";}
        else if(Size==3){ AC_bitstring[AC_index]+="1111111110100110";}
        else if(Size==4){ AC_bitstring[AC_index]+="1111111110100111";}
        else if(Size==5){ AC_bitstring[AC_index]+="1111111110101000";}
        else if(Size==6){ AC_bitstring[AC_index]+="1111111110101001";}
        else if(Size==7){ AC_bitstring[AC_index]+="1111111110101010";}
        else if(Size==8){ AC_bitstring[AC_index]+="1111111110101011";}
        else if(Size==9){ AC_bitstring[AC_index]+="1111111110101100";}
        else if(Size==10){AC_bitstring[AC_index]+="1111111110101101";}
        else{cout<<"Size= "<<Size<<" Run6fuck\n";err_cnt++;}
    }
    else if(Run==7){
        if(Size==1){      AC_bitstring[AC_index]+="11111010";}
        else if(Size==2){ AC_bitstring[AC_index]+="111111110111";}
        else if(Size==3){ AC_bitstring[AC_index]+="1111111110101110";}
        else if(Size==4){ AC_bitstring[AC_index]+="1111111110101111";}
        else if(Size==5){ AC_bitstring[AC_index]+="1111111110110000";}
        else if(Size==6){ AC_bitstring[AC_index]+="1111111110110001";}
        else if(Size==7){ AC_bitstring[AC_index]+="1111111110110010";}
        else if(Size==8){ AC_bitstring[AC_index]+="1111111110110011";}
        else if(Size==9){ AC_bitstring[AC_index]+="1111111110110100";}
        else if(Size==10){AC_bitstring[AC_index]+="1111111110110101";}
        else{cout<<"Size= "<<Size<<" Run7fuck\n";err_cnt++;}
    }
    else if(Run==8){
        if(Size==1){      AC_bitstring[AC_index]+="111111000";}
        else if(Size==2){ AC_bitstring[AC_index]+="111111111000000";}
        else if(Size==3){ AC_bitstring[AC_index]+="1111111110110110";}
        else if(Size==4){ AC_bitstring[AC_index]+="1111111110110111";}
        else if(Size==5){ AC_bitstring[AC_index]+="1111111110111000";}
        else if(Size==6){ AC_bitstring[AC_index]+="1111111110111001";}
        else if(Size==7){ AC_bitstring[AC_index]+="1111111110111010";}
        else if(Size==8){ AC_bitstring[AC_index]+="1111111110111011";}
        else if(Size==9){ AC_bitstring[AC_index]+="1111111110111100";}
        else if(Size==10){AC_bitstring[AC_index]+="1111111110111101";}
        else{cout<<"Size= "<<Size<<" Run8fuck\n";err_cnt++;}
    }
    else if(Run==9){
        if(Size==1){      AC_bitstring[AC_index]+="111111001";}
        else if(Size==2){ AC_bitstring[AC_index]+="1111111110111110";}
        else if(Size==3){ AC_bitstring[AC_index]+="1111111110111111";}
        else if(Size==4){ AC_bitstring[AC_index]+="1111111111000000";}
        else if(Size==5){ AC_bitstring[AC_index]+="1111111111000001";}
        else if(Size==6){ AC_bitstring[AC_index]+="1111111111000010";}
        else if(Size==7){ AC_bitstring[AC_index]+="1111111111000011";}
        else if(Size==8){ AC_bitstring[AC_index]+="1111111111000100";}
        else if(Size==9){ AC_bitstring[AC_index]+="1111111111000101";}
        else if(Size==10){AC_bitstring[AC_index]+="1111111111000110";}
        else{cout<<"Size= "<<Size<<" Run9fuck\n";err_cnt++;}
    }
    else if(Run==10){
        if(Size==1){      AC_bitstring[AC_index]+="111111010";}
        else if(Size==2){ AC_bitstring[AC_index]+="1111111111000111";}
        else if(Size==3){ AC_bitstring[AC_index]+="1111111111001000";}
        else if(Size==4){ AC_bitstring[AC_index]+="1111111111001001";}
        else if(Size==5){ AC_bitstring[AC_index]+="1111111111001010";}
        else if(Size==6){ AC_bitstring[AC_index]+="1111111111001011";}
        else if(Size==7){ AC_bitstring[AC_index]+="1111111111001100";}
        else if(Size==8){ AC_bitstring[AC_index]+="1111111111001101";}
        else if(Size==9){ AC_bitstring[AC_index]+="1111111111001110";}
        else if(Size==10){AC_bitstring[AC_index]+="1111111111001111";}
        else{cout<<"Size= "<<Size<<" Run10fuck\n";err_cnt++;}
    }
    else if(Run==11){
        if(Size==1){      AC_bitstring[AC_index]+="1111111001";}
        else if(Size==2){ AC_bitstring[AC_index]+="1111111111010000";}
        else if(Size==3){ AC_bitstring[AC_index]+="1111111111010001";}
        else if(Size==4){ AC_bitstring[AC_index]+="1111111111010010";}
        else if(Size==5){ AC_bitstring[AC_index]+="1111111111010011";}
        else if(Size==6){ AC_bitstring[AC_index]+="1111111111010100";}
        else if(Size==7){ AC_bitstring[AC_index]+="1111111111010101";}
        else if(Size==8){ AC_bitstring[AC_index]+="1111111111010110";}
        else if(Size==9){ AC_bitstring[AC_index]+="1111111111010111";}
        else if(Size==10){AC_bitstring[AC_index]+="1111111111011000";}
        else{cout<<"Size= "<<Size<<" Run11fuck\n";err_cnt++;}
    }
    else if(Run==12){
        if(Size==1){      AC_bitstring[AC_index]+="1111111010";}
        else if(Size==2){ AC_bitstring[AC_index]+="1111111111011001";}
        else if(Size==3){ AC_bitstring[AC_index]+="1111111111011010";}
        else if(Size==4){ AC_bitstring[AC_index]+="1111111111011011";}
        else if(Size==5){ AC_bitstring[AC_index]+="1111111111011100";}
        else if(Size==6){ AC_bitstring[AC_index]+="1111111111011101";}
        else if(Size==7){ AC_bitstring[AC_index]+="1111111111011110";}
        else if(Size==8){ AC_bitstring[AC_index]+="1111111111011111";}
        else if(Size==9){ AC_bitstring[AC_index]+="1111111111100000";}
        else if(Size==10){AC_bitstring[AC_index]+="1111111111100001";}
        else{cout<<"Size= "<<Size<<" Run12fuck\n";err_cnt++;}
    }
    else if(Run==13){
        if(Size==1){      AC_bitstring[AC_index]+="11111111000";}
        else if(Size==2){ AC_bitstring[AC_index]+="1111111111100010";}
        else if(Size==3){ AC_bitstring[AC_index]+="1111111111100011";}
        else if(Size==4){ AC_bitstring[AC_index]+="1111111111100100";}
        else if(Size==5){ AC_bitstring[AC_index]+="1111111111100101";}
        else if(Size==6){ AC_bitstring[AC_index]+="1111111111100110";}
        else if(Size==7){ AC_bitstring[AC_index]+="1111111111100111";}
        else if(Size==8){ AC_bitstring[AC_index]+="1111111111101000";}
        else if(Size==9){ AC_bitstring[AC_index]+="1111111111101001";}
        else if(Size==10){AC_bitstring[AC_index]+="1111111111101010";}
        else{cout<<"Size= "<<Size<<" Run13fuck\n";err_cnt++;}
    }
    else if(Run==14){
        if(Size==1){      AC_bitstring[AC_index]+="1111111111101011";}
        else if(Size==2){ AC_bitstring[AC_index]+="1111111111101100";}
        else if(Size==3){ AC_bitstring[AC_index]+="1111111111101101";}
        else if(Size==4){ AC_bitstring[AC_index]+="1111111111101110";}
        else if(Size==5){ AC_bitstring[AC_index]+="1111111111101111";}
        else if(Size==6){ AC_bitstring[AC_index]+="1111111111110000";}
        else if(Size==7){ AC_bitstring[AC_index]+="1111111111110001";}
        else if(Size==8){ AC_bitstring[AC_index]+="1111111111110010";}
        else if(Size==9){ AC_bitstring[AC_index]+="1111111111110011";}
        else if(Size==10){AC_bitstring[AC_index]+="1111111111110100";}
        else{cout<<"Size= "<<Size<<" Run14fuck\n";err_cnt++;}
    }
    else if(Run==15){
        if(Size==0){      AC_bitstring[AC_index]+="11111111001";}     /**ZRL**/
        else if(Size==1){ AC_bitstring[AC_index]+="1111111111110101";}
        else if(Size==2){ AC_bitstring[AC_index]+="1111111111110110";}
        else if(Size==3){ AC_bitstring[AC_index]+="1111111111110111";}
        else if(Size==4){ AC_bitstring[AC_index]+="1111111111111000";}
        else if(Size==5){ AC_bitstring[AC_index]+="1111111111111001";}
        else if(Size==6){ AC_bitstring[AC_index]+="1111111111111010";}
        else if(Size==7){ AC_bitstring[AC_index]+="1111111111111011";}
        else if(Size==8){ AC_bitstring[AC_index]+="1111111111111100";}
        else if(Size==9){ AC_bitstring[AC_index]+="1111111111111101";}
        else if(Size==10){AC_bitstring[AC_index]+="1111111111111110";}
        else{cout<<"Size= "<<Size<<" Run15fuck\n";err_cnt++;}
    }
    else{
        cout<<"Run = "<<Run<<" Are you serious?"<<endl;err_cnt++;
    }

}

/**AC coefficient code word**/

void AC_Coefcode(int AC_Coef,int AC_Category,int AC_index){
    int t=Power(2,AC_Category-1);//min positive number
    int AC_value=Power(2,AC_Category)-1;//right border
    int idx=0;
    string bstr="";

    if(AC_Category==0){

    }
    else if(AC_Category==1){
        if(AC_Coef>=t){
            bitset<1> b(AC_Coef);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            AC_bitstring[AC_index]+=bstr;
        }
        else{
            idx=AC_Coef+AC_value;
            bitset<1> b(idx);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            AC_bitstring[AC_index]+=bstr;
        }
    }
    else if(AC_Category==2){
        if(AC_Coef>=t){
            bitset<2> b(AC_Coef);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            AC_bitstring[AC_index]+=bstr;
        }
        else{
            idx=AC_Coef+AC_value;
            bitset<2> b(idx);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            AC_bitstring[AC_index]+=bstr;
        }
    }
    else if(AC_Category==3){
        if(AC_Coef>=t){
            bitset<3> b(AC_Coef);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            AC_bitstring[AC_index]+=bstr;
        }
        else{
            idx=AC_Coef+AC_value;
            bitset<3> b(idx);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            AC_bitstring[AC_index]+=bstr;
        }
    }
    else if(AC_Category==4){
        if(AC_Coef>=t){
            bitset<4> b(AC_Coef);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            AC_bitstring[AC_index]+=bstr;
        }
        else{
            idx=AC_Coef+AC_value;
            bitset<4> b(idx);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            AC_bitstring[AC_index]+=bstr;
        }
    }
    else if(AC_Category==5){
        if(AC_Coef>=t){
            bitset<5> b(AC_Coef);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            AC_bitstring[AC_index]+=bstr;
        }
        else{
            idx=AC_Coef+AC_value;
            bitset<5> b(idx);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            AC_bitstring[AC_index]+=bstr;
        }
    }
     else if(AC_Category==6){
        if(AC_Coef>=t){
            bitset<6> b(AC_Coef);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            AC_bitstring[AC_index]+=bstr;
        }
        else{
            idx=AC_Coef+AC_value;
            bitset<6> b(idx);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            AC_bitstring[AC_index]+=bstr;
        }
    }
     else if(AC_Category==7){
        if(AC_Coef>=t){
            bitset<7> b(AC_Coef);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            AC_bitstring[AC_index]+=bstr;
        }
        else{
            idx=AC_Coef+AC_value;
            bitset<7> b(idx);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            AC_bitstring[AC_index]+=bstr;
        }
    }
     else if(AC_Category==8){
        if(AC_Coef>=t){
            bitset<8> b(AC_Coef);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            AC_bitstring[AC_index]+=bstr;
        }
        else{
            idx=AC_Coef+AC_value;
            bitset<8> b(idx);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            AC_bitstring[AC_index]+=bstr;
        }
    }
     else if(AC_Category==9){
        if(AC_Coef>=t){
            bitset<9> b(AC_Coef);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            AC_bitstring[AC_index]+=bstr;
        }
        else{
            idx=AC_Coef+AC_value;
            bitset<9> b(idx);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            AC_bitstring[AC_index]+=bstr;
        }
    }
     else if(AC_Category==10){
        if(AC_Coef>=t){
            bitset<10> b(AC_Coef);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            AC_bitstring[AC_index]+=bstr;
        }
        else{
            idx=AC_Coef+AC_value;
            bitset<10> b(idx);
            bstr = b.to_string<char, char_traits<char>, allocator<char> > ();
            AC_bitstring[AC_index]+=bstr;
        }
    }
    else{
        cout<<"Does your head shut down?"<<endl;err_cnt++;
    }

}

/******AC code word******/

void AC_codeword(int *AC_arr){
    int Run=0,Size=0,temp=1,i,last=0;
    int AC_Coef=0;
    bool EOB=false;
    bool ZRL=false;

    for(i = 63 ; i > 0 ; i--){
        if(AC_arr[i]!=0){
            last=i;
            break;
        }
    }
    if(last!=0){
        for(i = 1 ; i <= last ; i++){
            if(AC_arr[i]==0){
                if(Run<15){
                    Run++;
                }
                else if(Run==15){//ZRL
                    Size=0;
                    AC_Coefficient(Run,Size,index_AC);
                    AC_Coefcode(AC_Coef,Size,index_AC);
                    Run=0;
                }
                else{
                        cout<<"Run over 15? WTF\n";
                }
            }
            else{
                Size=getCategoryOfDCTCoefficient(AC_arr[i]);
                AC_Coef=AC_arr[i];
                AC_Coefficient(Run,Size,index_AC);
                AC_Coefcode(AC_Coef,Size,index_AC);
                Run=0;
            }
        }
        //EOB
        //if(last!=63){
            Run=0;
            Size=0;
            AC_Coefficient(Run,Size,index_AC);
            AC_Coefcode(AC_Coef,Size,index_AC);
      //  }

    }
    else{//EOB
        Run=0;
        Size=0;
        AC_Coefficient(Run,Size,index_AC);
        AC_Coefcode(AC_Coef,Size,index_AC);
    }
   /*
    while(!EOB){
        Run=0;
       // Size=0;
        for(i = temp ; i < 64 &&!EOB ; i++){
            if(AC_arr[i]==0){
                Run++;
            }
            else{
                Size=getCategoryOfDCTCoefficient(AC_arr[i]);
                temp=i+1;
                AC_Coef=AC_arr[i];
                break;
            }
            if(Run==15&&AC_arr[i+1]==0){//if ZRL
                Size=0;
                for(int j = i+2 ; j < 64 &&!ZRL ; j++){
                    if(AC_arr[j]!=0){
                        ZRL=true;
                        temp=i+2;
                        break;
                    }
                }
                if(!ZRL){  //EOB
                    Run=0;
                    EOB=true;
                }
                AC_Coef=0;
            }

        }

        AC_Coefficient(Run,Size,index_AC);
        AC_Coefcode(AC_Coef,Size,index_AC);

        if(i==64){EOB=true;}

    }
    */
    index_AC++;
}

/******change binary to unsigned long******/

unsigned long chars2ul(char* x){
    unsigned long ret = 0;
    for(int i = 0 ; i < 8 ; i++){
        if(x[i]=='1')
           ret+=1<<(7-i);
    }
    return ret;


}

int str2val(string x){
    int leng=x.length();
    int sum=0;

    if(x[0]=='1'){
        if(leng>1){
            for(int i = 0 ; i < leng ; i++){
                if(x[i]=='1'){
                    sum+=Power(2,leng-1-i);
                }
            }
        }
        else{
            sum=1;
        }

    }
    else if(x[0]=='0'){
        if(leng>1){
           for(int i = 1 ; i < leng ; i++){
                if(x[i]=='1'){
                    sum+=Power(2,leng-1-i);
                }
            }
            sum-=(Power(2,leng)-1);
        }
        else{
            sum=-1;
        }

    }
    else{err_cnt++;cout<<"WTF:Xval\n";}

    return sum;
}

void str2DC(string x,int DC_idx){
    string tmpstr="";
    string dcstr="";
    int temp=0;

    if(x[0]=='1'){//1
        if(x[1]=='1'){//11
            if(x[2]=='1'){//111
                if(x[3]=='1'){//1111
                    if(x[4]=='1'){//11111
                        if(x[5]=='1'){//111111
                            if(x[6]=='1'){//1111111
                                if(x[7]=='1'){//11111111
                                    if(x[8]=='1'){//111111111
                                        err_cnt++;cout<<"WTF111111111\n";
                                    }
                                    else if(x[8]=='0'){//1111111110(DC_11)
                                        for(int i = 9 ; i < 20 ; i++){tmpstr+=x[i];temp=i+1;}
                                      //  dcstr+=tmpstr;
                                        deDCval[DC_idx]=str2val(tmpstr);
                                    }
                                    else{err_cnt++;cout<<"WTF:11111111X\n";}
                                }
                                else if(x[7]=='0'){//11111110(DC_10)
                                    for(int i = 8 ; i < 18 ; i++){tmpstr+=x[i];temp=i+1;}
                                  //  dcstr+=tmpstr;
                                    deDCval[DC_idx]=str2val(tmpstr);
                                }
                                else{err_cnt++;cout<<"WTF:1111111X\n";}
                            }
                            else if(x[6]=='0'){//1111110(DC_9)
                                for(int i = 7 ; i < 16 ; i++){tmpstr+=x[i];temp=i+1;}
                              //  dcstr+=tmpstr;
                                deDCval[DC_idx]=str2val(tmpstr);
                            }
                            else{err_cnt++;cout<<"WTF:111111X\n";}
                        }
                        else if(x[5]=='0'){//111110(DC_8)
                            for(int i = 6 ; i < 14 ; i++){tmpstr+=x[i];temp=i+1;}
                          //  dcstr+=tmpstr;
                            deDCval[DC_idx]=str2val(tmpstr);
                        }
                        else{err_cnt++;cout<<"WTF:11111X\n";}
                    }
                    else if(x[4]=='0'){//11110(DC_7)
                        for(int i = 5 ; i < 12 ; i++){tmpstr+=x[i];temp=i+1;}
                    //    dcstr+=tmpstr;
                        deDCval[DC_idx]=str2val(tmpstr);
                    }
                    else{err_cnt++;cout<<"WTF:1111X\n";}
                }
                else if(x[3]=='0'){//1110(DC_6)
                    for(int i = 4 ; i < 10 ; i++){tmpstr+=x[i];temp=i+1;}
                  //  dcstr+=tmpstr;
                    deDCval[DC_idx]=str2val(tmpstr);
                }
                else{err_cnt++;cout<<"WTF:111X\n";}
            }
            else if(x[2]=='0'){//110(DC_5)
                for(int i = 3 ; i < 8 ; i++){tmpstr+=x[i];temp=i+1;}
               // dcstr+=tmpstr;
                deDCval[DC_idx]=str2val(tmpstr);
            }
            else{err_cnt++;cout<<"WTF:11X\n";}
        }
        else if(x[1]=='0'){//10
            if(x[2]=='1'){//101(DC_4)
                for(int i = 3 ; i < 7 ; i++){tmpstr+=x[i];temp=i+1;}
               // dcstr+=tmpstr;
                deDCval[DC_idx]=str2val(tmpstr);
            }
            else if(x[2]=='0'){//100(DC_3)
                for(int i = 3 ; i < 6 ; i++){tmpstr+=x[i];temp=i+1;}
               // dcstr+=tmpstr;
                deDCval[DC_idx]=str2val(tmpstr);
            }
            else{err_cnt++;cout<<"WTF:1X\n";}
        }
    }
    else if(x[0]=='0'){//0
        if(x[1]=='1'){//01
            if(x[2]=='1'){//011(DC_2)
                for(int i = 3 ; i < 5 ; i++){tmpstr+=x[i];temp=i+1;}
               // dcstr+=tmpstr;
                deDCval[DC_idx]=str2val(tmpstr);
            }
            else if(x[2]=='0'){//010(DC_1)
                for(int i = 3 ; i < 4 ; i++){tmpstr+=x[i];temp=i+1;}
                /*if(x[3]=='1'){
                    deDCval[DC_idx]=1;
                    //dcstr+=x[3];
                }
                else if(x[3]=='0'){
                    deDCval[DC_idx]=-1;
                   // dcstr+=x[3];
                }
                else{err_cnt++;cout<<"WTF:010X\n";}
                temp=4;*/
                deDCval[DC_idx]=str2val(tmpstr);
            }
            else{err_cnt++;cout<<"WTF:01X\n";}
        }
        else if(x[1]=='0'){//00(DC_0)
            deDCval[DC_idx]=0;
            temp=2;
        }
        else{err_cnt++;cout<<"WTF:0X\n";}
    }
    else{err_cnt++;cout<<"WTF:X\n";}

    for(int i = temp ; i < x.length() ; i++){
        dcstr+=x[i];
    }
    instr=dcstr;
    //deDC[DC_idx]+=dcstr;

    idx_DC++;
    //cout<<"idx_DC = "<<idx_DC<<endl;
   // cout<<"instr.length() = "<<instr.length()<<endl;
}

void str2AC(string x,int AC_idx){
    bool EOB=false;
    int AC_idx2=0;//<129

    while(!EOB){
        string tmpstr="";
        string acstr="";
        int temp=0;

        if(x[0]=='1'){//1
            if(x[1]=='1'){//11
                if(x[2]=='1'){//111
                    if(x[3]=='1'){//1111
                        if(x[4]=='1'){//11111
                            if(x[5]=='1'){//111111
                                if(x[6]=='1'){//1111111
                                    if(x[7]=='1'){//11111111
                                        if(x[8]=='1'){//111111111
                                            if(x[9]=='1'){//1111111111
                                                if(x[10]=='1'){//11111111111
                                                    if(x[11]=='1'){//111111111111
                                                        if(x[12]=='1'){//1111111111111
                                                            if(x[13]=='1'){//11111111111111
                                                                if(x[14]=='1'){//111111111111111
                                                                    if(x[15]=='1'){//1111111111111111
                                                                        err_cnt++;cout<<"SHIT:1111111111111111\n";
                                                                    }
                                                                    else if(x[15]=='0'){//1111111111111110 (Run:15,Size:10)
                                                                        for(int i = 16 ; i < 26 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=15;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111111111X\n";}
                                                                }
                                                                else if(x[14]=='0'){//111111111111110
                                                                    if(x[15]=='1'){//1111111111111101 (Run:15,Size:9)
                                                                        for(int i = 16 ; i < 25 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=15;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111111111100 (Run:15,Size:8)
                                                                        for(int i = 16 ; i < 24 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=15;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111111110X\n";}
                                                                }
                                                                else{err_cnt++;cout<<"SHIT:11111111111111X\n";}
                                                            }
                                                            else if(x[13]=='0'){//11111111111110
                                                                if(x[14]=='1'){//111111111111101
                                                                    if(x[15]=='1'){//1111111111111011 (Run:15,Size:7)
                                                                        for(int i = 16 ; i < 23 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=15;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111111111010 (Run:15,Size:6)
                                                                        for(int i = 16 ; i < 22 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=15;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111111101X\n";}
                                                                }
                                                                else if(x[14]=='0'){//111111111111100
                                                                    if(x[15]=='1'){//1111111111111001 (Run:15,Size:5)
                                                                        for(int i = 16 ; i < 21 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=15;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111111111000 (Run:15,Size:4)
                                                                        for(int i = 16 ; i < 20 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=15;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111111100X\n";}
                                                                }
                                                                else{err_cnt++;cout<<"SHIT:11111111111110X\n";}
                                                            }
                                                            else{err_cnt++;cout<<"SHIT:1111111111111X\n";}
                                                        }
                                                        else if(x[12]=='0'){//1111111111110
                                                            if(x[13]=='1'){//11111111111101
                                                                if(x[14]=='1'){//111111111111011
                                                                    if(x[15]=='1'){//1111111111110111 (Run:15,Size:3)
                                                                        for(int i = 16 ; i < 19 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=15;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111111110110 (Run:15,Size:2)
                                                                        for(int i = 16 ; i < 18 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=15;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111111011X\n";}
                                                                }
                                                                else if(x[14]=='0'){//111111111111010
                                                                    if(x[15]=='1'){//1111111111110101 (Run:15,Size:1)
                                                                        for(int i = 16 ; i < 17 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=15;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111111110100 (Run:14,Size:10)
                                                                        for(int i = 16 ; i < 26 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=14;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111111010X\n";}
                                                                }
                                                                else{err_cnt++;cout<<"SHIT:11111111111101X\n";}
                                                            }
                                                            else if(x[13]=='0'){//11111111111100
                                                                if(x[14]=='1'){//111111111111001
                                                                    if(x[15]=='1'){//1111111111110011 (Run:14,Size:9)
                                                                        for(int i = 16 ; i < 25 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=14;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111111110010 (Run:14,Size:8)
                                                                        for(int i = 16 ; i < 24 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=14;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111111001X\n";}
                                                                }
                                                                else if(x[14]=='0'){//111111111111000
                                                                    if(x[15]=='1'){//1111111111110001 (Run:14,Size:7)
                                                                        for(int i = 16 ; i < 23 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=14;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111111110000 (Run:14,Size:6)
                                                                        for(int i = 16 ; i < 22 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=14;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111111000X\n";}
                                                                }
                                                                else{err_cnt++;cout<<"SHIT:11111111111100X\n";}
                                                            }
                                                            else{err_cnt++;cout<<"SHIT:1111111111110X\n";}
                                                        }
                                                        else{err_cnt++;cout<<"SHIT:111111111111X\n";}
                                                    }
                                                    else if(x[11]=='0'){//111111111110
                                                        if(x[12]=='1'){//1111111111101
                                                            if(x[13]=='1'){//11111111111011
                                                                if(x[14]=='1'){//111111111110111
                                                                    if(x[15]=='1'){//1111111111101111 (Run:14,Size:5)
                                                                        for(int i = 16 ; i < 21 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=14;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111111101110 (Run:14,Size:4)
                                                                        for(int i = 16 ; i < 20 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=14;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:11111111111011X\n";}
                                                                }
                                                                else if(x[14]=='0'){//111111111110110
                                                                    if(x[15]=='1'){//1111111111101101 (Run:14,Size:3)
                                                                        for(int i = 16 ; i < 19 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=14;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111111101100 (Run:14,Size:2)
                                                                        for(int i = 16 ; i < 18 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=14;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111110110X\n";}
                                                                }
                                                                else{err_cnt++;cout<<"SHIT:11111111111011X\n";}
                                                            }
                                                            else if(x[13]=='0'){//11111111111010
                                                                if(x[14]=='1'){//111111111110101
                                                                    if(x[15]=='1'){//1111111111101011 (Run:14,Size:1)
                                                                        for(int i = 16 ; i < 17 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=14;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111111101010 (Run:13,Size:10)
                                                                        for(int i = 16 ; i < 26 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=13;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111110101X\n";}
                                                                }
                                                                else if(x[14]=='0'){//111111111110100
                                                                    if(x[15]=='1'){//1111111111101001 (Run:13,Size:9)
                                                                        for(int i = 16 ; i < 25 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=13;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111111101000 (Run:13,Size:8)
                                                                        for(int i = 16 ; i < 24 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=13;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:11111111111010X\n";}
                                                                }
                                                                else{err_cnt++;cout<<"SHIT:11111111111010X\n";}
                                                            }
                                                            else{err_cnt++;cout<<"SHIT:1111111111101X\n";}
                                                        }
                                                        else if(x[12]=='0'){//1111111111100
                                                            if(x[13]=='1'){//11111111111001
                                                                if(x[14]=='1'){//111111111110011
                                                                    if(x[15]=='1'){//1111111111100111 (Run:13,Size:7)
                                                                        for(int i = 16 ; i < 23 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=13;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111111100110 (Run:13,Size:6)
                                                                        for(int i = 16 ; i < 22 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=13;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111110011X\n";}
                                                                }
                                                                else if(x[14]=='0'){//111111111110010
                                                                    if(x[15]=='1'){//1111111111100101 (Run:13,Size:5)
                                                                        for(int i = 16 ; i < 21 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=13;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111111100100 (Run:13,Size:4)
                                                                        for(int i = 16 ; i < 20 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=13;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111110010X\n";}
                                                                }
                                                                else{err_cnt++;cout<<"SHIT:11111111111001X\n";}
                                                            }
                                                            else if(x[13]=='0'){//11111111111000
                                                                if(x[14]=='1'){//111111111110001
                                                                    if(x[15]=='1'){//1111111111100011 (Run:13,Size:3)
                                                                        for(int i = 16 ; i < 19 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=13;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111111100010 (Run:13,Size:2)
                                                                        for(int i = 16 ; i < 18 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=13;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111110001X\n";}
                                                                }
                                                                else if(x[14]=='0'){//111111111110000
                                                                    if(x[15]=='1'){//1111111111100001 (Run:12,Size:10)
                                                                        for(int i = 16 ; i < 26 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=12;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111111100000 (Run:12,Size:9)
                                                                        for(int i = 16 ; i < 25 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=12;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111110000X\n";}
                                                                }
                                                                else{err_cnt++;cout<<"SHIT:11111111111000X\n";}
                                                            }
                                                            else{err_cnt++;cout<<"SHIT:1111111111100X\n";}
                                                        }
                                                        else{err_cnt++;cout<<"SHIT:111111111110X\n";}
                                                    }
                                                    else{err_cnt++;cout<<"SHIT:11111111111X\n";}
                                                }
                                                else if(x[10]=='0'){//11111111110
                                                    if(x[11]=='1'){//111111111101
                                                        if(x[12]=='1'){//1111111111011
                                                            if(x[13]=='1'){//11111111110111
                                                                if(x[14]=='1'){//111111111101111
                                                                    if(x[15]=='1'){//1111111111011111 (Run:12,Size:8)
                                                                        for(int i = 16 ; i < 24 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=12;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111111011110 (Run:12,Size:7)
                                                                        for(int i = 16 ; i < 23 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=12;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111101111X\n";}
                                                                }
                                                                else if(x[14]=='0'){//111111111101110
                                                                    if(x[15]=='1'){//1111111111011101 (Run:12,Size:6)
                                                                        for(int i = 16 ; i < 22 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=12;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111111011100 (Run:12,Size:5)
                                                                        for(int i = 16 ; i < 21 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=12;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111101110X\n";}
                                                                }
                                                                else{err_cnt++;cout<<"SHIT:11111111110111X\n";}
                                                            }
                                                            else if(x[13]=='0'){//11111111110110
                                                                if(x[14]=='1'){//111111111101101
                                                                    if(x[15]=='1'){//1111111111011011 (Run:12,Size:4)
                                                                        for(int i = 16 ; i < 20 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=12;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111111011010 (Run:12,Size:3)
                                                                        for(int i = 16 ; i < 19 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=12;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111101101X\n";}
                                                                }
                                                                else if(x[14]=='0'){//111111111101100
                                                                    if(x[15]=='1'){//1111111111011001 (Run:12,Size:2)
                                                                        for(int i = 16 ; i < 18 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=12;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111111011000 (Run:11,Size:10)
                                                                        for(int i = 16 ; i < 26 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=11;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111101100X\n";}
                                                                }
                                                                else{err_cnt++;cout<<"SHIT:11111111110110X\n";}
                                                            }
                                                            else{err_cnt++;cout<<"SHIT:1111111111011X\n";}
                                                        }
                                                        else if(x[12]=='0'){//1111111111010
                                                            if(x[13]=='1'){//11111111110101
                                                                if(x[14]=='1'){//111111111101011
                                                                    if(x[15]=='1'){//1111111111010111 (Run:11,Size:9)
                                                                        for(int i = 16 ; i < 25 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=11;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111111010110 (Run:11,Size:8)
                                                                        for(int i = 16 ; i < 24 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=11;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111101011X\n";}
                                                                }
                                                                else if(x[14]=='0'){//111111111101010
                                                                    if(x[15]=='1'){//1111111111010101 (Run:11,Size:7)
                                                                        for(int i = 16 ; i < 23 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=11;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111111010100 (Run:11,Size:6)
                                                                        for(int i = 16 ; i < 22 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=11;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111101010X\n";}
                                                                }
                                                                else{err_cnt++;cout<<"SHIT:11111111110101X\n";}
                                                            }
                                                            else if(x[13]=='0'){//11111111110100
                                                                if(x[14]=='1'){//111111111101001
                                                                    if(x[15]=='1'){//1111111111010011 (Run:11,Size:5)
                                                                        for(int i = 16 ; i < 21 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=11;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111111010010 (Run:11,Size:4)
                                                                        for(int i = 16 ; i < 20 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=11;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111101001X\n";}
                                                                }
                                                                else if(x[14]=='0'){//111111111101000
                                                                    if(x[15]=='1'){//1111111111010001 (Run:11,Size:3)
                                                                        for(int i = 16 ; i < 19 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=11;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111111010000 (Run:11,Size:2)
                                                                        for(int i = 16 ; i < 18 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=11;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111101000X\n";}
                                                                }
                                                                else{err_cnt++;cout<<"SHIT:11111111110100X\n";}
                                                            }
                                                            else{err_cnt++;cout<<"SHIT:1111111111010X\n";}
                                                        }
                                                        else{err_cnt++;cout<<"SHIT:111111111101X\n";}
/*****************************************************************************************************************************************************/
                                                    }
                                                    else if(x[11]=='0'){//111111111100
                                                        if(x[12]=='1'){//1111111111001
                                                            if(x[13]=='1'){//11111111110011
                                                                if(x[14]=='1'){//111111111100111
                                                                    if(x[15]=='1'){//1111111111001111 (Run:10,Size:10)
                                                                        for(int i = 16 ; i < 26 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=10;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111111001110 (Run:10,Size:9)
                                                                        for(int i = 16 ; i < 25 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=10;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111100111X\n";}
                                                                }
                                                                else if(x[14]=='0'){//111111111100110
                                                                    if(x[15]=='1'){//1111111111001101 (Run:10,Size:8)
                                                                        for(int i = 16 ; i < 24 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=10;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111111001100 (Run:10,Size:7)
                                                                        for(int i = 16 ; i < 23 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=10;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111100110X\n";}
                                                                }
                                                                else{err_cnt++;cout<<"SHIT:11111111110011X\n";}
                                                            }
                                                            else if(x[13]=='0'){//11111111110010
                                                                if(x[14]=='1'){//111111111100101
                                                                    if(x[15]=='1'){//1111111111001011 (Run:10,Size:6)
                                                                        for(int i = 16 ; i < 22 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=10;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111111001010 (Run:10,Size:5)
                                                                        for(int i = 16 ; i < 21 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=10;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111100101X\n";}
                                                                }
                                                                else if(x[14]=='0'){//111111111100100
                                                                    if(x[15]=='1'){//1111111111001001 (Run:10,Size:4)
                                                                        for(int i = 16 ; i < 20 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=10;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111111001000 (Run:10,Size:3)
                                                                        for(int i = 16 ; i < 19 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=10;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111100100X\n";}
                                                                }
                                                                else{err_cnt++;cout<<"SHIT:11111111110010X\n";}
                                                            }
                                                            else{err_cnt++;cout<<"SHIT:1111111111001X\n";}
                                                        }
                                                        else if(x[12]=='0'){//1111111111000
                                                            if(x[13]=='1'){//11111111110001
                                                                if(x[14]=='1'){//111111111100011
                                                                    if(x[15]=='1'){//1111111111000111 (Run:10,Size:2)
                                                                        for(int i = 16 ; i < 18 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=10;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111111000110 (Run:9,Size:10)
                                                                        for(int i = 16 ; i < 26 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=9;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111100011X\n";}
                                                                }
                                                                else if(x[14]=='0'){//111111111100010
                                                                    if(x[15]=='1'){//1111111111000101 (Run:9,Size:9)
                                                                        for(int i = 16 ; i < 25 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=9;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111111000100 (Run:9,Size:8)
                                                                        for(int i = 16 ; i < 24 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=9;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111100010X\n";}
                                                                }
                                                                else{err_cnt++;cout<<"SHIT:11111111110001X\n";}
                                                            }
                                                            else if(x[13]=='0'){//11111111110000
                                                                if(x[14]=='1'){//111111111100001
                                                                    if(x[15]=='1'){//1111111111000011 (Run:9,Size:7)
                                                                        for(int i = 16 ; i < 23 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=9;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111111000010 (Run:9,Size:6)
                                                                        for(int i = 16 ; i < 22 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=9;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111100001X\n";}
                                                                }
                                                                else if(x[14]=='0'){//111111111100000
                                                                    if(x[15]=='1'){//1111111111000001 (Run:9,Size:5)
                                                                        for(int i = 16 ; i < 21 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=9;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111111000000 (Run:9,Size:4)
                                                                        for(int i = 16 ; i < 20 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=9;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111100000X\n";}
                                                                }
                                                                else{err_cnt++;cout<<"SHIT:11111111110000X\n";}
                                                            }
                                                            else{err_cnt++;cout<<"SHIT:1111111111000X\n";}
                                                        }
                                                        else{err_cnt++;cout<<"SHIT:111111111100X\n";}
                                                    }
                                                    else{err_cnt++;cout<<"SHIT:11111111110X\n";}
                                                }
                                                else{err_cnt++;cout<<"SHIT:1111111111X\n";}
                                            }
                                            else if(x[9]=='0'){//1111111110
                                                if(x[10]=='1'){//11111111101
                                                    if(x[11]=='1'){//111111111011
                                                        if(x[12]=='1'){//1111111110111
                                                            if(x[13]=='1'){//11111111101111
                                                                if(x[14]=='1'){//111111111011111
                                                                    if(x[15]=='1'){//1111111110111111 (Run:9,Size:3)
                                                                        for(int i = 16 ; i < 19 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=9;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111110111110 (Run:9,Size:2)
                                                                        for(int i = 16 ; i < 18 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=9;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111011111X\n";}
                                                                }
                                                                else if(x[14]=='0'){//111111111011110
                                                                    if(x[15]=='1'){//1111111110111101 (Run:8,Size:10)
                                                                        for(int i = 16 ; i < 26 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=8;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111110111100 (Run:8,Size:9)
                                                                        for(int i = 16 ; i < 25 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=8;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111011110X\n";}
                                                                }
                                                                else{err_cnt++;cout<<"SHIT:11111111101111X\n";}
                                                            }
                                                            else if(x[13]=='0'){//11111111101110
                                                                if(x[14]=='1'){//111111111011101
                                                                    if(x[15]=='1'){//1111111110111011 (Run:8,Size:8)
                                                                        for(int i = 16 ; i < 24 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=8;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111110111010 (Run:8,Size:7)
                                                                        for(int i = 16 ; i < 23 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=8;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111011101X\n";}
                                                                }
                                                                else if(x[14]=='0'){//111111111011100
                                                                    if(x[15]=='1'){//1111111110111001 (Run:8,Size:6)
                                                                        for(int i = 16 ; i < 22 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=8;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111110111000 (Run:8,Size:5)
                                                                        for(int i = 16 ; i < 21 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=8;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111011100X\n";}
                                                                }
                                                                else{err_cnt++;cout<<"SHIT:11111111101110X\n";}
                                                            }
                                                            else{err_cnt++;cout<<"SHIT:1111111110111X\n";}
                                                        }
                                                        else if(x[12]=='0'){//1111111110110
                                                            if(x[13]=='1'){//11111111101101
                                                                if(x[14]=='1'){//111111111011011
                                                                    if(x[15]=='1'){//1111111110110111 (Run:8,Size:4)
                                                                        for(int i = 16 ; i < 20 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=8;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111110110110 (Run:8,Size:3)
                                                                        for(int i = 16 ; i < 19 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=8;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111011011X\n";}
                                                                }
                                                                else if(x[14]=='0'){//111111111011010
                                                                    if(x[15]=='1'){//1111111110110101 (Run:7,Size:10)
                                                                        for(int i = 16 ; i < 26 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=7;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111110110100 (Run:7,Size:9)
                                                                        for(int i = 16 ; i < 25 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=7;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111011010X\n";}
                                                                }
                                                                else{err_cnt++;cout<<"SHIT:11111111101101X\n";}
                                                            }
                                                            else if(x[13]=='0'){//11111111101100
                                                                if(x[14]=='1'){//111111111011001
                                                                    if(x[15]=='1'){//1111111110110011 (Run:7,Size:8)
                                                                        for(int i = 16 ; i < 24 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=7;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111110110010 (Run:7,Size:7)
                                                                        for(int i = 16 ; i < 23 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=7;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111011001X\n";}
                                                                }
                                                                else if(x[14]=='0'){//111111111011000
                                                                    if(x[15]=='1'){//1111111110110001 (Run:7,Size:6)
                                                                        for(int i = 16 ; i < 22 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=7;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111110110000 (Run:7,Size:5)
                                                                        for(int i = 16 ; i < 21 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=7;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111011000X\n";}
                                                                }
                                                                else{err_cnt++;cout<<"SHIT:11111111101100X\n";}
                                                            }
                                                            else{err_cnt++;cout<<"SHIT:1111111110110X\n";}
                                                        }
                                                        else{err_cnt++;cout<<"SHIT:111111111011X\n";}
                                                    }
                                                    else if(x[11]=='0'){//111111111010
                                                        if(x[12]=='1'){//1111111110101
                                                            if(x[13]=='1'){//11111111101011
                                                                if(x[14]=='1'){//111111111010111
                                                                    if(x[15]=='1'){//1111111110101111 (Run:7,Size:4)
                                                                        for(int i = 16 ; i < 20 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=7;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111110101110 (Run:7,Size:3)
                                                                        for(int i = 16 ; i < 19 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=7;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111010111X\n";}
                                                                }
                                                                else if(x[14]=='0'){//111111111010110
                                                                    if(x[15]=='1'){//1111111110101101 (Run:6,Size:10)
                                                                        for(int i = 16 ; i < 26 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=6;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111110101100 (Run:6,Size:9)
                                                                        for(int i = 16 ; i < 25 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=6;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111010110X\n";}
                                                                }
                                                                else{err_cnt++;cout<<"SHIT:11111111101011X\n";}
                                                            }
                                                            else if(x[13]=='0'){//11111111101010
                                                                if(x[14]=='1'){//111111111010101
                                                                    if(x[15]=='1'){//1111111110101011 (Run:6,Size:8)
                                                                        for(int i = 16 ; i < 24 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=6;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111110101010 (Run:6,Size:7)
                                                                        for(int i = 16 ; i < 23 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=6;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111010101X\n";}
                                                                }
                                                                else if(x[14]=='0'){//111111111010100
                                                                    if(x[15]=='1'){//1111111110101001 (Run:6,Size:6)
                                                                        for(int i = 16 ; i < 22 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=6;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111110101000 (Run:6,Size:5)
                                                                        for(int i = 16 ; i < 21 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=6;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111010100X\n";}
                                                                }
                                                                else{err_cnt++;cout<<"SHIT:11111111101010X\n";}
                                                            }
                                                            else{err_cnt++;cout<<"SHIT:1111111110101X\n";}
                                                        }
                                                        else if(x[12]=='0'){//1111111110100
                                                            if(x[13]=='1'){//11111111101001
                                                                if(x[14]=='1'){//111111111010011
                                                                    if(x[15]=='1'){//1111111110100111 (Run:6,Size:4)
                                                                        for(int i = 16 ; i < 20 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=6;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111110100110 (Run:6,Size:3)
                                                                        for(int i = 16 ; i < 19 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=6;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111010011X\n";}
                                                                }
                                                                else if(x[14]=='0'){//111111111010010
                                                                    if(x[15]=='1'){//1111111110100101 (Run:5,Size:10)
                                                                        for(int i = 16 ; i < 26 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=5;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111110100100 (Run:5,Size:9)
                                                                        for(int i = 16 ; i < 25 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=5;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111010010X\n";}
                                                                }
                                                                else{err_cnt++;cout<<"SHIT:11111111101001X\n";}
                                                            }
                                                            else if(x[13]=='0'){//11111111101000
                                                                if(x[14]=='1'){//111111111010001
                                                                    if(x[15]=='1'){//1111111110100011 (Run:5,Size:8)
                                                                        for(int i = 16 ; i < 24 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=5;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111110100010 (Run:5,Size:7)
                                                                        for(int i = 16 ; i < 23 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=5;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111010001X\n";}
                                                                }
                                                                else if(x[14]=='0'){//111111111010000
                                                                    if(x[15]=='1'){//1111111110100001 (Run:5,Size:6)
                                                                        for(int i = 16 ; i < 22 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=5;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111110100000 (Run:5,Size:5)
                                                                        for(int i = 16 ; i < 21 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=5;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111010000X\n";}
                                                                }
                                                                else{err_cnt++;cout<<"SHIT:11111111101000X\n";}
                                                            }
                                                            else{err_cnt++;cout<<"SHIT:1111111110100X\n";}
                                                        }
                                                        else{err_cnt++;cout<<"SHIT:111111111010X\n";}
                                                    }
                                                    else{err_cnt++;cout<<"SHIT:11111111101X\n";}
                                                }
                                                else if(x[10]=='0'){//11111111100
                                                    if(x[11]=='1'){//111111111001
                                                        if(x[12]=='1'){//1111111110011
                                                            if(x[13]=='1'){//11111111100111
                                                                if(x[14]=='1'){//111111111001111
                                                                    if(x[15]=='1'){//1111111110011111 (Run:5,Size:4)
                                                                        for(int i = 16 ; i < 20 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=5;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111110011110 (Run:5,Size:3)
                                                                        for(int i = 16 ; i < 19 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=5;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111001111X\n";}
                                                                }
                                                                else if(x[14]=='0'){//111111111001110
                                                                    if(x[15]=='1'){//1111111110011101 (Run:4,Size:10)
                                                                        for(int i = 16 ; i < 26 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=4;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111110011100 (Run:4,Size:9)
                                                                        for(int i = 16 ; i < 25 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=4;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111001110X\n";}
                                                                }
                                                                else{err_cnt++;cout<<"SHIT:11111111100111X\n";}
                                                            }
                                                            else if(x[13]=='0'){//11111111100110
                                                                if(x[14]=='1'){//111111111001101
                                                                    if(x[15]=='1'){//1111111110011011 (Run:4,Size:8)
                                                                        for(int i = 16 ; i < 24 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=4;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111110011010 (Run:4,Size:7)
                                                                        for(int i = 16 ; i < 23 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=4;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111001101X\n";}
                                                                }
                                                                else if(x[14]=='0'){//111111111001100
                                                                    if(x[15]=='1'){//1111111110011001 (Run:4,Size:6)
                                                                        for(int i = 16 ; i < 22 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=4;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111110011000 (Run:4,Size:5)
                                                                        for(int i = 16 ; i < 21 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=4;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111001100X\n";}
                                                                }
                                                                else{err_cnt++;cout<<"SHIT:11111111100110X\n";}
                                                            }
                                                            else{err_cnt++;cout<<"SHIT:1111111110011X\n";}
                                                        }
                                                        else if(x[12]=='0'){//1111111110010
                                                            if(x[13]=='1'){//11111111100101
                                                                if(x[14]=='1'){//111111111001011
                                                                    if(x[15]=='1'){//1111111110010111 (Run:4,Size:4)
                                                                        for(int i = 16 ; i < 20 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=4;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111110010110 (Run:4,Size:3)
                                                                        for(int i = 16 ; i < 19 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=4;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111001011X\n";}
                                                                }
                                                                else if(x[14]=='0'){//111111111001010
                                                                    if(x[15]=='1'){//1111111110010101 (Run:3,Size:10)
                                                                        for(int i = 16 ; i < 26 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=3;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111110010100 (Run:3,Size:9)
                                                                        for(int i = 16 ; i < 25 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=3;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111001010X\n";}
                                                                }
                                                                else{err_cnt++;cout<<"SHIT:11111111100101X\n";}
                                                            }
                                                            else if(x[13]=='0'){//11111111100100
                                                                if(x[14]=='1'){//111111111001001
                                                                    if(x[15]=='1'){//1111111110010011 (Run:3,Size:8)
                                                                        for(int i = 16 ; i < 24 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=3;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111110010010 (Run:3,Size:7)
                                                                        for(int i = 16 ; i < 23 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=3;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111001001X\n";}
                                                                }
                                                                else if(x[14]=='0'){//111111111001000
                                                                    if(x[15]=='1'){//1111111110010001 (Run:3,Size:6)
                                                                        for(int i = 16 ; i < 22 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=3;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111110010000 (Run:3,Size:5)
                                                                        for(int i = 16 ; i < 21 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=3;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111001000X\n";}
                                                                }
                                                                else{err_cnt++;cout<<"SHIT:11111111100100X\n";}
                                                            }
                                                            else{err_cnt++;cout<<"SHIT:1111111110010X\n";}
                                                        }
                                                        else{err_cnt++;cout<<"SHIT:111111111001X\n";}
/*****************************************************************************************************************************************************/
                                                    }
                                                    else if(x[11]=='0'){//111111111000
                                                        if(x[12]=='1'){//1111111110001
                                                            if(x[13]=='1'){//11111111100011
                                                                if(x[14]=='1'){//111111111000111
                                                                    if(x[15]=='1'){//1111111110001111 (Run:3,Size:4)
                                                                        for(int i = 16 ; i < 20 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=3;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111110001110 (Run:2,Size:10)
                                                                        for(int i = 16 ; i < 26 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=2;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111000111X\n";}
                                                                }
                                                                else if(x[14]=='0'){//111111111000110
                                                                    if(x[15]=='1'){//1111111110001101 (Run:2,Size:9)
                                                                        for(int i = 16 ; i < 25 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=2;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111110001100 (Run:2,Size:8)
                                                                        for(int i = 16 ; i < 24 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=2;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111000110X\n";}
                                                                }
                                                                else{err_cnt++;cout<<"SHIT:11111111100011X\n";}
                                                            }
                                                            else if(x[13]=='0'){//11111111100010
                                                                if(x[14]=='1'){//111111111000101
                                                                    if(x[15]=='1'){//1111111110001011 (Run:2,Size:7)
                                                                        for(int i = 16 ; i < 23 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=2;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111110001010 (Run:2,Size:6)
                                                                        for(int i = 16 ; i < 22 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=2;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111000101X\n";}
                                                                }
                                                                else if(x[14]=='0'){//111111111000100
                                                                    if(x[15]=='1'){//1111111110001001 (Run:2,Size:5)
                                                                        for(int i = 16 ; i < 21 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=2;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111110001000 (Run:1,Size:10)
                                                                        for(int i = 16 ; i < 26 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=1;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111000100X\n";}
                                                                }
                                                                else{err_cnt++;cout<<"SHIT:11111111100010X\n";}
                                                            }
                                                            else{err_cnt++;cout<<"SHIT:1111111110001X\n";}
                                                        }
                                                        else if(x[12]=='0'){//1111111110000
                                                            if(x[13]=='1'){//11111111100001
                                                                if(x[14]=='1'){//111111111000011
                                                                    if(x[15]=='1'){//1111111110000111 (Run:1,Size:9)
                                                                        for(int i = 16 ; i < 25 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=1;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111110000110 (Run:1,Size:8)
                                                                        for(int i = 16 ; i < 24 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=1;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111000011X\n";}
                                                                }
                                                                else if(x[14]=='0'){//111111111000010
                                                                    if(x[15]=='1'){//1111111110000101 (Run:1,Size:7)
                                                                        for(int i = 16 ; i < 23 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=1;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111110000100 (Run:1,Size:6)
                                                                        for(int i = 16 ; i < 22 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=1;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111000010X\n";}
                                                                }
                                                                else{err_cnt++;cout<<"SHIT:11111111100001X\n";}
                                                            }
                                                            else if(x[13]=='0'){//11111111100000
                                                                if(x[14]=='1'){//111111111000001
                                                                    if(x[15]=='1'){//1111111110000011 (Run:0,Size:10)
                                                                        for(int i = 16 ; i < 26 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=0;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else if(x[15]=='0'){//1111111110000010 (Run:0,Size:9)
                                                                        for(int i = 16 ; i < 25 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                        deACval[AC_idx][AC_idx2]=0;
                                                                        AC_idx2++;
                                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                        AC_idx2++;
                                                                    }
                                                                    else{err_cnt++;cout<<"SHIT:111111111000001X\n";}
                                                                }
                                                                else if(x[14]=='0'){//111111111000000 (Run:8,Size:2)
                                                                    for(int i = 15 ; i < 17 ; i++){tmpstr+=x[i];temp=i+1;}
                                                                    deACval[AC_idx][AC_idx2]=8;
                                                                    AC_idx2++;
                                                                    deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                                    AC_idx2++;
                                                                }
                                                                else{err_cnt++;cout<<"SHIT:11111111100000X\n";}
                                                            }
                                                            else{err_cnt++;cout<<"SHIT:1111111110000X\n";}
                                                        }
                                                        else{err_cnt++;cout<<"SHIT:111111111000X\n";}
                                                    }
                                                    else{err_cnt++;cout<<"SHIT:11111111100X\n";}
                                                }
                                                else{err_cnt++;cout<<"SHIT:1111111110X\n";}
                                            }
                                            else{err_cnt++;cout<<"SHIT:111111111X\n";}
                                        }
                                        else if(x[8]=='0'){//111111110
                                            if(x[9]=='1'){//1111111101
                                                if(x[10]=='1'){//11111111011
                                                    if(x[11]=='1'){//111111110111 (Run:7,Size:2)
                                                        for(int i = 12 ; i < 14 ; i++){tmpstr+=x[i];temp=i+1;}
                                                        deACval[AC_idx][AC_idx2]=7;
                                                        AC_idx2++;
                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                        AC_idx2++;
                                                    }
                                                    else if(x[11]=='0'){//111111110110 (Run:6,Size:2)
                                                        for(int i = 12 ; i < 14 ; i++){tmpstr+=x[i];temp=i+1;}
                                                        deACval[AC_idx][AC_idx2]=6;
                                                        AC_idx2++;
                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                        AC_idx2++;
                                                    }
                                                    else{err_cnt++;cout<<"SHIT:11111111011X\n";}
/*********************************************************************************************************************/
                                                }
                                                else if(x[10]=='0'){//11111111010
                                                    if(x[11]=='1'){//111111110101 (Run:3,Size:3)
                                                        for(int i = 12 ; i < 15 ; i++){tmpstr+=x[i];temp=i+1;}
                                                        deACval[AC_idx][AC_idx2]=3;
                                                        AC_idx2++;
                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                        AC_idx2++;
                                                    }
                                                    else if(x[11]=='0'){//111111110100 (Run:2,Size:4)
                                                        for(int i = 12 ; i < 16 ; i++){tmpstr+=x[i];temp=i+1;}
                                                        deACval[AC_idx][AC_idx2]=2;
                                                        AC_idx2++;
                                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                        AC_idx2++;
                                                    }
                                                    else{err_cnt++;cout<<"SHIT:11111111010X\n";}
                                                }
                                                else{err_cnt++;cout<<"SHIT:1111111101X\n";}
                                            }
/**************************************************************************************************************************/
                                            else if(x[9]=='0'){//1111111100
                                                if(x[10]=='1'){//11111111001 (ZRL) (Run:15,Size:0)#######
                                                    //for(int i = 11 ; i < 12 ; i++){tmpstr+=x[i];temp=i+1;}
                                                    temp=11;
                                                    deACval[AC_idx][AC_idx2]=15;
                                                    AC_idx2++;
                                                    deACval[AC_idx][AC_idx2]=0;
                                                    AC_idx2++;
                                                }
                                                else if(x[10]=='0'){//11111111000 (Run:13,Size:1)
                                                    for(int i = 11 ; i < 12 ; i++){tmpstr+=x[i];temp=i+1;}
                                                    deACval[AC_idx][AC_idx2]=13;
                                                    AC_idx2++;
                                                    deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                    AC_idx2++;
                                                }
                                                else{err_cnt++;cout<<"SHIT:1111111100X\n";}
                                            }
                                            else{err_cnt++;cout<<"SHIT:111111110X\n";}
                                        }
                                        else{err_cnt++;cout<<"SHIT:11111111X\n";}
                                    }
                                    else if(x[7]=='0'){//11111110
                                        if(x[8]=='1'){//111111101
                                            if(x[9]=='1'){//1111111011 (Run:,Size:)
                                                if(x[10]=='1'){//11111110111 (Run:5,Size:2)
                                                    for(int i = 11 ; i < 13 ; i++){tmpstr+=x[i];temp=i+1;}
                                                    deACval[AC_idx][AC_idx2]=5;
                                                    AC_idx2++;
                                                    deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                    AC_idx2++;
                                                }
                                                else if(x[10]=='0'){//11111110110 (Run:1,Size:5)
                                                    for(int i = 11 ; i < 16 ; i++){tmpstr+=x[i];temp=i+1;}
                                                    deACval[AC_idx][AC_idx2]=1;
                                                    AC_idx2++;
                                                    deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                    AC_idx2++;
                                                }
                                                else{err_cnt++;cout<<"SHIT:1111111010X\n";}
                                            }
                                            else if(x[9]=='0'){//1111111010 (Run:12,Size:1)
                                                for(int i = 10 ; i < 11 ; i++){tmpstr+=x[i];temp=i+1;}
                                                deACval[AC_idx][AC_idx2]=12;
                                                AC_idx2++;
                                                deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                AC_idx2++;
                                            }
                                            else{err_cnt++;cout<<"SHIT:111111101X\n";}
                                        }
                                        else if(x[8]=='0'){//111111100
                                            if(x[9]=='1'){//1111111001 (Run:11,Size:1)
                                                for(int i = 10 ; i < 11 ; i++){tmpstr+=x[i];temp=i+1;}
                                                deACval[AC_idx][AC_idx2]=11;
                                                AC_idx2++;
                                                deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                AC_idx2++;
                                            }
                                            else if(x[9]=='0'){//1111111000 (Run:4,Size:2)
                                                for(int i = 10 ; i < 12 ; i++){tmpstr+=x[i];temp=i+1;}
                                                deACval[AC_idx][AC_idx2]=4;
                                                AC_idx2++;
                                                deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                AC_idx2++;
                                            }
                                            else{err_cnt++;cout<<"SHIT:111111100X\n";}
                                        }
                                        else{err_cnt++;cout<<"SHIT:11111110X\n";}
                                    }
                                    else{err_cnt++;cout<<"SHIT:1111111X\n";}
                                }
                                else if(x[6]=='0'){//1111110
                                    if(x[7]=='1'){//11111101
                                        if(x[8]=='1'){//111111011
                                            if(x[9]=='1'){//1111110111 (Run:2,Size:3)
                                                for(int i = 10 ; i < 13 ; i++){tmpstr+=x[i];temp=i+1;}
                                                deACval[AC_idx][AC_idx2]=2;
                                                AC_idx2++;
                                                deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                AC_idx2++;
                                            }
                                            else if(x[9]=='0'){//1111110110 (Run:0,Size:8)
                                                for(int i = 10 ; i < 18 ; i++){tmpstr+=x[i];temp=i+1;}
                                                deACval[AC_idx][AC_idx2]=0;
                                                AC_idx2++;
                                                deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                                AC_idx2++;
                                            }
                                            else{err_cnt++;cout<<"SHIT:111111011X\n";}
                                        }
                                        else if(x[8]=='0'){//111111010 (Run:10,Size:1)
                                            for(int i = 9 ; i < 10 ; i++){tmpstr+=x[i];temp=i+1;}
                                            deACval[AC_idx][AC_idx2]=10;
                                            AC_idx2++;
                                            deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                            AC_idx2++;
                                        }
                                        else{err_cnt++;cout<<"SHIT:11111101X\n";}
                                    }
                                    else if(x[7]=='0'){//11111100
                                        if(x[8]=='1'){//111111001 (Run:9,Size:1)
                                            for(int i = 9 ; i < 10 ; i++){tmpstr+=x[i];temp=i+1;}
                                            deACval[AC_idx][AC_idx2]=9;
                                            AC_idx2++;
                                            deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                            AC_idx2++;
                                        }
                                        else if(x[8]=='0'){//111111000 (Run:8,Size:1)
                                            for(int i = 9 ; i < 10 ; i++){tmpstr+=x[i];temp=i+1;}
                                            deACval[AC_idx][AC_idx2]=8;
                                            AC_idx2++;
                                            deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                            AC_idx2++;
                                        }
                                        else{err_cnt++;cout<<"SHIT:11111100X\n";}
                                    }
                                    else{err_cnt++;cout<<"SHIT:1111110X\n";}
                                }
                                else{err_cnt++;cout<<"SHIT:111111X\n";}
                            }
                            else if(x[5]=='0'){//111110
                                if(x[6]=='1'){//1111101
                                    if(x[7]=='1'){//11111011
                                        if(x[8]=='1'){//111110111 (Run:3,Size:2)
                                            for(int i = 9 ; i < 11 ; i++){tmpstr+=x[i];temp=i+1;}
                                            deACval[AC_idx][AC_idx2]=3;
                                            AC_idx2++;
                                            deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                            AC_idx2++;
                                        }
                                        else if(x[8]=='0'){//111110110 (Run:1,Size:4)
                                            for(int i = 9 ; i < 13 ; i++){tmpstr+=x[i];temp=i+1;}
                                            deACval[AC_idx][AC_idx2]=1;
                                            AC_idx2++;
                                            deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                            AC_idx2++;
                                        }
                                        else{err_cnt++;cout<<"SHIT:11111011X\n";}
                                    }
                                    else if(x[7]=='0'){//11111010 (Run:7,Size:1)
                                        for(int i = 8 ; i < 9 ; i++){tmpstr+=x[i];temp=i+1;}
                                        deACval[AC_idx][AC_idx2]=7;
                                        AC_idx2++;
                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                        AC_idx2++;
                                    }
                                    else{err_cnt++;cout<<"SHIT:1111101X\n";}
                                }
                                else if(x[6]=='0'){//1111100
                                    if(x[7]=='1'){//11111001 (Run:2,Size:2)
                                        for(int i = 8 ; i < 10 ; i++){tmpstr+=x[i];temp=i+1;}
                                        deACval[AC_idx][AC_idx2]=2;
                                        AC_idx2++;
                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                        AC_idx2++;
                                    }
                                    else if(x[7]=='0'){//11111000 (Run:0,Size:7)
                                        for(int i = 8 ; i < 15 ; i++){tmpstr+=x[i];temp=i+1;}
                                        deACval[AC_idx][AC_idx2]=0;
                                        AC_idx2++;
                                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                        AC_idx2++;
                                    }
                                    else{err_cnt++;cout<<"SHIT:1111100X\n";}
                                }
                                else{err_cnt++;cout<<"SHIT:111110X\n";}
                            }
                            else{err_cnt++;cout<<"SHIT:11111X\n";}
                        }
                        else if(x[4]=='0'){//11110
                            if(x[5]=='1'){//111101
                                if(x[6]=='1'){//1111011 (Run:6,Size:1)
                                    for(int i = 7 ; i < 8 ; i++){tmpstr+=x[i];temp=i+1;}
                                    deACval[AC_idx][AC_idx2]=6;
                                    AC_idx2++;
                                    deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                    AC_idx2++;
                                }
                                else if(x[6]=='0'){//1111010 (Run:5,Size:1)
                                    for(int i = 7 ; i < 8 ; i++){tmpstr+=x[i];temp=i+1;}
                                    deACval[AC_idx][AC_idx2]=5;
                                    AC_idx2++;
                                    deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                    AC_idx2++;
                                }
                                else{err_cnt++;cout<<"SHIT:111101X\n";}
                            }
                            else if(x[5]=='0'){//111100
                                if(x[6]=='1'){//1111001  (Run:1,Size:3)
                                    for(int i = 7 ; i < 10 ; i++){tmpstr+=x[i];temp=i+1;}
                                    deACval[AC_idx][AC_idx2]=1;
                                    AC_idx2++;
                                    deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                    AC_idx2++;
                                }
                                else if(x[6]=='0'){//1111000 (Run:0,Size:6)
                                    for(int i = 7 ; i < 13 ; i++){tmpstr+=x[i];temp=i+1;}
                                    deACval[AC_idx][AC_idx2]=0;
                                    AC_idx2++;
                                    deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                    AC_idx2++;
                                }
                                else{err_cnt++;cout<<"SHIT:111100X\n";}
                            }
                            else{err_cnt++;cout<<"SHIT:11110X\n";}
                        }
                        else{err_cnt++;cout<<"SHIT:1111X\n";}
                    }
                    else if(x[3]=='0'){//1110
                        if(x[4]=='1'){//11101
                            if(x[5]=='1'){//111011 (Run:4,Size:1)
                                for(int i = 6 ; i < 7 ; i++){tmpstr+=x[i];temp=i+1;}
                                deACval[AC_idx][AC_idx2]=4;
                                AC_idx2++;
                                deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                AC_idx2++;
                            }
                            else if(x[5]=='0'){//111010 (Run:3,Size:1)
                                for(int i = 6 ; i < 7 ; i++){tmpstr+=x[i];temp=i+1;}
                                deACval[AC_idx][AC_idx2]=3;
                                AC_idx2++;
                                deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                                AC_idx2++;
                            }
                            else{err_cnt++;cout<<"SHIT:11101X\n";}
                        }
                        else if(x[4]=='0'){//11100 (Run:2,Size:1)
                            for(int i = 5 ; i < 6 ; i++){tmpstr+=x[i];temp=i+1;}
                            deACval[AC_idx][AC_idx2]=2;
                            AC_idx2++;
                            deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                            AC_idx2++;
                        }
                        else{err_cnt++;cout<<"SHIT:1110X\n";}
                    }
                    else{err_cnt++;cout<<"SHIT:111X\n";}
                }
                else if(x[2]=='0'){//110
                    if(x[3]=='1'){//1101
                        if(x[4]=='1'){//11011 (Run:1,Size:2)
                            for(int i = 5 ; i < 7 ; i++){tmpstr+=x[i];temp=i+1;}
                            deACval[AC_idx][AC_idx2]=1;
                            AC_idx2++;
                            deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                            AC_idx2++;
                        }
                        else if(x[4]=='0'){//11010 (Run:0,Size:5)
                            for(int i = 5 ; i < 10 ; i++){tmpstr+=x[i];temp=i+1;}
                            deACval[AC_idx][AC_idx2]=0;
                            AC_idx2++;
                            deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                            AC_idx2++;
                        }
                        else{err_cnt++;cout<<"SHIT:1101X\n";}
                    }
                    else if(x[3]=='0'){//1100 (Run:1,Size:1)
                        for(int i = 4 ; i < 5 ; i++){tmpstr+=x[i];temp=i+1;}
                        deACval[AC_idx][AC_idx2]=1;
                        AC_idx2++;
                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                        AC_idx2++;
                    }
                    else{err_cnt++;cout<<"SHIT:110X\n";}
                }
                else{err_cnt++;cout<<"SHIT:11X\n";}
            }
            else if(x[1]=='0'){//10
                if(x[2]=='1'){//101
                    if(x[3]=='1'){//1011 (Run:0,Size:4)
                        for(int i = 4 ; i < 8 ; i++){tmpstr+=x[i];temp=i+1;}
                        deACval[AC_idx][AC_idx2]=0;
                        AC_idx2++;
                        deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                        AC_idx2++;
                    }
                    else if(x[3]=='0'){//1010 (EOB) (Run:0,Size:0)
                        //tmpstr+=x[i];
                        temp=4;
                        deACval[AC_idx][AC_idx2]=-1024;
                        AC_idx2++;
                        EOB=true;
                    }
                    else{err_cnt++;cout<<"SHIT:101X\n";}
                }
                else if(x[2]=='0'){//100 (Run:0,Size:3)
                    for(int i = 3 ; i < 6 ; i++){tmpstr+=x[i];temp=i+1;}
                    deACval[AC_idx][AC_idx2]=0;
                    AC_idx2++;
                    deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                    AC_idx2++;
                }
                else{err_cnt++;cout<<"SHIT:10X\n";}
            }
            else{err_cnt++;cout<<"SHIT:1X\n";}
        }
        else if(x[0]=='0'){//0
            if(x[1]=='1'){//01 (Run:0,Size:2)
                for(int i = 2 ; i < 4 ; i++){tmpstr+=x[i];temp=i+1;}
                deACval[AC_idx][AC_idx2]=0;
                AC_idx2++;
                deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                AC_idx2++;
            }
            else if(x[1]=='0'){//00 (Run:0,Size:1)
                for(int i = 2 ; i < 3 ; i++){tmpstr+=x[i];temp=i+1;}
                deACval[AC_idx][AC_idx2]=0;
                AC_idx2++;
                deACval[AC_idx][AC_idx2]=str2val(tmpstr);
                AC_idx2++;
            }
            else{err_cnt++;cout<<"SHIT:0X\n";}
        }
        else{err_cnt++;cout<<"SHIT:X\n";}

        if(!EOB){
            if(temp!=x.length()){
                for(int i = temp ; i < x.length() ; i++){
                    acstr+=x[i];
                }
                x=acstr;
                //cout<<"x.length()1 = "<<x.length()<<endl;
            }
            else{
                x="";
                //cout<<"x.length()2 = "<<x.length()<<endl;
            }
        }
        else{
            if(temp!=x.length()){
                for(int i = temp ; i < x.length() ; i++){
                    acstr+=x[i];
                }
                instr=acstr;
                //cout<<"x.length()1 = "<<x.length()<<endl;
            }
            else{
                //cout<<"???????\n";
                instr="";
                //cout<<"x.length()2 = "<<x.length()<<endl;
            }
        }

    }

    idx_AC++;
    //cout<<"idx_AC = "<<idx_AC<<endl;
   // cout<<"instr.length() = "<<instr.length()<<endl;
}

/**ZigZagCoding 1D to 2D**/

void deAC_zigzag(float **x){
    int i=0,j=1,k=1;

    while(1){
        while(j!=0 && i!=7){//Going upside down until j!=0
            x[i][j]=deAC_zigarr[index][k++];
           // k++;
            i=i+1;
            j=j-1;
        }
        x[i][j]=deAC_zigarr[index][k++];//Take the edge element
       // k++;

        if(i<7)//If not last row, increment i
            i=i+1;

        else if(i==7)//If we hit the last row, we go right one place
            j=j+1;

        while(i!=0 && j!=7){//Going bottom up
            x[i][j]=deAC_zigarr[index][k++];
            i=i-1;
            j=j+1;
        }
        x[i][j]=deAC_zigarr[index][k++];//Take the edge element
        if(j<7)//If we didn't hit the edge, increment j
            j=j+1;

        else if(j==7)//If we hit the last element, go down one place
            i=i+1;

        if(i>=7 && j>=7)//If we hit last element matrix[8][8] exit
            break;
    }

}

int main(){
    // char x[] = "00000010";
   // cout << chars2ul(x) << endl;

    unsigned char image[height][width];
    int pixel[height][width];
    int DC[4096]={0};
    int DC2[4096]={0};
    int DC_Cate,choice,QF;
    int idx_tmp=0;
    bool Read_Success=false;

    char buf[FILENAME_MAX];
    char in[FILENAME_MAX+1];
/*
    printf("*Coding Operation:\n");
    printf("1)Encoding\n");
    printf("2)Decoding\n");
    printf("Your choice(1 or 2, other to exit):");
    scanf("%d",&choice);

    if(choice==1){

    }
*/
/*
    FILE *input=NULL;

    printf("Input complete filename: ");
    gets(buf);
    sprintf(in, """%s""", buf); // 為避免有空白,加入雙引號

    input =fopen(in,"rb");
    if(input==NULL){
        printf("Input filename error\n");
    }
    else{
        fread(image,sizeof(unsigned char),height*width,input);
        printf("Input image read success\n");
        Read_Success=true;
        fclose(input);
    }
*/
    Read_Success=true;
    FILE *input =fopen("Lena.raw","rb");
        fread(image,sizeof(unsigned char),height*width,input);
    fclose(input);

    fstream file;

    if(Read_Success){
        memset( (void*)pixel , 0 , sizeof(int) * height * width );//initialize 0

        for(int i = 0 ; i < height ; i++){ //char to 0~255
            for(int j = 0 ; j < width ; j++){
                pixel[i][j]=0+image[i][j]-128;   //-128 to do DCT
            }
        }

        float **img = new float*[height];//declare 512*512 **a
        for(int i = 0 ; i < height ; i++){
            img[i]=new float[width];
        }

        for(int i = 0 ; i < height ; i++){
            for(int j = 0 ; j < width ; j++){
                img[i][j] = 0.0 + pixel[i][j];
            }
        }

        float **block8x8 = new float*[8];//declare 8*8 block
        for(int k = 0 ; k < 8 ; k++){
            block8x8[k]=new float[8];
        }

        for(int i = 0 ; i < height ; i+=8){
            for(int j = 0 ; j < width ; j+=8){
                //cout<<"fuck "<<10*i+j<<endl;

                for(int u = 0 ; u < 8 ; u++){
                    //cout<<"shit "<<10*i+j<<endl;
                    for(int v = 0 ; v < 8 ; v++){
                        block8x8[u][v]=img[u+i][v+j];
                    }
                }
               // cout<<"DCT "<<10*i+j<<endl;

                dct2(block8x8,8);//do 2D DCT
                for(int e = 0 ; e < 8 ; e++){
                    for(int q = 0 ; q < 8 ; q++){
                        block8x8[e][q]=round(block8x8[e][q]);
                    }
                }
               // cout<<"Quantize "<<10*i+j<<endl;

                quantize(block8x8,8,QFC);//Quantize
                for(int e = 0 ; e < 8 ; e++){
                    for(int q = 0 ; q < 8 ; q++){
                        block8x8[e][q]=round(block8x8[e][q]);
                    }
                }

                if(i==0&&j==0){
                    DC[index_DC]=img[i][j];
                    DC2[index_DC]=DC[index_DC];
                  //  index_DC++;
                    //cout<<"index_DC = "<<index_DC<<endl;
                }
                else{
                    DC[index_DC]=img[i][j];
                    DC2[index_DC]=DC[index_DC]-DC[index_DC-1];//DCn'=DCn-DCn-1 (O) ||DCn-DCn-1' (X)
                   // index_DC++;
                    //cout<<"index_DC = "<<index_DC<<endl;
                }

                DC_Cate=getCategoryOfDCTCoefficient(DC2[index_DC]);

               // if(DC_Cate>11){cout<<"WTF"<<endl;}
               // cout<<"DC_Category = "<<DC_Cate<<endl;

                DC_codeword(DC2[index_DC],DC_Cate,index_DC);

                AC_zigzag(block8x8);//make the normal array to zigzag array
               // for(int s = 0 ; s < 64 ; s++){ cout<<AC_zigarr[s]<<" ";}
             //   cout<<endl;

                AC_codeword(AC_zigarr);
            /**check**/
                if(i==0&&j==0){
                    for(int c = 0 ; c < 64 ; c++){
                        AC_testarr[c]=AC_zigarr[c];
                    }
                }
            /**check**/
            }
        }
    /*
        for(int i = 0 ; i < 64 ; i++){ cout<<"DC[index_DC] = "<<DC[i]<<" DC2[index_DC] = "<<DC2[i]<<endl;}
    */
    /**  clear the **array  **/
        for (int w = 0 ; w < 8 ; w++){delete [] block8x8[w];}
        for (int i = 0 ; i < height ; i++){delete [] img[i];}
    //check
    /*
        for(int i = 0 ; i < 64 ; i++){ cout<<"DC_bitstring = "<<DC_bitstring[i]<<endl;}
    */
    /*
        for(int i = 0 ; i < 64 ; i++){ cout<<"AC_bitstring = "<<AC_bitstring[i]<<endl;}
    */
    //check
    //for(int i = 0 ; i < 64 ; i++){ cout<<"bitstring = "<<DC_bitstring[i]<<AC_bitstring[i]<<endl;}
        //cout<<"error_count = "<<err_cnt<<endl;
/**DC AC DC AC......**/
/*
        for(int i = 0 ; i < 4096 ; i++){
            DC_AC_bits+=DC_bitstring[i]+AC_bitstring[i];
        }
*/
/**DC DC DC .... AC AC AC....**/
        for(int i = 0 ; i < 4096 ; i++){
            DC_AC_bits+=DC_bitstring[i];
        }
        for(int i = 0 ; i < 4096 ; i++){
            DC_AC_bits+=AC_bitstring[i];
        }

        int len = DC_AC_bits.length();
        cout<<"len="<<len<<endl;

        while(DC_AC_bits.length()%8!=0){
            DC_AC_bits+="0";
        }



        char *bitstring = new char[len];
        strcpy(bitstring,DC_AC_bits.c_str());

/**One of output methods**/

        file.open("Lena_QF90.data",ios::out|ios::binary);

        if(!file)     //檢查檔案是否成功開啟
        {
            cerr << "Can't open file!\n";
            exit(1);     //在不正常情形下，中斷程式的執行
        }

        //file<<"\xFF\xD8";
        //file<<"\xFF\xE0\0\x10JFIF\0\x01\x02\x01\0\x64\0\x64\0\0";
        //file<<"\xFF\xDA";
        for(int i = 0 ; i < len ; i+=8){
            //fprintf(file,"%c",chars2ul(&bitstring[i]));
            file<<(unsigned char)chars2ul(&bitstring[i]);

        }
        //file<<"\xFF\xD9";

        file.close();
        file.clear();

/**One of output methods**/
/*
        FILE *out;

        out = fopen("JPEG.data", "wb+");

        //fprintf(out,"\xFF\xD8");
        //fprintf(out,"\x20\x20\x01");
        //fwrite("\xff\xd8\xff\xe0\x00\x10JFIF\x00\x01\x02\x01\x00\x64\x00\x64\x00\x00", 20, 20, out);
        //fputs("\xFF\xDA", out);

        for(int i = 0 ; i < len ; i+=8){
          //  unsigned char aaa = chars2ul(&bitstring[i]);
           // fwrite(aaa,sizeof(unsigned char),sizeof(aaa),out);
            fprintf(out,"%c",chars2ul(&bitstring[i]));
        }
        //fprintf(out,"\xFF\xD9");

//        fprintf(out,"%o",shit);
      //  strcpy(bitstring,DC_AC_bits.c_str());
       // fputs(bitstring,out);
     //   for(int i = 0 ; i < 4096 ; i++){
           // strcpy(bitstring,DC_bitstring[i].c_str());
           // fputs(bitstring,out);
            //fputs("\n",out);
           // strcpy(bitstring,AC_bitstring[i].c_str());
           // fputs(bitstring,out);

            //strcpy(bitstring,DC_AC_bits[i].c_str());
            //fputs(bitstring,out);
            //fputs("\n",out);
    //    }

        delete[] bitstring;

        fclose(out);
*/
        printf("\n--------- Encode Process Ending ---------\n");


    }
/*    //check
    for(int i = 0 ; i < 64 ; i++){
        cout<<DC_AC_bits[i];
    }
    cout<<endl;
*/

/***********************************************************************************************************************************************/
/******************************************************************Decode Start*****************************************************************/
/***********************************************************************************************************************************************/

    unsigned char deimage[height][width];
    int depixel[height][width];
    //memset( (void*)depixel , 0 , sizeof(int) * height * width );//initialize 0


    file.open("Lena_QF90.data", ios::in | ios::binary);

    if (!file){
        cerr << "open file for read failure!" << endl;
        return 0;
    }

    string st="";
    unsigned char uc = 0;
    int char2bit;
    while (file.read((char *)&uc, sizeof(uc))){
        char2bit=(int)uc;
        bitset<8> bs(char2bit);
        st = bs.to_string<char, char_traits<char>, allocator<char> > ();
        instr+=st;
        /*cout << bs << "\t";
        if (uc % 10 == 0){
            cout << endl;
        }*/
    }
/*  //check
    for(int i = 0 ; i < 64 ; i++){
        cout<<instr[i];
    }
    cout<<endl;
*/

    file.close();
    file.clear();

    int len2=instr.length();
   // cout<<"len2="<<len2<<endl;

/**********************DC AC Decode Start***********************************/

    cout<<"DC Decode Start"<<endl;
   // cout<<"instr.length() = "<<instr.length()<<endl;
/*
//DC AC DC AC......
    while(instr.length()!=0){

        str2DC(instr,idx_DC);
        str2AC(instr,idx_AC);

        if(idx_AC==1024){cout<<"DC,AC Decode 25%"<<endl;cout<<"instr.length() = "<<instr.length()<<endl;}
        if(idx_AC==2048){cout<<"DC,AC Decode 50%"<<endl;cout<<"instr.length() = "<<instr.length()<<endl;}
        if(idx_AC==3072){cout<<"DC,AC Decode 75%"<<endl;cout<<"instr.length() = "<<instr.length()<<endl;}
        if(idx_AC==4096){cout<<"DC,AC Decode 100%"<<endl;cout<<"instr.length() = "<<instr.length()<<endl;}
        if(idx_DC==4096&&idx_AC==4096){break;}
    }
*/
//DC DC DC ......
    while(instr.length()!=0){

        str2DC(instr,idx_DC);

        if(idx_DC==410){cout<<"DC Decode 10%"<<endl;/*cout<<"instr.length() = "<<instr.length()<<endl;*/}
        if(idx_DC==819){cout<<"DC Decode 20%"<<endl;/*cout<<"instr.length() = "<<instr.length()<<endl;*/}
        if(idx_DC==1229){cout<<"DC Decode 30%"<<endl;/*cout<<"instr.length() = "<<instr.length()<<endl;*/}
        if(idx_DC==1638){cout<<"DC Decode 40%"<<endl;/*cout<<"instr.length() = "<<instr.length()<<endl;*/}
        if(idx_DC==2048){cout<<"DC Decode 50%"<<endl;/*cout<<"instr.length() = "<<instr.length()<<endl;*/}
        if(idx_DC==2458){cout<<"DC Decode 60%"<<endl;/*cout<<"instr.length() = "<<instr.length()<<endl;*/}
        if(idx_DC==2867){cout<<"DC Decode 70%"<<endl;/*cout<<"instr.length() = "<<instr.length()<<endl;*/}
        if(idx_DC==3277){cout<<"DC Decode 80%"<<endl;/*cout<<"instr.length() = "<<instr.length()<<endl;*/}
        if(idx_DC==3686){cout<<"DC Decode 90%"<<endl;/*cout<<"instr.length() = "<<instr.length()<<endl;*/}
        if(idx_DC==4096){cout<<"DC Decode 100%"<<endl;/*cout<<"instr.length() = "<<instr.length()<<endl;*/}
        if(idx_DC==4096){break;}
    }

cout<<"AC Decode Start"<<endl;

//AC AC AC ......
    while(instr.length()!=0){

        str2AC(instr,idx_AC);
        if(idx_AC==410){cout<<"AC Decode 10%"<<endl;cout<<"instr.length() = "<<instr.length()<<endl;}
        if(idx_AC==819){cout<<"AC Decode 20%"<<endl;cout<<"instr.length() = "<<instr.length()<<endl;}
        if(idx_AC==1229){cout<<"AC Decode 30%"<<endl;cout<<"instr.length() = "<<instr.length()<<endl;}
        if(idx_AC==1638){cout<<"AC Decode 40%"<<endl;cout<<"instr.length() = "<<instr.length()<<endl;}
        if(idx_AC==2048){cout<<"AC Decode 50%"<<endl;cout<<"instr.length() = "<<instr.length()<<endl;}
        if(idx_AC==2458){cout<<"AC Decode 60%"<<endl;cout<<"instr.length() = "<<instr.length()<<endl;}
        if(idx_AC==2867){cout<<"AC Decode 70%"<<endl;cout<<"instr.length() = "<<instr.length()<<endl;}
        if(idx_AC==3277){cout<<"AC Decode 80%"<<endl;cout<<"instr.length() = "<<instr.length()<<endl;}
        if(idx_AC==3686){cout<<"AC Decode 90%"<<endl;cout<<"instr.length() = "<<instr.length()<<endl;}
        if(idx_AC==4096){cout<<"AC Decode 100%"<<endl;cout<<"instr.length() = "<<instr.length()<<endl;}
        if(idx_AC==4096){break;}
    }


    cout<<"DC,AC Decode Finish"<<endl;

/***************************DC AC Decode Finish****************************************/

    int TrueDeDCval[4096]={0};

    TrueDeDCval[0]=deDCval[0];

    for(int i = 1 ; i < 4096 ; i++){
        TrueDeDCval[i]=deDCval[i]+TrueDeDCval[i-1];
    }

/****************check ****************/
/*
    for(int i = 0 ; i < 32 ; i++){
        cout<<" DC = "<<DC[i];
        cout<<" DC2 = "<<DC2[i];
        cout<<" TrueDeDCval = "<<TrueDeDCval[i];
        cout<<" deDCval = "<<deDCval[i];
        cout<<endl;
    }



    cout<<" deACval= ";
    for(int i = 0 ; i < 10 ; i++){
        for(int j = 0 ; j < 129 ; j++){
            cout<<" "<<deACval[i][j]<<" ";
        }
        cout<<endl;
    }

    cout<<"AC_testarr = ";
    for(int i = 0 ; i < 64 ; i++){
            cout<<" "<<AC_testarr[i]<<" ";
            if(i!=0&&i%7==0)cout<<endl;
    }
*/

/********************************/



    for(int i = 0 ; i < 4096 ; i++){
        idx_tmp=1;
        for(int j = 0 ; j < 129 ; j+=2){
            if(deACval[i][j]!=-1024){
                if(deACval[i][j]==0){
                    deAC_zigarr[i][idx_tmp]==deACval[i][j+1];
                    idx_tmp++;
                }
                else{
                    for(int k = idx_tmp ; k < idx_tmp+deACval[i][j] ; k++){
                        deAC_zigarr[i][k]=0;
                    }
                    idx_tmp+=deACval[i][j];
                    deAC_zigarr[i][idx_tmp]=deACval[i][j+1];
                    idx_tmp++;
                }
            }
            else{
                if(idx_tmp!=64){
                    for(int w = idx_tmp ; w < 64 ; w++){
                        deAC_zigarr[i][w]=0;
                    }
                }
                else{
                    break;
                }
            }
        }
    }


    float **deimg = new float*[height];//declare 512*512 **a
    for(int i = 0 ; i < height ; i++){
        deimg[i]=new float[width];
    }

    float **deblock8x8 = new float*[8];//declare 8*8 block
    for(int k = 0 ; k < 8 ; k++){
        deblock8x8[k]=new float[8];
    }
    float shit[8][8];
    float fuck[8][8];
    float e04[8][8];

    for(int i = 0 ; i < height ; i+=8){
        for(int j = 0 ; j < width ; j+=8){

            deblock8x8[0][0]=TrueDeDCval[index];

            deAC_zigzag(deblock8x8);

            if(i==0&j==0){
                for(int e = 0 ; e < 8 ; e++){
                    for(int q = 0 ; q < 8 ; q++){
                       shit[e][q]=deblock8x8[e][q];
                    }
                }
            }
            //cout<<"\n------DeQuantize Start------\n";
            dequantize(deblock8x8,8,QFC);//Quantize

            for(int e = 0 ; e < 8 ; e++){
                for(int q = 0 ; q < 8 ; q++){
                    deblock8x8[e][q]=round(deblock8x8[e][q]);
                }
            }

            if(i==0&j==0){
                for(int e = 0 ; e < 8 ; e++){
                    for(int q = 0 ; q < 8 ; q++){
                       fuck[e][q]=deblock8x8[e][q];
                    }
                }
            }
            //cout<<"\n------IDCT_2D Start------\n";

            idct2(deblock8x8,8);//do 2D iDCT

            for(int e = 0 ; e < 8 ; e++){
                for(int q = 0 ; q < 8 ; q++){
                    deblock8x8[e][q]=round(deblock8x8[e][q])+128;
                }
            }

            for(int e = 0 ; e < 8 ; e++){
                for(int q = 0 ; q < 8 ; q++){
                    if(deblock8x8[e][q]>255){deblock8x8[e][q]=255;}
                }
            }


            if(i==0&j==0){
                for(int e = 0 ; e < 8 ; e++){
                    for(int q = 0 ; q < 8 ; q++){
                       e04[e][q]=deblock8x8[e][q];
                    }
                }
            }

            for(int u = 0 ; u < 8 ; u++){
                for(int v = 0 ; v < 8 ; v++){
                    deimg[u+i][v+j]=round(deblock8x8[u][v]);
                    //if()
                }
            }


            index++;
        }
    }
    /*****************check********************/
/*
    cout<<"normal:"<<endl;

    for(int i = 0 ; i < 8 ; i++){
        for(int j = 0 ; j < 8 ; j++){
            cout<<" "<<setw(2)<<shit[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
    cout<<"after dequantize:"<<endl;
    for(int i = 0 ; i < 8 ; i++){
        for(int j = 0 ; j < 8 ; j++){
            cout<<" "<<setw(2)<<fuck[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
    cout<<"after idct2:"<<endl;

    for(int i = 0 ; i < 8 ; i++){
        for(int j = 0 ; j < 8 ; j++){
            cout<<" "<<setw(2)<<e04[i][j]<<" ";
        }
        cout<<endl;
    }
*/

    float init_val=0.0;
    vector< vector<float> > ig;
    ig.resize( height , vector<float>( width , init_val ) );

    for(int i = 0 ; i < height ; i++){
        for(int j = 0 ; j < width ; j++){
            //depixel[i][j] =(int) deimg[i][j];
            ig[i][j]=deimg[i][j];
            //cout<<ig[i][j]<<endl;
            //cout<<(int)deimg[i][j]<<endl;
        }
    }
    unsigned char ucc = 0;

    vector<unsigned char> yo;
    yo.assign(height,ucc);//配置一個 row 的大小
    vector< vector<unsigned char> > a_2D;
    a_2D.assign(width,yo);//配置2維

    for(int i = 0 ; i < height ; i++){
        for(int j = 0 ; j < width ; j++){
            a_2D[i][j]=ig[i][j];
            //cout<<" "<<a_2D[i][j]<<" ";
        }
        //cout<<endl;
    }

/************************Output file*********************/

    file.open("Lena_QF90.raw", ios::out | ios::binary);

    if (!file){
        cerr << "open file for read failure!" << endl;
        return 0;
    }

    for(int i = 0 ; i < height ; i++){
        for(int j = 0 ; j < width ; j++){
            ucc=a_2D[i][j];
            file.write((char *)&ucc, sizeof(ucc));
        }
    }



    file.close();
    file.clear();



/*
    for(int i = 0 ; i < height ; i++){ //char to 0~255
        for(int j = 0 ; j < width ; j++){
            deimage[i][j] =(unsigned char) depixel[i][j];
        }
    }

    FILE *f1 =fopen("Lena_QF50.raw","wb");
        fwrite(deimage,1,height*width,f1);
    fclose(f1);
*/


    /******* clear the **array  *******/
    for (int r = 0 ; r < 8 ; r++){delete [] deblock8x8[r];}
    for (int i = 0 ; i < height ; i++){delete [] deimg[i];}

    cout<<"\n--------- Decode Process Ending ---------\n";

    return 0;
}



