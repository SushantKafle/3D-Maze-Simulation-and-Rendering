#include <vector>
#include <math.h>
using namespace std;

typedef struct vertex
{
    float x,y,z;
    float I;
};

class dpoint
{
    public:
    float Intensity;
    int r,b,g;
    bool set;
    float depth;
    dpoint()
    {
        set=false;
        r=0,b=0,g=0;
        Intensity=0;
    }

};

class edge
{
    public:
    vertex *v[2];
    edge()
    {
        v[0]=new vertex();
        v[1]=new vertex();
    }
};

class surface
{
    public:
    vertex *v[10];
    int r,g,b;
    float A,B,C,D;
    surface(float n) //int
    {
        for(int i=0;i<n;i++)
        {
            v[i]=new vertex();
        }
        r=0;g=0;b=0;
        A=0;
        B=0;
        C=0;
        D=0;
    }
};

