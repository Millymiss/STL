#include<iostream>               /*  Author:  Bowen Li  */
#include<fstream>
#include<vector>
#include<string>
#include<cmath>
using namespace std;

static constexpr double pi = 3.1415926;

class Vec3d {
private:
    double x, y, z;
public:
    Vec3d(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {
    }

    ~ Vec3d() {
    }

    Vec3d(const Vec3d &orig) : x(orig.x), y(orig.y), z(orig.z) {
    }

    Vec3d &operator=(const Vec3d &orig) {
        x = orig.x;
        y = orig.y;
        z = orig.z;
        return *this;
    }

    Vec3d operator +(const Vec3d& v){
        return Vec3d(x+v.x, y+v.y, z+v.z);
    }

    Vec3d operator -(const Vec3d &v) {
        return Vec3d(x - v.x, y - v.y, z - v.z);
    }

    Vec3d operator*(float a) {
        return Vec3d(x * a, y * a, z * a);
    }

    friend ostream& operator <<(ostream& s, const Vec3d& v);

    friend Vec3d Normal(const Vec3d& v1, const Vec3d& v2, const Vec3d& v3);
};

ostream& operator <<(ostream& s, const Vec3d& v) {
    s << v.x << " " << v.y << " " << v.z;
    return s;
}

Vec3d Normal(const Vec3d& v1, const Vec3d& v2, const Vec3d& v3) {
    double a = ((v2.y - v1.y) * (v3.z - v1.z) - (v2.z - v1.z) * (v3.y - v1.y));
    double b = ((v2.z - v1.z) * (v3.x - v1.x) - (v2.x - v1.x) * (v3.z - v1.z));
    double c = ((v2.x - v1.x) * (v3.y - v1.y) - (v2.y - v1.y) * (v3.x - v1.x));
    return Vec3d(a, b, c);
}

class Shape {
private:
    double x, y, z;
public:
    Shape(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}

    double getX() {
        return x;
    }

    double getY() {
        return y;
    }

    double getZ() {
        return z;
    }

    virtual void write(string Name) = 0;

    virtual ~Shape() {}
};

class Cylinder : public Shape {
private:
    double r, h;
    int facets;
public:
    Cylinder(double x, double y, double z, double r, double h, int facets) : Shape(x, y, z), r(r), h(h),
                                                                             facets(facets) {
    }

    void write(string Name) override {
        ofstream s(Name, ofstream::app);
        if (s) {
            Vec3d v1_u(getX(), getY(), getZ() + h / 2);
            Vec3d v2_u(getX() + r * cos(0), getY() + r * sin(0), getZ() + h / 2);
            Vec3d v3_u;
            Vec3d v1_d(getX(), getY(), getZ() - h / 2);
            Vec3d v2_d(getX() + r * cos(0), getY() + r * sin(0), getZ() - h / 2);
            Vec3d v3_d;
            for (int i = 0; i < facets; i++) {
                v3_u = Vec3d(getX() + r * cos(2 * pi / facets * (1 + i)), getY() + r * sin(2 * pi / facets * (1 + i)),
                             getZ() + h / 2);
                //facet 1 upper circle
                Vec3d f1 = Normal(v1_u, v2_u, v3_u);
                s << " " << "facet normal " << f1 << endl;
                s << "  " << "outer loop " << endl;
                s << "   " << "vertex " << v1_u << endl;
                s << "   " << "vertex " << v2_u << endl;
                s << "   " << "vertex " << v3_u << endl;
                s << "  " << "endloop" << endl;
                s << " " << "endfacet" << endl;
                //facet 2 under circle
                v3_d = Vec3d(getX() + r * cos(2 * pi / facets * (1 + i)), getY() + r * sin(2 * pi / facets * (1 + i)),
                             getZ() - h / 2);
                Vec3d f2 = Normal(v3_d, v2_d, v1_d);
                s << " " << "facet normal " << f2 << endl;
                s << "  " << "outer loop " << endl;
                s << "   " << "vertex " << v3_d << endl;
                s << "   " << "vertex " << v2_d << endl;
                s << "   " << "vertex " << v1_d << endl;
                s << "  " << "endloop" << endl;
                s << " " << "endfacet" << endl;
                //facet 3 upper triangle v2 v3_d v3
                Vec3d f3 = Normal(v2_u, v3_d, v3_u);
                s << " " << "facet normal " << f3 << endl;
                s << "  " << "outer loop " << endl;
                s << "   " << "vertex " << v2_u << endl;
                s << "   " << "vertex " << v3_d << endl;
                s << "   " << "vertex " << v3_u << endl;
                s << "  " << "endloop" << endl;
                s << " " << "endfacet" << endl;
                //facet 4  under triangle v2 v2_d v3_d
                Vec3d f4 = Normal(v2_u, v2_d, v3_d);
                s << " " << "facet normal " << f4 << endl;
                s << "  " << "outer loop " << endl;
                s << "   " << "vertex " << v2_u << endl;
                s << "   " << "vertex " << v2_d << endl;
                s << "   " << "vertex " << v3_d << endl;
                s << "  " << "endloop" << endl;
                s << " " << "endfacet" << endl;
                v2_u = v3_u;
                v2_d = v3_d;
            }
            s.close();
        }
    }
};

class Cube : public Shape {
private:
    double size;
public:
    Cube(double x, double y, double z, double size) : Shape(x, y, z), size(size) {}

