//=============================================================================================
// Szamitogepes grafika hazi feladat keret. Ervenyes 2013-tol.
// A //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sorokon beluli reszben celszeru garazdalkodni, mert a tobbit ugyis toroljuk.
// A beadott program csak ebben a fajlban lehet, a fajl 1 byte-os ASCII karaktereket tartalmazhat.
// Tilos:
// - mast "beincludolni", illetve mas konyvtarat hasznalni
// - faljmuveleteket vegezni (printf is fajlmuvelet!)
// - new operatort hivni az onInitialization függvényt kivéve, a lefoglalt adat korrekt felszabadítása nélkül
// - felesleges programsorokat a beadott programban hagyni
// - tovabbi kommenteket a beadott programba irni a forrasmegjelolest kommentjeit kiveve
// ---------------------------------------------------------------------------------------------
// A feladatot ANSI C++ nyelvu forditoprogrammal ellenorizzuk, a Visual Studio-hoz kepesti elteresekrol
// es a leggyakoribb hibakrol (pl. ideiglenes objektumot nem lehet referencia tipusnak ertekul adni)
// a hazibeado portal ad egy osszefoglalot.
// ---------------------------------------------------------------------------------------------
// A feladatmegoldasokban csak olyan gl/glu/glut fuggvenyek hasznalhatok, amelyek
// 1. Az oran a feladatkiadasig elhangzottak ES (logikai AND muvelet)
// 2. Az alabbi listaban szerepelnek:
// Rendering pass: glBegin, glVertex[2|3]f, glColor3f, glNormal3f, glTexCoord2f, glEnd, glDrawPixels
// Transzformaciok: glViewport, glMatrixMode, glLoadIdentity, glMultMatrixf, gluOrtho2D,
// glTranslatef, glRotatef, glScalef, gluLookAt, gluPerspective, glPushMatrix, glPopMatrix,
// Illuminacio: glMaterialfv, glMaterialfv, glMaterialf, glLightfv
// Texturazas: glGenTextures, glBindTexture, glTexParameteri, glTexImage2D, glTexEnvi,
// Pipeline vezerles: glShadeModel, glEnable/Disable a kovetkezokre:
// GL_LIGHTING, GL_NORMALIZE, GL_DEPTH_TEST, GL_CULL_FACE, GL_TEXTURE_2D, GL_BLEND, GL_LIGHT[0..7]
//
// NYILATKOZAT
// ---------------------------------------------------------------------------------------------
// Nev    : KIS ISTVAN
// Neptun : Z7E5X1
// ---------------------------------------------------------------------------------------------
// ezennel kijelentem, hogy a feladatot magam keszitettem, es ha barmilyen segitseget igenybe vettem vagy
// mas szellemi termeket felhasznaltam, akkor a forrast es az atvett reszt kommentekben egyertelmuen jeloltem.
// A forrasmegjeloles kotelme vonatkozik az eloadas foliakat es a targy oktatoi, illetve a
// grafhazi doktor tanacsait kiveve barmilyen csatornan (szoban, irasban, Interneten, stb.) erkezo minden egyeb
// informaciora (keplet, program, algoritmus, stb.). Kijelentem, hogy a forrasmegjelolessel atvett reszeket is ertem,
// azok helyessegere matematikai bizonyitast tudok adni. Tisztaban vagyok azzal, hogy az atvett reszek nem szamitanak
// a sajat kontribucioba, igy a feladat elfogadasarol a tobbi resz mennyisege es minosege alapjan szuletik dontes.
// Tudomasul veszem, hogy a forrasmegjeloles kotelmenek megsertese eseten a hazifeladatra adhato pontokat
// negativ elojellel szamoljak el es ezzel parhuzamosan eljaras is indul velem szemben.
//=============================================================================================

#include <math.h>
#include <stdlib.h>

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Innentol modosithatod...

//--------------------------------------------------------
// 3D Vektor
//--------------------------------------------------------
struct Vector {
   float x, y, z;

   Vector( ) {
	x = y = z = 0;
   }
   Vector(float x0, float y0, float z0 = 0) {
	x = x0; y = y0; z = z0;
   }
   Vector operator*(float a) {
	return Vector(x * a, y * a, z * a);
   }
   Vector operator/(float a) {
	return Vector(x / a, y / a, z / a);
   }
   Vector operator+(const Vector& v) {
 	return Vector(x + v.x, y + v.y, z + v.z);
   }
   Vector operator-(const Vector& v) {
 	return Vector(x - v.x, y - v.y, z - v.z);
   }
   float operator*(const Vector& v) { 	// dot product
	return (x * v.x + y * v.y + z * v.z);
   }
   Vector operator%(const Vector& v) { 	// cross product
	return Vector(y*v.z-z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
   }
   float Length() { return sqrt(x * x + y * y + z * z); }

   };

//--------------------------------------------------------
// Spektrum illetve szin
//--------------------------------------------------------
struct Color {
   float r, g, b;

