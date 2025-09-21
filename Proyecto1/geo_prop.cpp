#include"geo_prop.h"
#include<cmath>
#include<algorithm>
// const double EPS = 1e-9;

namespace geoProp{
    double dist(const point &p1, const point &p2){
        return hypot(p1.x - p2.x, p1.y - p2.y);
    }

    struct vec{
        double x, y;

        vec(const double &_x, const double &_y) : x(_x), y(_y) {}
    };

    vec toVec(const point &a, const point &b){
        return vec(b.x - a.x, b.y - a.y);
    }

    vec scale(const vec &v, double s){
        return vec(v.x * s, v.y * s);
    }

    point translate(const point &p, const vec &v){
        return point(p.x + v.x, p.y + v.y);
    }

    double dot(const vec &a, const vec &b){
        return (a.x * b.x + a.y + b.y);
    }

    double norm_sq(const vec &v){
        return v.x * v.x + v.y * v.y; 
    }

    double distToLine(const point &p, const point &a, 
                      const point &b,  point &c){
        vec ap = toVec(a, p), ab = toVec(a, b);
        double u = dot(ap, ab) / norm_sq(ab);
        c = translate(a, scale(ab, u));
        return dist(p, c);
    }

    double distToLineSegment(const point &p, const point &a, 
                             const point &b, point &c){
        vec ap = toVec(a, p), ab = toVec(a, b);
        double u = dot(ap, ab) / norm_sq(ab);
        if(u < 0.0){
            c = point(a.x, a.y);
            return dist(p, a);
        }
        if (u > 1.0){
            c = point(b.x, b.y);
            return dist(p, b);
        }

        return distToLine(p, a, b, c);
    }

    double area(const std::vector<point> &P){
        double result = 0.0, x1, y1, x2, y2;
        for(int i=0; i<(int)P.size() -1; i++){
            x1 = P[i].x;
            x2 = P[i + 1].x;
            y1 = P[i].y;
            y2 = P[i + 1].y;
            result += (x1 * y2 - x2 * y1);
        }

        return std::fabs(result)/2.0;
    }

    //Test de convavidad
    double cross_(const vec &a, const vec &b){
        return a.x * b.y - a.y * b.x;
    }

    
    double cross(const point &a, const point &b, const point &c){
        vec ab = toVec(a, b), 
            ac = toVec(a, c);

        return cross_(ab, ac);
    }

    //Regresa true si son colineales 
    bool ccw(const point &p, const point &q,
             const point &r, bool ref){
        return ref?(cross_(toVec(p, q), toVec(p, r)) > -EPS):(cross_(toVec(p, q), toVec(p, r))<EPS);
    }

    bool isConvex(const std::vector<point> &P){
        int sz = (int)P.size();
        if(sz <= 3) return false;
        bool isLeft = ccw(P[0], P[1], P[2]);

        for(int i=1; i< sz - 1; i++){
            if(ccw(P[i], P[i+1], P[(i+1) == sz ? 1 : i+2]) != isLeft){
                return false;
            } 
        }
        return true;
    }

    //Test de pertenencia
    double angle(const point &a, const point &o, const point &b){
        vec oa = toVec(o, a), ob = toVec(o, b);
        return acos(dot(oa, ob) / sqrt(norm_sq(oa) * norm_sq(ob)));
    }

    bool inPolygon(const point &pt, const std::vector<point> &P){
        if((int)P.size() == 0) return false;
        double sum = 0.0;

        for(int i=0; i<(int)P.size() - 1; i++){
            if(ccw(pt, P[i], P[i+1])){
                sum += angle(P[i], pt, P[i+1]);
            } else{
                sum -= angle(P[i], pt, P[i+1]);
            }
        }

        return std::fabs(std::fabs(sum - 2 * M_PIl)) < EPS;
    }

}


namespace vectorProcessing{
    void removeDuplicated(std::vector<geoProp::point> &vec){
        std::sort(vec.begin(), vec.end());
        auto last = std::unique(vec.begin(), vec.end());
        vec.erase(last, vec.end());
    }

    std::vector<geoProp::point> getUniquePoints(const std::vector<geoProp::point> &vec){
        std::vector<geoProp::point> result = vec;
        std::sort(result.begin(), result.end());
        auto last = std::unique(result.begin(), result.end());
        result.erase(last, result.end());
        return result;
    }
}

namespace console_operations{
    void print_vector (const std::vector<geoProp::point> &points){
        for(const geoProp::point &p: points){
            std::cout << "(" << p.x << ", " << p.y << ")" << std::endl;
        }

        std::cout << std::endl;
    }

    void print_set(const std::set<geoProp::point> &points){
        for(const geoProp::point &p: points){
            std::cout << "(" << p.x << ", " << p.y << ")" << std::endl;
        }

        std::cout << std::endl;
    }

}

namespace file_operations{
    bool load_vector(std::vector<geoProp::point> &points, const std::string &path){
        std::ifstream infile(path);
        
        if(!infile.is_open()){
            throw std::runtime_error("Cannot open file: " + path); 
            return false; 
        }

        points.clear(); 
        char ch; 
        double x, y; 
        infile >> ch;
        while(infile >> ch >> x >> ch >> y >> ch >> ch){
            std::cout << x << " " << y << std::endl;
            points.emplace_back(x, y); 
        }

        
        std::cout << "Leyendo puntos: " << std::endl;
        console_operations::print_vector(points); 
        std::cout << "Conjunto de puntos recuperados de: " << path << std::endl;  

        return true;
    }

    bool save_vector(const std::vector<geoProp::point> &points, const std::string &path){
        std::ofstream outfile(path);

        if(!outfile.is_open()){
            throw std::runtime_error("Cannot open file: " + path); 
            return false; 
        }

        outfile << "[ ";
        for(size_t i = 0; i<points.size(); i++){
            outfile << "(" << points[i].x << "," << points[i].y << ")"; 
            if(i != points.size() - 1){
                outfile << ", "; 
            }
        }

        outfile << "]" << std::endl;

        std::cout << "Puntos a guardar: " << std::endl;
        console_operations::print_vector(points); 
        std::cout << "Conjunto de puntos guardado correctamente en: " << path << std::endl; 

        return true;
    }
}

// int main(void){
//     return 0;
// }