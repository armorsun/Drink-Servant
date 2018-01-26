#define PI 3.141592
#define R0 1250
#define R90 1920
#define L0 1450
#define L90 1950
#define _180 1590
#define _90 1180
#define _0 770

class Angle{
  public:
    //R
    void set_angleR(double t);
    void set_msR(int ms);
    double get_angleR(char type = 'd');
    int get_msR();
    
    //L
    void set_angleL(double t);
    void set_msL(int ms);
    double get_angleL(char type = 'd');
    int get_msL();
   
    //Rotate
    void set_angleRotate(double t);
    void set_msRotate(int ms);
    double get_angleRotate(char type = 'd');
    int get_msRotate();
  private:
    double thetaR;
    int msR;
    double thetaL;
    int msL;
    double thetaRotate;
    int msRotate;
    
};

