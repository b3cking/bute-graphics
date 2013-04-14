//=============================================================================================
// Szamitogepes grafika hazi feladat keret. Ervenyes 2012-tol.
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
// Nev    : Kis Istvan
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

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
// MsWindows-on ez is kell
#include <windows.h>
#endif // Win32 platform

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Innentol modosithatod...

const int screenWidth = 600;	// alkalmazás ablak felbontása
const int screenHeight = 600;
float old_time = 0;
int tegla_kiutve = 0;

float coordProjecter(float p) {
    return p/25;
}

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

//Az oldalsó fal

class Fal {
    public:
        float xsize,ysize, limit;
        Vector pos;

    Fal(){
        xsize = ysize = limit = 0;
        pos = Vector(0,0);
    }
    Fal(float x0, float y0, float xs, float ys, float l) {
          limit = l;
          pos.x=x0; pos.y=y0;
          xsize=xs; ysize=ys;
    }
    void draw() {

        glColor3f(1,1,1);
          glBegin(GL_QUADS);
            glVertex2f(pos.x-xsize/2,pos.y-ysize/2);
            glVertex2f(pos.x+xsize/2,pos.y-ysize/2);
            glVertex2f(pos.x+xsize/2,pos.y+ysize/2);
            glVertex2f(pos.x-xsize/2,pos.y+ysize/2);
          glEnd();
    }
};

Fal fal1 = Fal(coordProjecter(0),coordProjecter(-19.5),coordProjecter(40),coordProjecter(1), coordProjecter(-19));
Fal fal2 = Fal(coordProjecter(0),coordProjecter(19.5),coordProjecter(40),coordProjecter(1), coordProjecter(19));

//Kiüthető téglák

class Tegla {
        public:
        float xsize,ysize;
        Vector pos;
        Vector v;
        float tomeg;

    Tegla(){
        xsize = ysize = 0;
        pos = Vector(0,0);
        v = Vector(0,0);
        tomeg = 0;
    }
    Tegla(float x0, float y0, float xs, float ys) {
          xsize=xs; ysize=ys;
          pos.x=x0;pos.y=y0;
          v = Vector(0,0);
          tomeg = 10;
    }
    void draw() {
        glColor3f(1,0,0);
          glBegin(GL_QUADS);
            glVertex2f(pos.x-xsize/2,pos.y-ysize/2);
            glVertex2f(pos.x+xsize/2,pos.y-ysize/2);
            glVertex2f(pos.x+xsize/2,pos.y+ysize/2);
            glVertex2f(pos.x-xsize/2,pos.y+ysize/2);
          glEnd();
    }
    void animate(float dt) {
          pos = pos - v*dt;

          if (pos.y>=1) {
               pos.y=1;
               v.y*=-1;
          }
    }
};

Tegla tegla1 = Tegla(coordProjecter(-19.5),coordProjecter(16),coordProjecter(1),coordProjecter(8));
Tegla tegla2 = Tegla(coordProjecter(-19.5),coordProjecter(8),coordProjecter(1),coordProjecter(8));
Tegla tegla3 = Tegla(coordProjecter(-19.5),coordProjecter(0),coordProjecter(1),coordProjecter(8));
Tegla tegla4 = Tegla(coordProjecter(-19.5),coordProjecter(-8),coordProjecter(1),coordProjecter(8));
Tegla tegla5 = Tegla(coordProjecter(-19.5),coordProjecter(-16),coordProjecter(1),coordProjecter(8));

//Ütő

class Uto {
        public:
        float xsize,ysize;
        Vector pos;
        Vector v; //~sebesseg, nem pont az
        float sloppy;

    Uto(){
        xsize = ysize = 0;
        pos = Vector(0,0);
        v = Vector(0,0);
        sloppy = 1;
    }
    Uto(float x0, float y0, float xs, float ys, float vx, float vy) {
          xsize=xs; ysize=ys;
          pos.x=x0;pos.y=y0;
          v.x = vx; v.y = vy;
          sloppy = (float)xsize / 8;
    }
    void draw() {
        glColor3f(0,1,0);
          glBegin(GL_QUADS);
            glVertex2f(pos.x-xsize/2,pos.y-ysize/2);
            glVertex2f(pos.x+xsize/2,pos.y-ysize/2);
            glVertex2f(pos.x+xsize/2,pos.y+ysize/2);
            glVertex2f(pos.x-xsize/2,pos.y+ysize/2);
          glEnd();
    }
    void animate(float dt) {
          if (pos.y>=1) {
               pos.y=1;
               v.y*=-1;
          }
    }
};

Uto uto = Uto(coordProjecter(20.5),coordProjecter(0),coordProjecter(1),coordProjecter(8), 0.06, 0.06);

//Labda

class Labda {
    public:
        float vmax;
        Vector pos;
        Vector v;
        float r;
        float tomeg;