    void write(string Name) override {
        ofstream s(Name, ofstream::app);
        if (s) {
            Vec3d v1_u(getX() + size / 2, getY() - size / 2, getZ() + size / 2);
            Vec3d v2_u(getX() + size / 2, getY() + size / 2, getZ() + size / 2);
            Vec3d v3_u(getX() - size / 2, getY() + size / 2, getZ() + size / 2);
            Vec3d v4_u(getX() - size / 2, getY() - size / 2, getZ() + size / 2);
            Vec3d v1_d(getX() + size / 2, getY() - size / 2, getZ() - size / 2);
            Vec3d v2_d(getX() + size / 2, getY() + size / 2, getZ() - size / 2);
            Vec3d v3_d(getX() - size / 2, getY() + size / 2, getZ() - size / 2);
            Vec3d v4_d(getX() - size / 2, getY() - size / 2, getZ() - size / 2);
            Vec3d f;
            for (int i = 0; i < 12; i++) {
                switch (i) {
                    case 0://upper triangle 1: 2_u 3_u 4_u
                        f = Normal(v2_u, v3_u, v4_u);
                        s << " " << "facet normal " << f << endl;
                        s << "  " << "outer loop" << endl;
                        s << "   " << "vertex " << v2_u << endl;
                        s << "   " << "vertex " << v3_u << endl;
                        s << "   " << "vertex " << v4_u << endl;
                        s << "  " << "endloop" << endl;
                        s << " " << "endfacet" << endl;
                        break;
                    case 1://upper triangle 2: 1_u 2_u 4_u
                        f = Normal(v1_u, v2_u, v4_u);
                        s << " " << "facet normal " << f << endl;
                        s << "  " << "outer loop" << endl;
                        s << "   " << "vertex " << v1_u << endl;
                        s << "   " << "vertex " << v2_u << endl;
                        s << "   " << "vertex " << v4_u << endl;
                        s << "  " << "endloop" << endl;
                        s << " " << "endfacet" << endl;
                        break;
                    case 2://under triangle 1: 4_d 3_d 2_d
                        f = Normal(v4_d, v3_d, v2_d);
                        s << " " << "facet normal " << f << endl;
                        s << "  " << "outer loop" << endl;
                        s << "   " << "vertex " << v4_d << endl;
                        s << "   " << "vertex " << v3_d << endl;
                        s << "   " << "vertex " << v2_d << endl;
                        s << "  " << "endloop" << endl;
                        s << " " << "endfacet" << endl;
                        break;
                    case 3://under triangle 2: 1_d 4_d 2_d
                        f = Normal(v1_d, v4_d, v2_d);
                        s << " " << "facet normal " << f << endl;
                        s << "  " << "outer loop" << endl;
                        s << "   " << "vertex " << v1_d << endl;
                        s << "   " << "vertex " << v4_d << endl;
                        s << "   " << "vertex " << v2_d << endl;
                        s << "  " << "endloop" << endl;
                        s << " " << "endfacet" << endl;
                        break;
                    case 4://left triangle 1: 4_d 1_d 1_u
                        f = Normal(v4_d, v1_d, v1_u);
                        s << " " << "facet normal " << f << endl;
                        s << "  " << "outer loop" << endl;
                        s << "   " << "vertex " << v4_d << endl;
                        s << "   " << "vertex " << v1_d << endl;
                        s << "   " << "vertex " << v1_u << endl;
                        s << "  " << "endloop" << endl;
                        s << " " << "endfacet" << endl;
                        break;
                    case 5://left triangle 2: 1_u 4_u 4_d
                        f = Normal(v1_u, v4_u, v4_d);
                        s << " " << "facet normal " << f << endl;
                        s << "  " << "outer loop" << endl;
                        s << "   " << "vertex " << v1_u << endl;
                        s << "   " << "vertex " << v4_u << endl;
                        s << "   " << "vertex " << v4_d << endl;
                        s << "  " << "endloop" << endl;
                        s << " " << "endfacet" << endl;
                        break;
                    case 6://right triangle 1: 2_u 3_d 3_u
                        f = Normal(v2_u, v3_d, v3_u);
                        s << " " << "facet normal " << f << endl;
                        s << "  " << "outer loop" << endl;
                        s << "   " << "vertex " << v2_u << endl;
                        s << "   " << "vertex " << v3_d << endl;
                        s << "   " << "vertex " << v3_u << endl;
                        s << "  " << "endloop" << endl;
                        s << " " << "endfacet" << endl;
                        break;
                    case 7://right triangle 2: 2_u 2_d 3_d
                        f = Normal(v2_u, v2_d, v3_d);
                        s << " " << "facet normal " << f << endl;
                        s << "  " << "outer loop" << endl;
                        s << "   " << "vertex " << v2_u << endl;
                        s << "   " << "vertex " << v2_d << endl;
                        s << "   " << "vertex " << v3_d << endl;
                        s << "  " << "endloop" << endl;
                        s << " " << "endfacet" << endl;
                        break;
                    case 8://front triangle 1: 2_d 2_u 1_u
                        f = Normal(v2_d, v2_u, v1_u);
                        s << " " << "facet normal " << f << endl;
                        s << "  " << "outer loop" << endl;
                        s << "   " << "vertex " << v2_d << endl;
                        s << "   " << "vertex " << v2_u << endl;
                        s << "   " << "vertex " << v1_u << endl;
                        s << "  " << "endloop" << endl;
                        s << " " << "endfacet" << endl;
                        break;
                    case 9://front triangle 2: 1_u 1_d 2_d
                        f = Normal(v1_u, v1_d, v2_d);
                        s << " " << "facet normal " << f << endl;
                        s << "  " << "outer loop" << endl;
                        s << "   " << "vertex " << v1_u << endl;
                        s << "   " << "vertex " << v1_d << endl;
                        s << "   " << "vertex " << v2_d << endl;
                        s << "  " << "endloop" << endl;
                        s << " " << "endfacet" << endl;
                        break;
                    case 10://back triangle 1: 4_d 4_u 3_d
                        f = Normal(v4_d, v4_u, v3_d);
                        s << " " << "facet normal " << f << endl;
                        s << "  " << "outer loop" << endl;
                        s << "   " << "vertex " << v4_d << endl;
                        s << "   " << "vertex " << v4_u << endl;
                        s << "   " << "vertex " << v3_d << endl;
                        s << "  " << "endloop" << endl;
                        s << " " << "endfacet" << endl;
                        break;
                    case 11://back triangle 2: 4_u 3_u 3_d
                        f = Normal(v4_u, v3_u, v3_d);
                        s << " " << "facet normal " << f << endl;
                        s << "  " << "outer loop" << endl;
                        s << "   " << "vertex " << v4_u << endl;
                        s << "   " << "vertex " << v3_u << endl;
                        s << "   " << "vertex " << v3_d << endl;
                        s << "  " << "endloop" << endl;
                        s << " " << "endfacet" << endl;
                        break;
                }

            }
            s.close();
        }
    }
};

class CAD {
private:
    vector<Shape *> shapes;
public:
    CAD() {}

    ~CAD() {
        for (int i = 0; i < shapes.size(); i++)
            delete shapes[i];
    }

    CAD(const CAD &orig) = delete;

    CAD &operator=(const CAD &orig)= delete;

    void add(Shape *s) {
        shapes.push_back(s);
    }

    void write(string Name) {
        ofstream s(Name);
        if (s) {
            auto n = Name.find('.');
            string str = Name.substr(0, n);
            s << "solid " << str << endl;
            s.close();
            for (int i = 0; i < shapes.size(); i++)
                shapes[i]->write(Name);
            ofstream s(Name, ofstream::app);
            s << "endsolid " << str << endl;
            s.close();
        }
    }
};

int main() {
    CAD c;
    c.add(new Cube(0, 0, 0, 5));
    c.add(new Cylinder(100, 0, 0, 3, 10, 10));
    c.write("test.stl");
}