   Color( ) {
	r = g = b = 0;
   }
   Color(float r0, float g0, float b0) {
	r = r0; g = g0; b = b0;
   }
   Color operator*(float a) {
	return Color(r * a, g * a, b * a);
   }
   Color operator*(const Color& c) {
	return Color(r * c.r, g * c.g, b * c.b);
   }
   Color operator+(const Color& c) {
 	return Color(r + c.r, g + c.g, b + c.b);
   }
};

class CurveCR {
        public:
            Vector cp[100];
            Vector v[100];
            float t[100];
            int n, arrowSegment;
            float arrowTime;
            Vector a,b,c,d;
            bool flag;
            float scalar;
    CurveCR(){
        n=0; arrowTime = 0; arrowSegment = 0; flag = true; scalar=0.3;
    }
    float absolute(float f) {
        if (f<0) return f*-1;
        if (f>=0) return f;
    }
    void addElement(float x, float y) {
            if (n<100) {
                cp[n] = Vector(x,y);
                t[n]=n+sin((float)n/5.0)*0.4;
                n++;
                flag = true;
                arrowSegment = 0;
                arrowTime = t[0];
            }
        }
    void calcVelocityInControlPoints(){
            v[0]=Vector(0,0);
            v[n-1]=Vector(0,0);
            for(int i=1; i<n-1; i++) {
                v[i] = ((cp[i]-cp[i-1]) / (t[i] - t[i-1]) + (cp[i+1]-cp[i])/(t[i+1]-t[i]))/2;
            }
    }
    Vector r_calc(float t_moment, int segment) {
            float td = t[segment+1] - t[segment];
            float actual_td = t_moment - t[segment];
            a = (v[segment+1]+v[segment]-(cp[segment+1]-cp[segment])*2/td)/td/td;
            b = ((cp[segment+1]-cp[segment])/td*3-(v[segment+1]+v[segment]*2))/td;
            c = v[segment];
            d = cp[segment];
            Vector ret = a*actual_td*actual_td*actual_td + b*actual_td*actual_td + c*actual_td + d;
            return ret;
    }
    Vector v_calc(float t_moment, int segment) {
        float actual_td = t_moment - t[segment];
        Vector vret = a*actual_td*actual_td*3 + b*actual_td*2 + c;
        return vret;
    }
    void drawControlPoints() {
        for(int point=0; point<n; point++) {
            glColor3f(1, 0, 0);
            glBegin(GL_TRIANGLES);
                glVertex2f(cp[point].x-10, cp[point].y+10);
                glVertex2f(cp[point].x+10, cp[point].y+10);
                glVertex2f(cp[point].x, cp[point].y);
            glEnd();
        }
    }
    void drawAllPoints() {
        glColor3f(1, 0, 0);
        glBegin(GL_LINE_STRIP);
            for (int i=0; i<n-1; i++ ) {
                for (int j=0; j<1000; j++) {
                    float tpart = t[i] + (float)j/999 * (t[i+1]-t[i]);
                    Vector p_actual = r_calc(tpart,i);
                    glVertex2f(p_actual.x,p_actual.y);
                }
            }
        glEnd();
    }
    void drawArrow() {
            float angle = 0;
            float eps = 5;
            if (n>=2) {
            if(flag) {
                scalar=absolute(scalar);
            } else {
                scalar=absolute(scalar)*(-1);
            }
                Vector p = r_calc(arrowTime,arrowSegment);
                Vector tmp = v_calc(arrowTime,arrowSegment)*(scalar);
                Vector p2 = tmp + p;
                glColor3f(1, 1, 1);
                glBegin(GL_LINES);
                    glVertex2f(p.x,p.y);
                    glVertex2f(p2.x,p2.y);
                glEnd();
                angle = atan2(tmp.y,tmp.x)*180/M_PI;
                glPushMatrix();
                glTranslatef(p2.x,p2.y,0.0);
                glRotatef(angle-90,0,0,1);
                glTranslatef(-(p2.x),-(p2.y),0.0);
                glBegin(GL_TRIANGLES);
                        glVertex2f(p2.x - eps,p2.y - eps);
                        glVertex2f(p2.x + eps,p2.y - eps);
                        glVertex2f(p2.x,p2.y);
                glEnd();
                glRotatef(-angle+90,0,0,1);
                glPopMatrix();
            }
    }
    void animateArrow(float dt) {
            if (n>=2) {
                if (arrowSegment >= n-1){
                    flag = false;
                    arrowSegment = n-2;
                    arrowTime = t[n-1];
                }
                if (arrowSegment < 0){
                    flag = true;
                    arrowSegment = 0;
                    arrowTime = t[0];
                }
                if((arrowSegment < n-1) && flag) {
                    arrowTime+=dt;
                    if (arrowTime > t[arrowSegment+1]) {
                        arrowSegment++;
                    }
                }
                if((arrowSegment >= 0) && !flag) {
                    arrowTime-=dt;
                    if (arrowTime <= t[arrowSegment]) {
                        arrowSegment--;
                    }
                }
            }
    }
};

class CurveCRI {
        public:
            Vector cp[100];
            Vector v[100];
            float t[100];
            int n, arrowSegment;
            float arrowTime;
            Vector a,b,c,d;
            bool flag;
            float scalar;
    CurveCRI(){
        n=0; arrowTime = 0; arrowSegment = 0; flag = false; scalar=0.3;
    }
    float absolute(float f) {
        if (f<0) return f*-1;
        if (f>=0) return f;
    }
    void addElement(float x, float y) {
            if (n<100) {
                cp[n] = Vector(x,y);
                t[n]=n+sin((float)n/5.0)*0.4;
                n++;
                flag = false;
                arrowSegment = n-2;
                arrowTime = t[n-1];
            }
        }
    void calcVelocityInControlPoints(){
            v[0]=Vector(10,10);
            v[n-1]=Vector(10,10);
            for(int i=1; i<n-1; i++) {
                v[i] = ((cp[i+1]-cp[i-1]) / (t[i+1] - t[i-1]));
            }
    }
    Vector r_calc(float t_moment, int segment) {
            float td = t[segment+1] - t[segment];
            float actual_td = t_moment - t[segment];
            a = (v[segment+1]+v[segment]-(cp[segment+1]-cp[segment])*2/td)/td/td;
            b = ((cp[segment+1]-cp[segment])/td*3-(v[segment+1]+v[segment]*2))/td;
            c = v[segment];
            d = cp[segment];
            Vector ret = a*actual_td*actual_td*actual_td + b*actual_td*actual_td + c*actual_td + d;
            return ret;
    }
    Vector v_calc(float t_moment, int segment) {
        float actual_td = t_moment - t[segment];
        Vector vret = a*actual_td*actual_td*3 + b*actual_td*2 + c;
        return vret;
    }
    void drawControlPoints() {
        for(int point=0; point<n; point++) {
            glColor3f(1, 0, 0);
            glBegin(GL_TRIANGLES);
                glVertex2f(cp[point].x-10, cp[point].y+10);
                glVertex2f(cp[point].x+10, cp[point].y+10);
                glVertex2f(cp[point].x, cp[point].y);
            glEnd();
        }
    }
    void drawAllPoints() {
        glColor3f(0, 1, 0);
        glBegin(GL_LINE_STRIP); //összekötögeti a pontokat
            for (int i=0; i<n-1; i++ ) {
                for (int j=0; j<1000; j++) {
                    float tpart = t[i] + (float)j/999 * (t[i+1]-t[i]);
                    Vector p_actual = r_calc(tpart,i);
                    glVertex2f(p_actual.x,p_actual.y);
                }
            }
        glEnd();
    }
    void drawArrow() {
            float angle = 0;
            float eps = 5;
            if (n>=2) {
            if(flag) {
                scalar=absolute(scalar);
            } else {
                scalar=absolute(scalar)*(-1);
            }
                Vector p = r_calc(arrowTime,arrowSegment);
                Vector tmp = v_calc(arrowTime,arrowSegment)*(scalar);
                Vector p2 = tmp + p;
                glColor3f(1, 1, 1);
                glBegin(GL_LINES);
                    glVertex2f(p.x,p.y);
                    glVertex2f(p2.x,p2.y);
                glEnd();
                angle = atan2(tmp.y,tmp.x)*180/M_PI;
                glPushMatrix();
                glTranslatef(p2.x,p2.y,0.0);
                glRotatef(angle-90,0,0,1);
                glTranslatef(-(p2.x),-(p2.y),0.0);
                glBegin(GL_TRIANGLES);
                        glVertex2f(p2.x - eps,p2.y - eps);
                        glVertex2f(p2.x + eps,p2.y - eps);
                        glVertex2f(p2.x,p2.y);
                glEnd();
                glRotatef(-angle+90,0,0,1);
                glPopMatrix();
            }
    }
    void animateArrow(float dt) {
            if (n>=2) {
                if (arrowSegment >= n-1){
                    flag = false;
                    arrowSegment = n-2;
                    arrowTime = t[n-1];
                }
                if (arrowSegment < 0){
                    flag = true;
                    arrowSegment = 0;
                    arrowTime = t[0];
                }
                if((arrowSegment < n-1) && flag) {
                    arrowTime+=dt;
                    if (arrowTime > t[arrowSegment+1]) {
                        arrowSegment++;
                    }
                }
                if((arrowSegment >= 0) && !flag) {
                    arrowTime-=dt;
                    if (arrowTime <= t[arrowSegment]) {
                        arrowSegment--;
                    }
                }
            }
    }
};

const int screenWidth = 600;	// alkalmazás ablak felbontása
const int screenHeight = 600;
float old_time = 0;
int i = 0;
bool flag;
CurveCR cr_curve = CurveCR();
CurveCRI cri_curve = CurveCRI();
float w = 600;
Vector vcp = Vector (300,300);
Color image[screenWidth*screenHeight];	// egy alkalmazás ablaknyi kép

// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization( ) {
	glViewport(0, 0, screenWidth, screenHeight);

