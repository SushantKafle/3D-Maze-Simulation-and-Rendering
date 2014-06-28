#include "primary.h"

float theta=45;
float l=0.75;
bool topView=false;
float test=0;
float anglex=0,angley=0,anglez=0;
char shadding='x';

dpoint dpoints[800][500];
int tx=0,ty=0,tz=0;
float roty=0,rotz=0,rotx=0;



void calcNormal(surface *s)
{
        vertex ver[3];
        ver[0]=*(s->v[0]);
        ver[1]=*(s->v[1]);
        ver[2]=*(s->v[2]);

        s->A= (ver[0].y*(ver[1].z - ver[2].z)) + (ver[1].y*(ver[2].z-ver[0].z)) + (ver[2].y*(ver[0].z-ver[1].z)) ;
        s->B= (ver[0].z*(ver[1].x - ver[2].x)) + (ver[1].z*(ver[2].x-ver[0].x)) + (ver[2].z*(ver[0].x-ver[1].x)) ;
        s->C= (ver[0].x*(ver[1].y - ver[2].y)) + (ver[1].x*(ver[2].y-ver[0].y)) + (ver[2].x*(ver[0].y-ver[1].y)) ;
        s->D= -1 * ( (ver[0].x*(ver[1].y*ver[2].z - ver[2].y*ver[1].z)) + (ver[1].x*(ver[2].y*ver[0].z - ver[0].y*ver[2].z)) + (ver[2].x*(ver[0].y*ver[1].z - ver[1].y*ver[0].z))) ;
}


void minmax (int* a, int i, int j, int* min, int* max) {
  int lmin, lmax, rmin, rmax, mid;
  if (i == j) {
    *min = a[i];
    *max = a[j];
  } else if (j == i + 1) {
    if (a[i] > a[j]) {
      *min = a[j];
      *max = a[i];
    } else {
      *min = a[i];
      *max = a[j];
    }
  } else {
    mid = (i + j) / 2;
    minmax(a, i, mid, &lmin, &lmax);
    minmax(a, mid + 1, j, &rmin, &rmax);
    *min = (lmin > rmin) ? rmin : lmin;
    *max = (lmax > rmax) ? lmax : rmax;
  }
}

float getIntensity(surface s)
{
    vertex source;
    source.x=10;source.y=10;source.z=0;
    //source.x=10;source.y=0;source.z=-240;
    vertex temp;
    temp.x=s.A;temp.y=s.B;temp.z=s.C;

    float I; //Abmient Intensity

    float Ia=1; //Intensity of light Source

    int ka=0.9; //merely reflective
    float mag_s=pow(((source.x*source.x)+(source.z*source.z)+(source.z*source.z)),0.5);
    float mag_t=pow(((temp.x*temp.x)+(temp.y*temp.y)+(temp.z*temp.z)),0.5);
    float result=((source.x*temp.x)+(source.y*temp.y)+(source.z*temp.z))/(mag_s*mag_t);

    //I=((float) rand()) / (float) RAND_MAX;

    I=fabs(result);

    return I;
}

float getIntensity(vertex v,float A,float B,float C)
{
    float x1=10,y1=10,z1=0;

   float x2=v.x,y2=v.y,z2=v.z;

    float mag1=pow((A*A+B*B+C*C),0.5);

    x2=(x2-x1);
    y2=(y2-y1);
    z2=(z2-z1);

    float mag2=pow((x2*x2+y2*y2+z2*z2),0.5);

    return  fabs((A*x2 + B*y2 + C*z2)/(mag1*mag2));
}

float getIntensity(surface s,float x2,float y2,float z2)
{    //float x1=10,y1=-20,z1=-20;
   float x1=10,y1=10,z1=0;
   float tempx=x2,tempy=y2,tempz=z2;

    calcNormal(&s);
    float A,B,C;
    A=s.A;B=s.B;C=s.C;

    float mag1=pow((A*A+B*B+C*C),0.5);

    x2=(x2-x1);
    y2=(y2-y1);
    z2=(z2-z1);

    float mag2=pow((x2*x2+y2*y2+z2*z2),0.5);

    float Id=0.1+fabs((A*x2 + B*y2 + C*z2)/(mag1*mag2));

    /*Calculate Half Vector
    Magnitude of N is mag1 and A,B,C are A,B,C*/

    float x3=400,y3=250,z3=200; //These are the view Points
    x3=(x3-tempx);
    y3=(y3-tempy);
    z3=(z3-tempz);
    float mag3=pow((x3*x3+y3*y3+z3*z3),0.5); //Magnitude of V vector
    //mag2 is magnitude of L vector
    //x2,y2,z2 are L vectors parameters

    float halfx,halfy,halfz; //Half vector

    halfx=(x3+x2);halfy=(y3+y2);halfz=(z3+z2);
    float magH=pow((halfx*halfx+halfy*halfy+halfz*halfz),0.5);
    halfx=halfx/magH;halfy=halfy/magH;halfz=halfz/magH;

    magH=pow((halfx*halfx+halfy*halfy+halfz*halfz),0.5); //Magnitude of H vector

    float Is=pow(fabs((A*halfx + B*halfy + C*halfz)/(mag1*magH)),1000);



    return  Id+Is;
}