    Labda(){
        pos = Vector(0,0);
        v = Vector(0,0);
        r = 0;
        tomeg = 0;
        vmax = 0;
    }
    Labda(float x0, float y0, float vx0, float vy0, float r0) {
          pos.x=x0; pos.y=y0; v.x=vx0; v.y=vy0; r=r0;
          tomeg = 1;
          vmax = 4;
          }

    void draw() {
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(1,1,0);
        glVertex2f(pos.x, pos.y);
        for (int i=0;i<100; i++) {
            float t=(float)i/99*2*M_PI;
            glVertex2f(pos.x+r*cos(t),pos.y+r*sin(t));
        }
        glEnd();
        }

    void teglaUtkozes(Tegla &tegla) {
        if ((tegla.pos.y+tegla.ysize/2 > pos.y-r) && (tegla.pos.y-tegla.ysize/2 < pos.y+r) && (tegla.pos.x+tegla.xsize/2 >= pos.x-r) ) {

                tegla.v.x=(float)-v.x*9.09/50;
                v.x=(float)-1*40.9/50*v.x;
                tegla.v.y=(float)-v.y*9.09/50;
                v.y=(float)1*40.9/50*v.y;
                pos.x=tegla.pos.x+r+tegla.xsize;
                tegla_kiutve++;
                if (tegla_kiutve == 5) {
                        exit(0);
                }
            }
    };

    void utoUtkozes(Uto &uto) {
        float temp;
        if (uto.pos.x-uto.xsize/2-r <= pos.x &&
            uto.pos.x >= pos.x &&
            uto.pos.y+uto.ysize/2-uto.sloppy >= pos.y &&
            uto.pos.y-uto.ysize/2+uto.sloppy <= pos.y  ){
                        pos.x = uto.pos.x-uto.xsize/2-r;
                        if (v.Length()*1.2<vmax) {
                            v.x=v.x*-1.2;
                            v.y=v.y*1.2;
                        }
                        else {
                            v.x=v.x*-1;
                        }
        }
        else if (uto.pos.x+uto.xsize/2+r >= pos.x &&
                 uto.pos.x <= pos.x &&
                uto.pos.y+uto.ysize/2-uto.sloppy >= pos.y &&
                uto.pos.y-uto.ysize/2+uto.sloppy <= pos.y  ) {
                        pos.x = uto.pos.x+uto.xsize/2+r;
                        if (v.Length()*1.2<vmax) {
                            v.x=v.x*-1.2;
                            v.y=v.y*1.2;
                        }
                        else {
                            v.x=v.x*-1;
                        }
        }
        else if (uto.pos.x-uto.xsize/2-r <= pos.x &&
                uto.pos.x >= pos.x &&
                uto.pos.y+uto.ysize/2-uto.sloppy <= pos.y &&
                uto.pos.y+uto.ysize/2+r >= pos.y)
                {
                        pos.x = uto.pos.x-uto.xsize/2-r;
                        if (v.Length()*1.2<vmax) {
                            v.x=v.Length()*1.2*sin(M_PI/4)*-1;
                            v.y=v.Length()*1.2*cos(M_PI/4);
                        } else {
                            v.x=v.Length()*sin(M_PI/4)*-1;
                            v.y=v.Length()*cos(M_PI/4);
                        }
        }
         else if (uto.pos.x+uto.xsize/2+r >= pos.x &&
                uto.pos.x <= pos.x &&
                uto.pos.y+uto.ysize/2-uto.sloppy <= pos.y &&
                uto.pos.y+uto.ysize/2+r >= pos.y)
                {
                        pos.x = uto.pos.x+uto.xsize/2+r;
                        if (v.Length()*1.2<vmax) {
                            v.x=v.Length()*1.2*sin(M_PI/4);
                            v.y=v.Length()*1.2*cos(M_PI/4);
                        } else {
                            v.x=v.Length()*sin(M_PI/4);
                            v.y=v.Length()*cos(M_PI/4);
                        }


        }
        else if (uto.pos.x-uto.xsize/2-r <= pos.x &&
                uto.pos.x >= pos.x &&
                uto.pos.y-uto.ysize/2+uto.sloppy >= pos.y &&
                uto.pos.y-uto.ysize/2-r <= pos.y)
                {
                        pos.x = uto.pos.x-uto.xsize/2-r;
                        if (v.Length()*1.2<vmax) {
                            v.x=v.Length()*1.2*sin(M_PI/4)*-1;
                            v.y=v.Length()*1.2*cos(M_PI/4)*-1;
                        } else {
                            v.x=v.Length()*sin(M_PI/4)*-1;
                            v.y=v.Length()*cos(M_PI/4)*-1;
                        }

        }
        else if (uto.pos.x+uto.xsize/2+r >= pos.x &&
                uto.pos.x <= pos.x &&
                uto.pos.y-uto.ysize/2+uto.sloppy >= pos.y &&
                uto.pos.y-uto.ysize/2-r <= pos.y)
                {
                        pos.x = uto.pos.x+uto.xsize/2+r;
                        if (v.Length()*1.2<vmax) {
                            v.x=v.Length()*1.2*sin(M_PI/4);
                            v.y=v.Length()*1.2*cos(M_PI/4)*-1;
                        } else {
                            v.x=v.Length()*sin(M_PI/4);
                            v.y=v.Length()*cos(M_PI/4)*-1;
                        }

        }
    }
    void animate(float dt) {
          pos = pos + v*dt;
          if ((pos.y+r > fal2.limit) && pos.x-r<=fal2.pos.x+fal2.xsize/2)  {
            v.y*=-1;
            pos.y=fal2.limit-r;
          }
          if ((pos.y-r < fal1.limit) && pos.x-r<=fal1.pos.x+fal2.xsize/2) {
            v.y*=-1;
            pos.y=fal1.limit+r;
          }
          teglaUtkozes(tegla1);
          teglaUtkozes(tegla2);
          teglaUtkozes(tegla3);
          teglaUtkozes(tegla4);
          teglaUtkozes(tegla5);
          utoUtkozes(uto);

          if (pos.x>=1) {
               exit(0);
          }
          if (pos.x-r<=coordProjecter(-19)) {
               pos.x=coordProjecter(-19)+r;
               v.x*=-1;
          }
     }
};

