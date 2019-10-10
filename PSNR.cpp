#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#define height 512
#define width 512

using namespace std;

int main(){
    double MSE=0.0;
    double PSNR=0.0;
    double difference=0.0,sum=0.0;
    unsigned char uc=0;
    unsigned char image1[height][width];
    unsigned char image2[height][width];
    //double pixel1[height][width];
    //double pixel2[height][width];

    fstream file1,file2;

    file1.open("Lena.raw", ios::in|ios::binary);
    file2.open("Lena_QF90.raw",ios::in|ios::binary);

    if (!file1){
        cerr << "open file1 for read failure!" << endl;
        return 0;
    }
    if (!file2){
        cerr << "open file2 for read failure!" << endl;
        return 0;
    }

    /*
    vector<unsigned char> row1;
    row1.assign(height,uc);//配置一個 row 的大小
    vector< vector<unsigned char> > img1;
    img1.assign(width,row1);//配置2維
*/
    int init_val=0;
    vector< vector<int> > pixel1;
    pixel1.resize( height , vector<int>( width , init_val ) );
    vector< vector<int> > pixel2;
    pixel2.resize( height , vector<int>( width , init_val ) );

    for(int i = 0 ; i < height ; i++){
        for(int j = 0 ; j < width ; j++){
            file1.read((char*)&uc,sizeof(char));
            image1[i][j]=uc;
        }
    }
    for(int i = 0 ; i < height ; i++){
        for(int j = 0 ; j < width ; j++){
            file2.read((char*)&uc,sizeof(char));
            image2[i][j]=uc;
        }
    }

    for(int i = 0 ; i < height ; i++){
        for(int j = 0 ; j < width ; j++){
            pixel1[i][j]=image1[i][j];
            pixel2[i][j]=image2[i][j];
        }
    }
    for(int i = 0 ; i < height ; i++){
        for(int j = 0 ; j < width ; j++){
            difference=fabs(pixel1[i][j]-pixel2[i][j]);
            sum+=difference*difference;
        }
    }

    MSE=sum/(height*width);

    PSNR = 10.0*log10((255*255)/MSE);


    cout<<"MSE = "<<MSE<<endl;
    cout<<"PSNR = "<<PSNR<<endl;


    file1.close();
    file1.clear();
    file2.close();
    file2.clear();

    return 0;
}
