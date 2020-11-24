#include <iostream>
#include <chrono>
#include <dirent.h> //opendir
#include <string.h>
#include <stdio.h>
#include <vector>
#include "bmp.h"



    using namespace std;
    
    string checkPathString(string operation ,string in_path, string out_path);
    void listdir(string initStrPath);

    string checkPathString( string op, string in_path, string out_path){
        if(in_path[in_path.length()-1] == '/'){
            in_path = in_path.substr(0, in_path.length()-1);
        }
        return in_path;
    }



    void listdir(string initStrPath){
        struct dirent *dir;
        DIR *dp = opendir(initStrPath.c_str());
        string op;
        string out_path;
        if (dp){
            while((dir=readdir(dp)) != NULL){
                string strPath = "";
                if( strcmp(dir->d_name, ".")==0 || strcmp(dir->d_name,".")==0) continue;
                if( dir->d_type == DT_DIR){
                    initStrPath = checkPathString(op,initStrPath,out_path);
                    strPath = initStrPath + "/" + dir->d_name;
                    listdir(strPath);
                }else{
                        initStrPath = checkPathString(op,initStrPath,out_path);
                        strPath = initStrPath + "/" + dir->d_name;
                        cout << strPath << endl;
                }
            }
            closedir(dp);
        } 
    }

int main(int argc, char *argv[]){

    if(argc <=3){
        cout << "Wrong format:"<<endl;
        cout << argv[0] << "<operation, in_path, out_path>" << endl;
    }
    listdir(argv[1]);
    /*
    char option;
    argv[1] = option;
    if (option=="copy"|| option=="gauss"||option=="sobel"){ //check the operation mode from keyboard
    
    }else{
        cerr<<"Unexpected operation: " << option; //in case wrong option, we have an standard error
        bool x = false;
        while(x!=true){
            cout <<"operation: copy, gauss, sobel";
            if (argv[1] == "copy"|| argv[1] == "gauss"|| argv[1] == "sobel"){ //check the operation mode from keyboard
                x = true;
            }else{
                cerr<<"Unexpected operation: " << argv[1]; //in case wrong option, we have an standard error
            }
        }
    }
    */

    cout << "bmp part start\n";

    BMP prueba("../inimg/test24.bmp");
    prueba.write("test24_copy.bmp");
    //BMP prueba_falla ("../inimg/test.bmp");
    //prueba_falla.write("Shapes_copy.bmp");

    cout << "bmp part end\n";


    return 0;
}