Labda labda = Labda(coordProjecter(-12.31),coordProjecter(7),coordProjecter(10),coordProjecter(13),coordProjecter(1));

Color image[screenWidth*screenHeight];	// egy alkalmazás ablaknyi kép


// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization( ) {
	glViewport(0, 0, screenWidth, screenHeight);

    // Peldakent keszitunk egy kepet az operativ memoriaba
    for(int Y = 0; Y < screenHeight; Y++)
		for(int X = 0; X < screenWidth; X++)
			image[Y*screenWidth + X] = Color((float)X/screenWidth, (float)Y/screenHeight, 0);

}

// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
void onDisplay( ) {
    glClearColor(0,0,0,0);		// torlesi szin beallitasa
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // kepernyo torles

    // ..

    // Peldakent atmasoljuk a kepet a rasztertarba
    //glDrawPixels(screenWidth, screenHeight, GL_RGB, GL_FLOAT, image);
    fal1.draw();
    fal2.draw();
    labda.draw();
    tegla1.draw();
    tegla2.draw();
    tegla3.draw();
    tegla4.draw();
    tegla5.draw();
    uto.draw();
    glutSwapBuffers();     				// Buffercsere: rajzolas vege

}

// Billentyuzet esemenyeket lekezelo fuggveny
void onKeyboard(unsigned char key, int x, int y) {

    if (key == 'j') {
        if (!(uto.pos.x <= coordProjecter(-10))) {
            uto.pos.x=uto.pos.x-uto.v.x;
        }
    }
    if (key == 'k') {
        if (!(uto.pos.x > coordProjecter(20))) {
            uto.pos.x=uto.pos.x+uto.v.x;
        }
    }
    if (key == 'i') {
        if (!(uto.pos.y >= coordProjecter(14))) {
            uto.pos.y=uto.pos.y+uto.v.y;
        }
    }
    if (key == 'm') {
        if (!(uto.pos.y <= coordProjecter(-14))) {
                uto.pos.y=uto.pos.y-uto.v.y;
        }
    }
    glutPostRedisplay( );
}

// Eger esemenyeket lekezelo fuggveny
void onMouse(int button, int state, int x, int y) {
    //if (button == GLUT_LEFT && state == GLUT_DOWN)   // A GLUT_LEFT_BUTTON / GLUT_RIGHT_BUTTON illetve GLUT_DOWN / GLUT_UP
	//	glutPostRedisplay( ); 						 // Ilyenkor rajzold ujra a kepet
}

void simulateWorld(float tstart, float tend) {  //diszkrét időkezelés
    for (float ts=tstart; ts<=tend; ts+=0.00001) {
               float te = ((ts + 0.00001) < tend) ? (ts + 0.00001) : tend;
               labda.animate(te-ts);
               tegla1.animate(te-ts);
               tegla2.animate(te-ts);
               tegla3.animate(te-ts);
               tegla4.animate(te-ts);
               tegla5.animate(te-ts);
               uto.animate(te-ts);
          }
}
// `Idle' esemenykezelo, jelzi, hogy az ido telik, az Idle esemenyek frekvenciajara csak a 0 a garantalt minimalis ertek
void onIdle( ) {
    float time = (float)glutGet(GLUT_ELAPSED_TIME)/1000;		// program inditasa ota eltelt ido
    simulateWorld(old_time, time);
    glutPostRedisplay( );
    old_time = time; // elozo ido

     // idõszeletet itt lehet darabolni, ha tul nagy lenne, pl. utkozesdetekcio, vagy ilyesmi miatt problema lehet az animate-ben, atugorhatjak egymast, ezert daraboljuk

     // ezt bárhova írhatod a keretben, class sym blabla, es akkor az osszes ilyen fuggvenyt bele lehet tolni, vagy kozvetlen a for ciklust beleirod, hello


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
