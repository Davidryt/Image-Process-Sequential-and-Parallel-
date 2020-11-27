#include <iostream>
#include <chrono>
#include <dirent.h> //opendir
#include <string.h>
#include <stdio.h>
#include <vector>
#include "bmp.h"
#include <cstring>
#include <filesystem>


    using namespace std;
    using namespace chrono;
    
    string checkPathString(string operation ,string in_path, string out_path);
    void listdir(string in_path);

    string checkPathString( string op, string in_path, string out_path){
        if(in_path[in_path.length()-1] == '/'){
            in_path = in_path.substr(0, in_path.length()-1);
        }
        return in_path;
    }



    void listdir(string in_path, string out_path){
        struct dirent *dir; // pointer represent directory stream
        DIR *dp = opendir(in_path.c_str()); // creating pointer of type dirent
        string op;
        string out;
        bool result = false; //check the input path
        if (dp){
            while((dir=readdir(dp)) != NULL){
                string strPath = "";
                if( strcmp(dir->d_name, ".")==0 || strcmp(dir->d_name,".")==0){
                    result=true;
                    break;
                }
                if( dir->d_type == DT_DIR){
                    in_path = checkPathString(op,in_path,out);
                    strPath = in_path + "/" + dir->d_name;
                    listdir(strPath, out_path);
                }else{
                        in_path = checkPathString(op,in_path,out);
                        strPath = in_path + "/" + dir->d_name;
                        cout << strPath << endl;
                    }
            }
            if(result){
                cout<<"Input path: "<< in_path <<endl;//print if the file is correct
                cout<<"Output path: "<< out_path <<endl;//print if the file is correct 
                while((dir=readdir(dp)) != NULL){
                    cout <<"File: "<< dir->d_name << endl; 
                }
            } 
            closedir(dp);
        } 
    }

int main(int argc, char *argv[]){

    if(argc <=3){
        cout << "Wrong format:"<<endl;
        cout << argv[0] << " <operation, in_path, out_path>";
    }if(argv[1] == string("gauss") ||argv[1] == string("copy") || argv[1] == string("sobel")){
        DIR *dirp1;
        DIR *dirp2;
        dirp1 = opendir(argv[2]);
        dirp2 = opendir(argv[3]);
        bool correct1 = false;
        bool correct2 = false;
        if (dirp1 == NULL){
        cout<<"Input directory " << "["<<argv[2]<<"]"<< "does not exist" <<endl;
        }else{
            correct1 = true;
        }
        if (dirp2 == NULL){
        cout<<"Output directory " << "["<<argv[3]<<"]"<< "does not exist" <<endl;
        }else{
            correct2 = true;
        }
        if (correct1 == true && correct2 == true){
            listdir(argv[2], argv[3]);

        }else{
            return 0; // if one of the directories are not able to open /don´t exist, the program will finish
        }
    }else{
        cerr<<"Unexpected operation: " << argv[1]<<endl; //in case wrong option, we have an standard error
        bool x = false;
        while(x !=true){
            cout << "operation: copy, gauss, sobel"<<endl;
            string op;
            cin >> op;
            if(op == string("gauss") ||op == string("copy") || op == string("sobel")){
                x = true;
            }
        }
        DIR *dirp1;
        DIR *dirp2;
        dirp1 = opendir(argv[2]);
        dirp2 = opendir(argv[3]);
        bool correct1 = false;
        bool correct2 = false;
        if (dirp1 == NULL){
        cout<<"Input directory " << "["<<argv[2]<<"]"<< "does not exist" <<endl;
        }else{
            correct1 = true;
        }
        if (dirp2 == NULL){
        cout<<"Output directory " << "["<<argv[3]<<"]"<< "does not exist" <<endl;
        }else{
            correct2 = true;
        }
        if (correct1 == true && correct2 == true){
            listdir(argv[2], argv[3]);
        }else{
            return 0; // if one of the directories are not able to open /don´t exist, the program will finish
        }
    }

    cout << "bmp part start\n";

    BMP prueba("../inimg/t1_24.bmp");
    prueba.write("copy.bmp");
    //BMP prueba_falla ("../inimg/test.bmp");
    //prueba_falla.write("Shapes_copy.bmp");

    cout << "bmp part end\n";


    return 0;
}