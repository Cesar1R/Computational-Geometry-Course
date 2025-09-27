#include "geo_prop.h"
#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <algorithm>

#include<cmath>

using namespace std;
using namespace geoProp;
using file_operations::load_vector;
using file_operations::save_vector;


//Showing progress
string process_path = "";


vector<point> graham_scan(vector<point> & S){
    int n = (int)S.size();
    if(n <= 3) return S;

    //pivot index
    int pivot_index = 0; 
    for(int i=0; i<(int)S.size(); i++){
        if(S[i].y < S[pivot_index].y || 
           (S[i].y == S[pivot_index].y && S[i].x < S[pivot_index].x)){
            pivot_index = i;
        }
    }

    swap(S[0], S[pivot_index]);
    point pivot = S[0];

    std::sort(S.begin() + 1, S.end(), [&pivot](const point &a, const point &b){
        return polarCompare(a, b, pivot);
    });

    stack<point> pointStack;
    pointStack.push(S[0]);
    pointStack.push(S[1]);

    point currTop; 
    for(int i=2; i<n; i++){
        currTop = pointStack.top(); pointStack.pop();

        while(pointStack.size() > 1 
             && (cross(pointStack.top(), currTop, S[i])  <= 0)){
                currTop = pointStack.top(); pointStack.pop();
        }
        pointStack.push(currTop);
        pointStack.push(S[i]);

        if((process_path != "") && (!(i%10) || i==n-1 )){
                    
            vector<point> hull; 
            stack<point> tmpS = pointStack; 
            while(!tmpS.empty()){
                hull.push_back(tmpS.top()); tmpS.pop();
            }

            reverse(hull.begin(), hull.end());
        
            save_vector(hull, process_path  + "/" + to_string(i) + ".txt");
        }

    }


    vector<point> hull; 
    while(!pointStack.empty()){
        hull.push_back(pointStack.top()); pointStack.pop();
    }

    reverse(hull.begin(), hull.end());

    return hull;

}

int main(int argc, char* argv[]){
    if(argc < 3 || 4 < argc ){
        cerr << "Usage: " << argv[0] << "<input_file> <output_file>" << endl; 
        return 1; 
    }

    string in_path = argv[1], 
           out_path = argv[2];
    
    process_path = (argc == 4)? argv[3]:"";


           cout << process_path << endl;

    vector<point> vec;
    try{
        load_vector(vec, in_path);
        vector<point> convex_hull = graham_scan(vec);
        save_vector(convex_hull, out_path);
    } catch (const std::exception &e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;

}