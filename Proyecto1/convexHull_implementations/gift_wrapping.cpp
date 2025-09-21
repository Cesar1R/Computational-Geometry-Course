#include"geo_prop.h"
#include<iostream>
#include<vector>
#include<algorithm>
#include<set>

using namespace std;
using namespace geoProp;
using console_operations::print_vector; 
using file_operations::save_vector; 
using file_operations::load_vector; 

vector<point> gift_wrapping(vector<point> & S){

    int n = (int) S.size();
    if(n <= 3) return S;

    //Eliminacion de puntos duplicados
    vectorProcessing::removeDuplicated(S);
    n = (int) S.size();

    int leftmost = 0;
    for(int i=1; i<n; i++){
        if(S[i].x < S[leftmost].x || 
           fabs(S[i].x - S[leftmost].x < EPS && S[i].y < S[leftmost].y)){
            leftmost = i;
        }
    }

    vector<point> convex_hull;
    int current = leftmost; 
    int prev = -1; 

    do{
        if(convex_hull.empty() || S[current] != convex_hull.back()){
            convex_hull.push_back(S[current]);
        }
        
        int next = (current + 1)% S.size();
        for(int i=1; i < (int)S.size(); i++){
            if(i == current || i ==prev) continue;

            double cross_val = cross(S[current], S[i], S[next]);
            if(cross_val > EPS //Si el giro es en el sentido deseado
               || (fabs(cross_val) < EPS && dist(S[current], S[i]) > dist(S[current], S[next]))){ // En caso de colinearidad, elegimos el mas lejano
                next = i;
            }
        }

        prev = current; 
        current = next;
    }while(current != 0 && convex_hull.size() <= n);
    
    vectorProcessing::removeDuplicated(convex_hull);
    return convex_hull; 
}

int main(int argc, char* argv[]){
    if(argc != 3){
        cerr << "Usage: " << argv[0] << "<input_file> <output_file>" << endl; 
        return 1; 
    }

    string in_path = argv[1],
           out_path = argv[2];

	vector<point> vec; 

    try{
        load_vector(vec, in_path); 

        vector<point> convex_hull = gift_wrapping(vec);
        
        save_vector(convex_hull, out_path);
    } catch (const std::exception &e){
        cerr << "Error: " << e.what() << endl;
    }
    

    return 0;
}