vertex project(vertex point)
{
    float temp=(int)(22*theta)/(float)(180*7) ;

    point.x=(int)((point.x)+(point.z*l*cos(temp)));
    point.y=(int)((point.y)+(point.z*l*sin(temp)));

    return point;
}

void setShadding(char x)
{
    shadding=x;
}


class cube
{
    public:
    vertex v[8];
    edge e[12];
    surface *s[6];
    int red,green,blue; //Color of cube
    int transx,transy,transz;
    int rotx,rotz,roty;
    int centerx,centery, centerz,length,breadth,height;
    float scaleValue;
    cube()
    {

        scaleValue=1;
    }

    void init(int x,int y,int z,int l,int b,int h,int rr,int gg,int bb)
    {
        red=rr;
        green=gg;
        blue=bb;
        centerx=x;centery=y;centerz=z;length=l;breadth=b;height=h;

        for (int i=0;i<6;i++)
        for (int i=0;i<6;i++)
        {
            s[i]=new surface(4);
        }


        int rl[8]={1,2,5,6,0,3,7,4};
        int td[8]={3,2,7,6,4,0,1,5};
        int fb[8]={0,1,2,3,4,5,6,7};

        /*Initializing Vertex*/
        for(int i=0;i<8;i++)
        {
            if(i<4)
            {
            v[rl[i]].x=(x+(l/2));
            v[td[i]].y=(y+(h/2));
            v[fb[i]].z=(z+(b/2));
            }
            else
            {
            v[rl[i]].x=(x-(l/2));
            v[td[i]].y=(y-(h/2));
            v[fb[i]].z=(z-(b/2));
            }
        }

        /*Initializing Edge*/
        for(int i=0;i<8;i++)
        {
            e[i].v[0]=&v[i];
            if(i==3 || i==7)
                e[i].v[1]=&v[i-3];
            else
                e[i].v[1]=&v[i+1];

            if(i<4)
            {
            e[i+8].v[0]=&v[i];
            e[i+8].v[1]=&v[i+4];
            }
        }


        /*Initializing Surface*/
            s[0]->v[0]=&v[0];
            s[0]->v[1]=&v[1];
            s[0]->v[2]=&v[2];
            s[0]->v[3]=&v[3];

            s[1]->v[0]=&v[4];
            s[1]->v[1]=&v[5];
            s[1]->v[2]=&v[6];
            s[1]->v[3]=&v[7];

            s[2]->v[0]=&v[0];
            s[2]->v[1]=&v[3];
            s[2]->v[2]=&v[7];
            s[2]->v[3]=&v[4];

            s[3]->v[0]=&v[1];
            s[3]->v[1]=&v[2];
            s[3]->v[2]=&v[6];
            s[3]->v[3]=&v[5];

            s[4]->v[0]=&v[0];
            s[4]->v[1]=&v[1];
            s[4]->v[2]=&v[5];
            s[4]->v[3]=&v[4];

            s[5]->v[0]=&v[2];
            s[5]->v[1]=&v[3];
            s[5]->v[2]=&v[7];
            s[5]->v[3]=&v[6];

        /*Calculating Normal*/
//        for (int i=0;i<6;i++)
//        calcNormal(s[i]);

    }


//Implementation of Ray Tracing Algorithm
bool isInside(surface s,int x,int y)
    {

        bool result = false;
        int j = 3;
        for (int i = 0; i < 4; i++)
        {

            if (s.v[i]->y < y && s.v[j]->y >= y || s.v[j]->y < y && s.v[i]->y >= y)
            {
                if (s.v[i]->x + (y - s.v[i]->y) / (s.v[j]->y - s.v[i]->y) * (s.v[j]->x - s.v[i]->x) < x)
                {
                    result = !result;
                }
            }
            j = i;
        }
        return result;
    }




