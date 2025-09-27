#ifndef GEO_PROP_H
#define GEO_PROP_H

#include<iostream>
#include<vector>
#include<cmath>
#include<set>
#include<string>
#include<fstream>


inline constexpr double EPS = 1e-9;

namespace geoProp{ 
    
    struct point{
        double x, y;

        point() {x = y = 0.0;}
        point(double x_, double y_) : x(x_), y(y_) {}
        point(const point &p): x(p.x), y(p.y) {}

        point& operator=(const point& other){
            if(this != &other){
                x = other.x;
                y = other.y;
            }

            return *this;
        }

        point operator+(const point& other) const {
            return point(x + other.x, y + other.y);
        }

        
        point operator-(const point& other) const {
            return point(x - other.x, y - other.y);
        }

        bool operator<(const point& other) const {
            if(x != other.x){
                return x < other.x;
            }
            return y < other.y;
        }
        
        bool operator == (const point &other) const{
            return (std::fabs(x - other.x) < EPS && (std::fabs(y - other.y) < EPS));
        }

        bool operator != (const point &other) const {
            return !(*this == other);
        }

    }; 
    // bool operator == (const point &oter);

    double dist(const point &p1, const point &p2);

    struct vec;

    vec toVec(const point &a, const point &b);

    vec scale(const vec &v, double s);

    point translate(const point &p, const vec &v);

    double dot(const vec &a, const vec &b);

    double norm_sq(const vec &v);

    double distToLine(const point &p, const point &a, 
                      const point &b, point &c);

    double distToLineSegment(const point &p, const point &a, 
                             const point &b, point &c);

    double area(const std::vector<point> &P);

    //Test de convavidad
    double cross_(const vec &a, const vec &b);
    double cross(const point &a, const point &b, const point &c);

    bool ccw(const point &p, const point &q,
             const point &r, bool ref=true);

    bool isConvex(const std::vector<point> &P);

    //Test de pertenencia
    double angle(const point &a, const point &o, const point &b);

    bool inPolygon(const point &pt, const std::vector<point> &P);

    bool polarCompare(const point &a, const point &b, const point &pivot);

}

namespace vectorProcessing{
    void removeDuplicated(std::vector<geoProp::point> &vec);

    std::vector<geoProp::point> getUniquePoints(const std::vector<geoProp::point> &vec);
}

namespace console_operations{
    void print_vector(const std::vector<geoProp::point> &points);
    void print_set(const std::set<geoProp::point> &points);
}

namespace file_operations{
    bool load_vector(std::vector<geoProp::point> &points, const std::string &path);
    bool save_vector(const std::vector<geoProp::point> &points, const std::string &path);
}

#endif

