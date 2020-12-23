#include <iostream>
#include <chrono>
#include <dirent.h> //opendir
#include <string.h>
#include <stdio.h>
#include <vector>
#include "bmp.h"
#include <cstring>


using namespace std;
using namespace chrono;

string checkPathString(string in_path);

void listdir(string in_path);

string checkPathString(string in_path) {
    if (in_path[in_path.length() - 1] == '/') {
        in_path = in_path.substr(0, in_path.length() - 1);
    }
    return in_path;
}

void listdir(string in_path, string out_path) {
    struct dirent *dir; // pointer represent directory stream
    DIR *dp = opendir(in_path.c_str()); // creating pointer of type dirent
    string op;
    string out;
    bool result = false; //check the input path
    if (dp) {
        while ((dir = readdir(dp)) != NULL) {
            string strPath = "";
            if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
                result = true;
                break;
            }
            if (dir->d_type == DT_DIR) {
                in_path = checkPathString(in_path);
                strPath = in_path + "/" + dir->d_name;
                listdir(strPath, out_path);
            } else {
                in_path = checkPathString(in_path);
                strPath = in_path + "/" + dir->d_name;
                cout << strPath << endl;
            }
        }
        if (result) {
            cout << "Input path: " << in_path << endl;//print if the file is correct
            cout << "Output path: " << out_path << endl;//print if the file is correct
            while ((dir = readdir(dp)) != NULL) {
				if (strcmp(dir->d_name,".")!=0){
                	auto s = chrono::system_clock::now();
                	cout << "File: " <<dir->d_name << endl;
                	auto end = chrono::system_clock::now();
                	chrono::duration<float,std::milli> duration = end - s;
                	cout <<"Load time: " << duration.count()<< endl;
                
                    string strPath = in_path + "/" + dir->d_name;
                    const char *i = strPath.c_str();
                    BMP p1(i);
                    p1.write("copy_temp.bmp");
                    auto e0 = chrono::system_clock::now();
                    chrono::duration<float,std::milli> d0 = e0-s;
                    cout <<"Store time: " << d0.count()<< endl;
                    p1.gaussBlur();
                    auto e1 = chrono::system_clock::now();
                    chrono::duration<float,std::milli> d1 = e1-s;
                    cout <<"Gauss time: " << d1.count()<< endl;
                    p1.sobelOperator();
                    auto e2 = chrono::system_clock::now();
                    chrono::duration<float,std::milli> d2 = e2-s;
                    cout <<"Sobel time: " << d2.count()<< endl;

                }
            }
        }
        closedir(dp);
    }
}

// Check whether args are acceptable
bool check_args(int argc, char *argv[]) {
    if (argc != 4) {
        cout << "Wrong format:" << endl;
        cout << argv[0] << " <operation, in_path, out_path>" << endl;
        return false;
    }
    if (argv[1] == string("gauss") || argv[1] == string("copy") || argv[1] == string("sobel")) {
        DIR *dirp1;
        DIR *dirp2;
        dirp1 = opendir(argv[2]);
        dirp2 = opendir(argv[3]);
        bool correct1 = false;
        bool correct2 = false;
        if (dirp1 == NULL) {
            cout << "Input directory " << "[" << argv[2] << "]" << "does not exist" << endl;
        } else {
            correct1 = true;
        }
        if (dirp2 == NULL) {
            cout << "Output directory " << "[" << argv[3] << "]" << "does not exist" << endl;
        } else {
            correct2 = true;
        }
        if (correct1 == true && correct2 == true) {
            listdir(argv[2], argv[3]);

        } else {
            return false; // if one of the directories are not able to open /don´t exist, the program will finish
        }
    } else {
        cerr << "Unexpected operation: " << argv[1] << endl; //in case wrong option, we have an standard error
        bool x = false;
        while (x != true) {
            cout << "operation: copy, gauss, sobel" << endl;
            string op;
            cin >> op;
            if (op == string("gauss") || op == string("copy") || op == string("sobel")) {
                x = true;
            }
        }
        DIR *dirp1;
        DIR *dirp2;
        dirp1 = opendir(argv[2]);
        dirp2 = opendir(argv[3]);
        bool correct1 = false;
        bool correct2 = false;
        if (dirp1 == NULL) {
            cout << "Input directory " << "[" << argv[2] << "]" << "does not exist" << endl;
        } else {
            correct1 = true;
        }
        if (dirp2 == NULL) {
            cout << "Output directory " << "[" << argv[3] << "]" << "does not exist" << endl;
        } else {
            correct2 = true;
        }
        if (correct1 == true && correct2 == true) {
            listdir(argv[2], argv[3]);
        } else {
            return false; // if one of the directories are not able to open /don´t exist, the program will finish
        }
    }
    return true;
}

int main(int argc, char *argv[]) {
    if(!check_args(argc, argv)) return 1;   // End program with error 1 if check wasn't successful
    string option {argv[1]};
    cout << "the operation is: "<<option <<endl;
    struct dirent *dir; // pointer represent directory stream
    DIR *dp = opendir(argv[2]); // creating pointer of type dirent
	std::string result;
	std::string outpath = argv[3];
	int count=0;
        if(strcmp(argv[1],"copy")==0) {
            while ((dir = readdir(dp)) != NULL)
            {
                cout << dir->d_name << endl;
				if(strcmp(dir->d_name,".")==0||strcmp(dir->d_name,"..")==0){}
				else{
                    string strPath = string(argv[2]) + "/" + dir->d_name;
                    const char *i = strPath.c_str();
                    BMP p1(i);
					result = outpath + "/copy_" + std::to_string(count) +".bmp";
                    p1.write(result.c_str());
                    cout << "File: " <<dir->d_name << " has been copied "<<endl;
					count++;
                }
            }
        }else if(strcmp(argv[1],"gauss")==0) {
            while ((dir = readdir(dp)) != NULL)
            {
                if(strcmp(dir->d_name,".")==0||strcmp(dir->d_name,"..")==0){}
				else{
                    string strPath = string(argv[2]) + "/" + dir->d_name;
                    const char *i = strPath.c_str();
                    cout << i << endl;
                    BMP p1(i);
                    p1.gaussBlur();
					result = outpath + "/gauss_copy_" + std::to_string(count) +".bmp";
                    p1.write(result.c_str());
                    cout << "File: " <<dir->d_name << " the operation gaussian blur has been applied "<<endl;
					count++;
                }
            }
        }else if(strcmp(argv[1],"sobel")==0) {
            while ((dir = readdir(dp)) != NULL)
            {
               if(strcmp(dir->d_name,".")==0||strcmp(dir->d_name,"..")==0){}
				else{
                    string strPath = string(argv[2]) + "/" + dir->d_name;
                    const char *i = strPath.c_str();
                    BMP p1(i);
					p1.gaussBlur();
                    p1.sobelOperator();  //NOT SURE IF ONLY HERE or also FOR MEASUREMENT
					result = outpath + "/sobel_copy_" + std::to_string(count) +".bmp";
                    p1.write(result.c_str());
                    cout << "File: " <<dir->d_name << " the operation sobel has been applied "<<endl;
					count++;
                }
            }
        }else {
            throw std::runtime_error("Error: unexpected operation");
        }

    return 0;
}