    void draw()
    {
        vertex p;
        surface *test=new surface(4);
        float I;

        if(shadding=='g')
        gouraudInit();

    for(int index=0;index<6;index++)
        {

            if(shadding=='f')
            {
                calcNormal(s[index]);
                I=getIntensity(*s[index]);
            }

           for(int k=0;k<4;k++)
            *test->v[k]=project(*s[index]->v[k]);

            calcNormal(test);

            int minx,maxx,miny,maxy;
            /*Bad Programming*/
            int temp[4]={test->v[0]->x,test->v[1]->x,test->v[2]->x,test->v[3]->x};
            minmax(temp,0,3,&minx,&maxx);
            int temp1[4]={test->v[0]->y,test->v[1]->y,test->v[2]->y,test->v[3]->y};
            minmax(temp1,0,3,&miny,&maxy);
            /*Bad Programming*/

                for(int y=miny;y<=maxy;y++)
                {

                        bool calc=true;
                        float m,c;
                        int t=0;
                        float tempI[2]={1,1};
                        float tempx[2]={0,0};
                        int tempv[2]={0,0};


                        for(int x=minx;x<=maxx;x++)
                        {
                            if(isInside(*test,x,y))
                            {
                                if(x<0 or x > 800 or y<0 or y > 500)
                                    continue;
                            if(shadding=='g')
                            {
                                int count=0;
                                for(int i=0;i<4;i++)
                                {
                                    float _x;
                                    if(test->v[(i+1)%4]->x==test->v[i]->x)
                                    _x=test->v[i]->x;
                                    else if(test->v[(i+1)%4]->y==test->v[i]->y)
                                    continue;
                                    else{
                                    m=(test->v[(i+1)%4]->y-test->v[i]->y)/(float)(test->v[(i+1)%4]->x-test->v[i]->x);
                                    c=test->v[i]->y-(m*test->v[i]->x);
                                    _x=((y-c)/m);
                                    }

                                    if( (_x > test->v[i]->x and _x>test->v[(i+1)%4]->x) or (_x < test->v[i]->x and _x< test->v[(i+1)%4]->x) )
                                        continue;

                                    tempI[count]= ((y-test->v[(i+1)%4]->y)*test->v[i]->I)/(test->v[i]->y-test->v[(i+1)%4]->y) + ((test->v[i]->y-y)*test->v[(i+1)%4]->I)/(test->v[i]->y-test->v[(i+1)%4]->y);
                                    tempx[count]=_x;
                                    tempv[count]=i;
                                    ++count;

                                    if(count >= 2)
                                    {
                                        if((int)tempx[0]==(int)tempx[1])
                                        count=1;
                                        else
                                            break;
                                    }

                                }
                            }


                            int z=getDepth(*test,x,y);

                            if(z > dpoints[x][y].depth || !dpoints[x][y].set )
                            {
                                    if(shadding=='g')
                                    I=((tempx[1]-x)*tempI[0])/(float)(tempx[1]-tempx[0])+((x-tempx[0])*tempI[1])/(float)(tempx[1]-tempx[0]);
                                    else if (shadding=='x')
                                    I=getIntensity(*test,x,y,z);

                                    dpoints[x][y].set=true;
                                    dpoints[x][y].Intensity=I;
                                    dpoints[x][y].depth=z;
                                    dpoints[x][y].r=red;dpoints[x][y].g=green;dpoints[x][y].b=blue;
                            }
                        }

                        }

                    }
                }

        }


    float getDepth(surface s,float x,float y)
    {
        float depth=(float)(-s.A*x-s.B*y-s.D)/(float)s.C;
        return depth;
    }



        void translate(char axis,char sign,int value)
    {

        int x,y,z,l,b,h,r,g,bb;
        cube temp;
        x=centerx;y=centery;z=centerz;
        l=length;b=breadth;h=height;
        r=red;bb=blue;g=green;
        temp.init(x,y,z,l,b,h,r,g,bb);

            if(scaleValue!=1)
        {
                for(int i=0;i<8;i++)
            {
            temp.v[i].x-=400;temp.v[i].y-=250;temp.v[i].z-=0;
            temp.v[i].y*=scaleValue;
            temp.v[i].z*=scaleValue;
            temp.v[i].x*=scaleValue;
            temp.v[i].x+=400;temp.v[i].y+=250;temp.v[i].z+=0;
            }
        }


        switch(axis)
        {
            case 'y':
            if(sign=='+')
            {
                for(int i=0;i<8;i++)
                {
                    temp.v[i].y+=value;
                }
                centery+=value;
            }else
            {
                for(int i=0;i<8;i++)
                {
                    temp.v[i].y-=value;
                }
                centery-=value;
            }
            break;

            case 'x':
            if(sign=='+')
            {
                for(int i=0;i<8;i++)
                {
                    temp.v[i].x+=value;
                }
                centerx+=value;
            }else
            {
                for(int i=0;i<8;i++)
                {
                    temp.v[i].x-=value;
                }
                centerx-=value;
            }
            break;

            case 'z':
            if(sign=='+')
            {
                for(int i=0;i<8;i++)
                {
                    temp.v[i].z+=value;
                }
                centerz+=value;
            }else
            {
                for(int i=0;i<8;i++)
                {
                    temp.v[i].z-=value;
                }
                centerz-=value;
            }
            break;

        }
        temp.draw();

    }