    // Peldakent keszitunk egy kepet az operativ memoriaba
    /*for(int Y = 0; Y < screenHeight; Y++)
		for(int X = 0; X < screenWidth; X++)
			image[Y*screenWidth + X] = Color((float)X/screenWidth, (float)Y/screenHeight, 0);*/

}

// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
void onDisplay( ) {
    glLoadIdentity();
    gluOrtho2D(vcp.x-w/2,vcp.x+w/2,vcp.y+w/2,vcp.y-w/2);
    glClearColor(0, 0, 0, 0);		// torlesi szin beallitasa
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // kepernyo torles

    cr_curve.calcVelocityInControlPoints();
    cr_curve.drawControlPoints();
    cr_curve.drawAllPoints();
    cr_curve.drawArrow();

    cri_curve.calcVelocityInControlPoints();
    cri_curve.drawControlPoints();
    cri_curve.drawAllPoints();
    cri_curve.drawArrow();

    glutSwapBuffers();     				// Buffercsere: rajzolas vege

}

// Billentyuzet esemenyeket lekezelo fuggveny
void onKeyboard(unsigned char key, int x, int y) {
    if (key == 'd') glutPostRedisplay( ); 		// d beture rajzold ujra a kepet
    if (key == 'z' || key == 'Z') {
        w*=0.5;
    }
    if (key == 'p' || key == 'P') {
        vcp.x-=w*0.1;
        vcp.y-=w*0.2;
    }
}

