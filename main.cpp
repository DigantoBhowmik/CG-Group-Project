#include<cstdio>
#include <windows.h>
#include <GL/glut.h>
#include<math.h>>

#define PI           3.14159265358979323846
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080


void sun_set() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //SKY START
    glBegin(GL_QUADS);
    glColor3ub(220, 134, 48);
    glVertex2f(0,0);
    glVertex2f(0,1080);
    glVertex2f(1920,1080);
    glVertex2f(1920,0);
    glEnd();
    //SKY END

    //RIVER START
    glBegin(GL_QUADS);
    glColor3ub(190, 30, 45);
    glVertex2f(0,745);
    glVertex2f(0,1080);
    glVertex2f(1920,1080);
    glVertex2f(1920,745);
    glEnd();
    //RIVER END

    //LEFT-BOTTOM HILL START
    glBegin(GL_POLYGON);
    glColor3ub(148, 36, 40);
    glVertex2f(435,1080);
    glVertex2f(425,1050);
    glVertex2f(325,1007);
    glVertex2f(280,943);
    glVertex2f(160,925);
    glVertex2f(0,860);
    glVertex2f(0,1080);
    glEnd();

    glBegin(GL_POLYGON);  //SHADOW
    glColor3ub(136, 30, 34);
    glVertex2f(310,1080);
    glVertex2f(310,1010);
    glVertex2f(265,975);
    glVertex2f(180,975);
    glVertex2f(0,860);
    glVertex2f(0,975);
    glVertex2f(65,1030);
    glVertex2f(110,1040);
    glEnd();

    //LEFT-BOTTOM HILL END

    //RIGHT-BOTTOM HILL START

    glBegin(GL_POLYGON);
    glColor3ub(148, 36, 40);
    glVertex2f(1920,910);
    glVertex2f(1740,885);
    glVertex2f(1560,925);
    glVertex2f(1320,1020);
    glVertex2f(1290,1055);
    glVertex2f(1190,1055);
    glVertex2f(1182,1080);
    glVertex2f(1920,1080);
    glEnd();

    glBegin(GL_POLYGON);  //SHADOW
    glColor3ub(136, 30, 34);
    glVertex2f(1460,1080);
    glVertex2f(1547,1000);
    glVertex2f(1615,1010);
    glVertex2f(1615,960);
    glVertex2f(1645,960);
    glVertex2f(1665,907);
    glVertex2f(1560,925);
    glVertex2f(1430,985);
    glVertex2f(1380,1080);
    glEnd();

    glBegin(GL_POLYGON);  //SHADOW
    glColor3ub(136, 30, 34);
    glVertex2f(1920,910);
    glVertex2f(1740,885);
    glVertex2f(1730,911);
    glVertex2f(1774,956);
    glVertex2f(1792,1010);
    glVertex2f(1744,1080);
    glVertex2f(1920,1080);
    glEnd();

    //RIGHT-BOTTOM HILL END

    glBegin(GL_POLYGON);
    glColor3ub(148, 36, 40);
    glVertex2f(503,968);
    glVertex2f(475,944);
    glVertex2f(463,945);
    glVertex2f(447,935);
    glVertex2f(423,944);
    glVertex2f(392,968);
    glEnd();

    glBegin(GL_POLYGON);  //SHADOW
    glColor3ub(136, 30, 34);
    glVertex2f(447,935);
    glVertex2f(443,944);
    glVertex2f(392,968);
    glVertex2f(428,968);
    glVertex2f(435,957);
    glVertex2f(443,954);
    glVertex2f(440,946);
    glEnd();
    //LEFT HILL START
    glBegin(GL_POLYGON);
    glColor3ub(170, 43, 41);
    glVertex2f(660,745);
    glVertex2f(605,700);
    glVertex2f(560,693);
    glVertex2f(447,648);
    glVertex2f(375,685);
    glVertex2f(186,569);
    glVertex2f(0,660);
    glVertex2f(0,745);
    glEnd();
    glBegin(GL_POLYGON);  //SHADOW
    glColor3ub(196, 76, 88);
    glVertex2f(428,724);
    glVertex2f(470,699);
    glVertex2f(453,688);
    glVertex2f(466,668);
    glVertex2f(447,648);
    glVertex2f(427,660);
    glVertex2f(364,745);
    glVertex2f(428,745);
    glEnd();
    glBegin(GL_POLYGON);  //SHADOW
    glColor3ub(196, 76, 88);
    glVertex2f(140,720);
    glVertex2f(161,681);
    glVertex2f(155,650);
    glVertex2f(187,606);
    glVertex2f(186,569);
    glVertex2f(0,660);
    glVertex2f(0,745);
    glVertex2f(140,745);
    glEnd();
    ///////////////
    glPushMatrix();
    glTranslatef(-106,-67,0);
    glBegin(GL_POLYGON);
    glColor3ub(148, 36, 40);
    glVertex2f(503,968);
    glVertex2f(475,944);
    glVertex2f(463,945);
    glVertex2f(447,935);
    glVertex2f(423,944);
    glVertex2f(392,968);
    glEnd();

    glBegin(GL_POLYGON);  //SHADOW
    glColor3ub(136, 30, 34);
    glVertex2f(447,935);
    glVertex2f(443,944);
    glVertex2f(392,968);
    glVertex2f(428,968);
    glVertex2f(435,957);
    glVertex2f(443,954);
    glVertex2f(440,946);
    glEnd();
    glLoadIdentity();
    glPopMatrix();

    //////////////
    glPushMatrix();
    glTranslatef(679,0,0);
    glBegin(GL_POLYGON);
    glColor3ub(148, 36, 40);
    glVertex2f(503,968);
    glVertex2f(475,944);
    glVertex2f(463,945);
    glVertex2f(447,935);
    glVertex2f(423,944);
    glVertex2f(392,968);
    glEnd();

    glBegin(GL_POLYGON);  //SHADOW
    glColor3ub(136, 30, 34);
    glVertex2f(447,935);
    glVertex2f(443,944);
    glVertex2f(392,968);
    glVertex2f(428,968);
    glVertex2f(435,957);
    glVertex2f(443,954);
    glVertex2f(440,946);
    glEnd();
    glLoadIdentity();
    glPopMatrix();

    //////////////
    glPushMatrix();
    glTranslatef(794,-5,0);
    glBegin(GL_POLYGON);
    glColor3ub(148, 36, 40);
    glVertex2f(503,968);
    glVertex2f(475,944);
    glVertex2f(463,945);
    glVertex2f(447,935);
    glVertex2f(423,944);
    glVertex2f(392,968);
    glEnd();

    glBegin(GL_POLYGON);  //SHADOW
    glColor3ub(136, 30, 34);
    glVertex2f(447,935);
    glVertex2f(443,944);
    glVertex2f(392,968);
    glVertex2f(428,968);
    glVertex2f(435,957);
    glVertex2f(443,954);
    glVertex2f(440,946);
    glEnd();

    glPopMatrix();

    //////////////
    glPushMatrix();
    glTranslatef(794,-44,0);
    glBegin(GL_POLYGON);
    glColor3ub(148, 36, 40);
    glVertex2f(503,968);
    glVertex2f(475,944);
    glVertex2f(463,945);
    glVertex2f(447,935);
    glVertex2f(423,944);
    glVertex2f(392,968);
    glEnd();

    glBegin(GL_POLYGON);  //SHADOW
    glColor3ub(136, 30, 34);
    glVertex2f(447,935);
    glVertex2f(443,944);
    glVertex2f(392,968);
    glVertex2f(428,968);
    glVertex2f(435,957);
    glVertex2f(443,954);
    glVertex2f(440,946);
    glEnd();
    glLoadIdentity();
    glPopMatrix();


    //BOAT START
    glPushMatrix();
    glTranslated(75,0,0);
    glLineWidth(5);
    glBegin(GL_LINES);
    glColor3ub(129,42,53);
    glVertex2f(620, 802);
    glVertex2f(620, 771);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(119, 22, 26);
    glVertex2f(675,835);
    glVertex2f(698,802);
    glVertex2f(560,802);
    glVertex2f(560,835);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(119, 22, 26);
    glVertex2f(725,782);
    glVertex2f(620,638);
    glVertex2f(620,709);
    glVertex2f(587,660);
    glVertex2f(542,781);
    glEnd();
    glLoadIdentity();
    glPopMatrix();

    glPushMatrix();
    glTranslated(980,0,0);
    glLineWidth(5);
    glBegin(GL_LINES);
    glColor3ub(129,42,53);
    glVertex2f(620, 802);
    glVertex2f(620, 771);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(119, 22, 26);
    glVertex2f(675,835);
    glVertex2f(698,802);
    glVertex2f(560,802);
    glVertex2f(560,835);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(119, 22, 26);
    glVertex2f(725,782);
    glVertex2f(620,638);
    glVertex2f(620,709);
    glVertex2f(587,660);
    glVertex2f(542,781);
    glEnd();
    glLoadIdentity();
    glPopMatrix();

    glFlush();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutCreateWindow("Test");
	glutInitWindowSize(320, 320);
	glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, -1);
	glutDisplayFunc(sun_set);
	glutMainLoop();
	return 0;
}