   void gouraudInit()
   {
        for(int k=0;k<8;k++)
        {
            float nA=0,nB=0,nC=0,deno=0;
            for( int i=0;i<6;i++)
            {
                for(int j=0;j<4;j++)
                {
                        if(s[i]->v[j]->x==v[k].x && s[i]->v[j]->y==v[k].y)
                        {
                            calcNormal(s[i]);
                            nA+=s[i]->A;nB+=s[i]->B;nC+=s[i]->C;
                            break;
                        }
                }
            }

            deno=pow((double)((nA*nA)+(nB*nB)+(nC*nC)),0.5);
            v[k].I=getIntensity(v[k],nA,nB,nC);
        }


   }


    void rotate(int deg,char axis='y')
    {
        int x,y,z,l,b,h,r,g,bb;
        cube temp;
        x=centerx;y=centery;z=centerz;
        l=length;b=breadth;h=height;
        r=red;bb=blue;g=green;
        temp.init(x,y,z,l,b,h,r,g,bb);

        if(scaleValue!=1)
        {
                for(int i=0;i<8;i++)
            {
            temp.v[i].x-=400;temp.v[i].y-=250;temp.v[i].z-=0;
            temp.v[i].y*=scaleValue;
            temp.v[i].z*=scaleValue;
            temp.v[i].x*=scaleValue;
            temp.v[i].x+=400;temp.v[i].y+=250;temp.v[i].z+=0;
            }
        }

        float angle=(float)(22*deg)/(float)(7*180);

        if(axis=='y')
            angley=angle;

            if(angley!=0)
            {
                    for(int i=0;i<8;i++)
            {
            temp.v[i].x-=400;temp.v[i].y-=250;temp.v[i].z-=0;
            temp.v[i].z=((temp.v[i].z*cos(angley))-(temp.v[i].x*sin(angley)));
            temp.v[i].x=((temp.v[i].z*sin(angley))+(temp.v[i].x*cos(angley)));
            temp.v[i].x+=400;temp.v[i].y+=250;temp.v[i].z+=0;
            }

            }

        if (axis=='z')
            anglez=angle;

            if(anglez!=0)
            {
                    for(int i=0;i<8;i++)
            {
            temp.v[i].x-=400;temp.v[i].y-=250;temp.v[i].z-=0;
            temp.v[i].x=((temp.v[i].x*cos(anglez))-(temp.v[i].y*sin(anglez)));
            temp.v[i].y=((temp.v[i].x*sin(anglez))+(temp.v[i].y*cos(anglez)));
            temp.v[i].x+=400;temp.v[i].y+=250;temp.v[i].z+=0;
            }
            }


        if(axis=='x')
            anglex=angle;

            if(anglex!=0)
            {
                    for(int i=0;i<8;i++)
            {
            temp.v[i].x-=400;temp.v[i].y-=250;temp.v[i].z-=0;
            temp.v[i].y=((temp.v[i].y*cos(anglex))-(temp.v[i].z*sin(anglex)));
            temp.v[i].z=((temp.v[i].y*sin(anglex))+(temp.v[i].z*cos(anglex)));
            temp.v[i].x+=400;temp.v[i].y+=250;temp.v[i].z+=0;
            }
            }

        temp.draw();
    }

    void scale(float value)
    {
        scaleValue=value;
        int x,y,z,l,b,h,r,g,bb;
        cube temp;
        x=centerx;y=centery;z=centerz;
        l=length;b=breadth;h=height;
        r=red;bb=blue;g=green;
        temp.init(x,y,z,l,b,h,r,g,bb);

        for(int i=0;i<8;i++)
        {
            temp.v[i].x-=400;temp.v[i].y-=250;temp.v[i].z-=0;
            temp.v[i].y*=value;
            temp.v[i].z*=value;
            temp.v[i].x*=value;
            temp.v[i].x+=400;temp.v[i].y+=250;temp.v[i].z+=0;
        }
            temp.draw();
    }

};