// Eger esemenyeket lekezelo fuggveny
void onMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT && state == GLUT_DOWN) {   // A GLUT_LEFT_BUTTON / GLUT_RIGHT_BUTTON illetve GLUT_DOWN / GLUT_UP
            cr_curve.addElement(x,y);
            cri_curve.addElement(x,y);
        }
    }


void simulateWorld(float tstart, float tend) {
    for (float ts=tstart; ts<=tend; ts+=0.00001) {
               float te = ((ts + 0.00001) < tend) ? (ts + 0.00001) : tend;
               cr_curve.animateArrow(te-ts);
               cri_curve.animateArrow(te-ts);
          }
}

// `Idle' esemenykezelo, jelzi, hogy az ido telik, az Idle esemenyek frekvenciajara csak a 0 a garantalt minimalis ertek
void onIdle( ) {
    float time = (float)glutGet(GLUT_ELAPSED_TIME)/1000;
    simulateWorld(old_time, time);
    old_time = time;
    glutPostRedisplay( );
}

// ...Idaig modosithatod
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// A C++ program belepesi pontja, a main fuggvenyt mar nem szabad bantani
int main(int argc, char **argv) {
    glutInit(&argc, argv); 				// GLUT inicializalasa
    glutInitWindowSize(600, 600);			// Alkalmazas ablak kezdeti merete 600x600 pixel
    glutInitWindowPosition(100, 100);			// Az elozo alkalmazas ablakhoz kepest hol tunik fel
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);	// 8 bites R,G,B,A + dupla buffer + melyseg buffer

    glutCreateWindow("Grafika hazi feladat");		// Alkalmazas ablak megszuletik es megjelenik a kepernyon

    glMatrixMode(GL_MODELVIEW);				// A MODELVIEW transzformaciot egysegmatrixra inicializaljuk
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);			// A PROJECTION transzformaciot egysegmatrixra inicializaljuk
    glLoadIdentity();

    onInitialization();					// Az altalad irt inicializalast lefuttatjuk

    glutDisplayFunc(onDisplay);				// Esemenykezelok regisztralasa
    glutMouseFunc(onMouse);
    glutIdleFunc(onIdle);
    glutKeyboardFunc(onKeyboard);

    glutMainLoop();					// Esemenykezelo hurok

    return 0;
}
