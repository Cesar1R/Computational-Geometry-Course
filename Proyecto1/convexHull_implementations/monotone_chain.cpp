#include "geo_prop.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;
using namespace geoProp;
using file_operations::load_vector;
using file_operations::save_vector;


string process_path;

vector<point> monotoneChain(vector<point> & S){
    int n = (int)S.size();
    if(n <= 3) return S;

    sort(S.begin(), S.end());

    vector<point> U, L;
                  U.reserve(n/2 + 1);
                  L.reserve(n/2 + 1);
    
    for(int i=0; i<n; i++){
        while(L.size() >= 2 
              && ccw(*(L.rbegin() + 1), *(L.rbegin()), S[i])){
            L.pop_back();
        }
        L.push_back(S[i]);
    }

    save_vector(L, process_path + "/cHull1.txt");

    for(int i=n-1; 0<=i; i--){
        while(U.size() >= 2
              && ccw(*(U.rbegin()+1), *(U.rbegin()), S[i])){
            U.pop_back();
        }

        U.push_back(S[i]);
    }

    save_vector(U, process_path + "/cHull2.txt");

    L.pop_back();
    U.pop_back();

    L.insert(L.end(), U.begin(), U.end());

    save_vector(L, process_path + "/cHull3.txt");

    return L;
}


int main(int argc, char* argv []){

    if(argc < 3 || 4 < argc ){
        cerr << "Usage: " << argv[0] << "<input_file> <output_file>" << endl; 
        return 1; 
    }

    string in_path = argv[1], 
           out_path = argv[2];

    process_path = (argc == 4)? argv[3]:"";

    vector<point> vec;
    try{
        load_vector(vec, in_path);

        vector<point> cHull = monotoneChain(vec);
        
        save_vector(cHull, out_path);
    } catch (const std::exception &e) {
        cerr << "Error: " << e.what() << endl;
    }


    return 0;
}