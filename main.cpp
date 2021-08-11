#include "./entities/cloud.h"
#include "./entities/ship.h"
#include "./lib.h"
#include <GL/gl.h>
#include <GL/glut.h>
#include <array>
#include <chrono>
#include <cmath>
#include <iostream>
#include <math.h>
#include <random>
#include <tuple>
#include <vector>

using namespace std;

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#define STAR_COUNT 300

#define LIMIT_FPS true
int FPS = 30;

Scene scene = Scene::sunset;

void drawSun() {
  glColor4ub(243, 238, 191, 255 * .05);
  drawFilledCircle(1170, 740, 190);
  glColor4ub(243, 238, 191, 255 * .1);
  drawFilledCircle(1170, 740, 160);
  glColor3ub(243, 238, 191);
  drawFilledCircle(1170, 740, 135);
}
void drawStar() {
  glColor4f(1, 1, 1, 0.1);
  drawFilledCircle(0, 0, .25);
  glColor3f(.8, .8, .8);
  drawFilledCircle(0, 0, .1);
}
struct STAR_META {
  int x;
  int y;
  int r;
};
void drawStars(GLfloat width, GLfloat height, int amount) {
  static vector<STAR_META> stars_meta;

  if (stars_meta.size() < amount) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> u_rand_x(0, width), u_rand_y(0, height),
        u_rand_r(15, 30);

    for (int n = 0; n < amount - stars_meta.size(); ++n) {
      int x = u_rand_x(gen), y = u_rand_y(gen), r = u_rand_r(gen);
      STAR_META star = {x, y, r};
      stars_meta.push_back(star);
    }
  }
  for (auto star_it = stars_meta.begin();
       star_it != stars_meta.begin() + amount; star_it++) {
    auto [x, y, r] = *star_it;
    glPushMatrix();
    glTranslatef(x, y, 0.f);
    glScalef(r, r, 1.0f);
    drawStar();
    glPopMatrix();
  }
}
void drawStars(Scene scene) {
  switch (scene) {
  case Scene::sunset:
    drawStars(WINDOW_WIDTH, 400, 20);
    break;
  case Scene::night:
    drawStars(WINDOW_WIDTH, 700, 200);
    break;
  }
}
void drawBridgeCables() {
  static bool ran_once = false;
  static vector<vector<Vertex>> interpolated_vertices;
  if (!ran_once) {
    vector<Vertex> cables[] = {
        {{455, 256}, {(1469.2 + 455) / 2, 650}, {1469.2, 256}},
        {{480.8, 423}, {(480.8 + 1447.1) / 2, 530}, {1447.1, 423}},
        {{0, 496}, {250, 496}, {405.2, 293.8}},
        {{0, 496}, {300, 496}, {405.2, 377}},
        {{1920, 496}, {1733, 496}, {1522.7, 302.2}},
        {{1920, 496}, {1733, 496}, {1522.7, 384.9}}};
    for (auto cable : cables) {
      interpolated_vertices.push_back(getQuadBezierCurvePoints(cable.data()));
    }
    ran_once = true;
  }
  auto cable_color = Hex2glRGB(0xbe1e2d);
  glColor3fv((GLfloat *)&cable_color);
  for (auto vertices : interpolated_vertices) {
    glLineWidth(8);
    glBegin(GL_LINE_STRIP);
    drawVertices(vertices);
    glEnd();
    for (int i = 1; i < vertices.size(); i += 10) {
      auto vertex = vertices[i];
      glLineWidth(5);
      drawLine(vertex.x, vertex.y, vertex.x, 505);
    }
  }
}
void drawBridgeTowers() {
  glColor3ub(182, 60, 74);
  drawQuad(407.8, 168, 13.5, 76.4);
  glColor3ub(149, 36, 49);
  drawQuad(407.8, 168, 9, 76.4);
  glColor3ub(182, 60, 74);
  drawQuad(449.9, 168, 13.5, 76.4);
  glColor3ub(149, 36, 49);
  drawQuad(449.9, 168, 9, 76.4);
  glColor3ub(149, 36, 49);
  drawQuad(407.8, 183.2, 55.6, 6.4);

  glColor3ub(182, 60, 74);
  drawQuad(396.5, 243.7, 17.4, 65.5);
  glColor3ub(149, 36, 49);
  drawQuad(396.5, 243.7, 11.6, 65.5);
  glColor3ub(182, 60, 74);
  drawQuad(450.8, 243.7, 17.4, 65.5);
  glColor3ub(149, 36, 49);
  drawQuad(450.8, 243.7, 11.6, 65.5);
  glColor3ub(149, 36, 49);
  drawQuad(396.5, 240.6, 71.7, 4.1);

  glColor3ub(182, 60, 74);
  drawQuad(386.1, 313.9, 22.4, 84.4);
  glColor3ub(149, 36, 49);
  drawQuad(386.1, 313.9, 15, 84.4);
  glColor3ub(182, 60, 74);
  drawQuad(456.1, 313.9, 22.4, 84.4);
  glColor3ub(149, 36, 49);
  drawQuad(456.1, 313.9, 15, 84.4);
  glColor3ub(149, 36, 49);
  drawQuad(386.1, 309, 92.5, 5.3);

  glColor3ub(182, 60, 74);
  drawQuad(372.7, 405, 28.9, 108.8);
  glColor3ub(149, 36, 49);
  drawQuad(372.7, 405, 19.3, 108.8);
  glColor3ub(182, 60, 74);
  drawQuad(463, 405, 28.9, 108.8);
  glColor3ub(149, 36, 49);
  drawQuad(463, 405, 19.3, 108.8);
  glColor3ub(149, 36, 49);
  drawQuad(372.7, 398.1, 119.2, 6.9);
}
void drawBridge() {
  drawBridgeCables();
  // left
  drawBridgeTowers();
  // right
  glPushMatrix();
  glTranslated(1062, 0, 0);
  drawBridgeTowers();
  glPopMatrix();

  // BRIDGE START
  glBegin(GL_QUADS);
  glColor3ub(149, 36, 49);
  glVertex2f(0, 560);
  glVertex2f(1920, 560);
  glVertex2f(1920, 500);
  glVertex2f(0, 500);
  glEnd();
  glBegin(GL_QUADS); // UPPER
  glColor3ub(182, 60, 74);
  glVertex2f(0, 510);
  glVertex2f(1920, 510);
  glVertex2f(1920, 500);
  glVertex2f(0, 500);
  glEnd();
  // LEFT PILLER START
  glBegin(GL_QUADS);
  glColor3ub(149, 36, 49);
  glVertex2f(325, 560);
  glVertex2f(325, 585);
  glVertex2f(415, 585);
  glVertex2f(415, 560);
  glEnd();
  glBegin(GL_QUADS);
  glColor3ub(149, 36, 49);
  glVertex2f(350, 585);
  glVertex2f(350, 745);
  glVertex2f(390, 745);
  glVertex2f(390, 585);
  glEnd();
  glBegin(GL_QUADS); // UPPER
  glColor3ub(182, 60, 74);
  glVertex2f(378, 585);
  glVertex2f(378, 745);
  glVertex2f(390, 745);
  glVertex2f(390, 585);
  glEnd();

  glPushMatrix();
  glTranslated(120, 0, 0);
  glBegin(GL_QUADS);
  glColor3ub(149, 36, 49);
  glVertex2f(325, 560);
  glVertex2f(325, 585);
  glVertex2f(415, 585);
  glVertex2f(415, 560);
  glEnd();
  glBegin(GL_QUADS);
  glColor3ub(149, 36, 49);
  glVertex2f(350, 585);
  glVertex2f(350, 745);
  glVertex2f(390, 745);
  glVertex2f(390, 585);
  glEnd();
  glBegin(GL_QUADS); // UPPER
  glColor3ub(182, 60, 74);
  glVertex2f(378, 585);
  glVertex2f(378, 745);
  glVertex2f(390, 745);
  glVertex2f(390, 585);
  glEnd();
  glPopMatrix();
  // LEFT PILLER END

  // RIGHT PILLER START
  glPushMatrix();
  glTranslated(1060, 0, 0);
  glBegin(GL_QUADS);
  glColor3ub(149, 36, 49);
  glVertex2f(325, 560);
  glVertex2f(325, 585);
  glVertex2f(415, 585);
  glVertex2f(415, 560);
  glEnd();
  glBegin(GL_QUADS);
  glColor3ub(149, 36, 49);
  glVertex2f(350, 585);
  glVertex2f(350, 745);
  glVertex2f(390, 745);
  glVertex2f(390, 585);
  glEnd();
  glBegin(GL_QUADS); // UPPER
  glColor3ub(182, 60, 74);
  glVertex2f(378, 585);
  glVertex2f(378, 745);
  glVertex2f(390, 745);
  glVertex2f(390, 585);
  glEnd();
  glPushMatrix();
  glTranslated(120, 0, 0);
  glBegin(GL_QUADS);
  glColor3ub(149, 36, 49);
  glVertex2f(325, 560);
  glVertex2f(325, 585);
  glVertex2f(415, 585);
  glVertex2f(415, 560);
  glEnd();
  glBegin(GL_QUADS);
  glColor3ub(149, 36, 49);
  glVertex2f(350, 585);
  glVertex2f(350, 745);
  glVertex2f(390, 745);
  glVertex2f(390, 585);
  glEnd();
  glBegin(GL_QUADS); // UPPER
  glColor3ub(182, 60, 74);
  glVertex2f(378, 585);
  glVertex2f(378, 745);
  glVertex2f(390, 745);
  glVertex2f(390, 585);
  glEnd();
  glPopMatrix();
  glPopMatrix();
  // RIGHT PILLER END
  // BRIDE END
}
void drawFarHill() {
  // LEFT HILL START
  glBegin(GL_POLYGON);
  glColor3ub(170, 43, 41);
  glVertex2f(660, 745);
  glVertex2f(605, 700);
  glVertex2f(560, 693);
  glVertex2f(447, 648);
  glVertex2f(375, 685);
  glVertex2f(186, 569);
  glVertex2f(0, 660);
  glVertex2f(0, 745);
  glEnd();
  glBegin(GL_POLYGON); // SHADOW
  glColor3ub(196, 76, 88);
  glVertex2f(428, 724);
  glVertex2f(470, 699);
  glVertex2f(453, 688);
  glVertex2f(466, 668);
  glVertex2f(447, 648);
  glVertex2f(427, 660);
  glVertex2f(364, 745);
  glVertex2f(428, 745);
  glEnd();
  glBegin(GL_POLYGON); // SHADOW
  glColor3ub(196, 76, 88);
  glVertex2f(140, 720);
  glVertex2f(161, 681);
  glVertex2f(155, 650);
  glVertex2f(187, 606);
  glVertex2f(186, 569);
  glVertex2f(0, 660);
  glVertex2f(0, 745);
  glVertex2f(140, 745);
  glEnd();
  // LEFT HILL END

  // RIGHT HILL START
  glPushMatrix();
  glTranslated(1920, 0, 0);
  glBegin(GL_POLYGON);
  glColor3ub(170, 43, 41);
  glVertex2f(-660, 745);
  glVertex2f(-605, 700);
  glVertex2f(-560, 693);
  glVertex2f(-447, 648);
  glVertex2f(-375, 685);
  glVertex2f(-186, 569);
  glVertex2f(0, 660);
  glVertex2f(0, 745);
  glEnd();
  glBegin(GL_POLYGON); // SHADOW
  glColor3ub(196, 76, 88);
  glVertex2f(-428, 724);
  glVertex2f(-470, 699);
  glVertex2f(-453, 688);
  glVertex2f(-466, 668);
  glVertex2f(-447, 648);
  glVertex2f(-427, 660);
  glVertex2f(-364, 745);
  glVertex2f(-428, 745);
  glEnd();
  glBegin(GL_POLYGON); // SHADOW
  glColor3ub(196, 76, 88);
  glVertex2f(-140, 720);
  glVertex2f(-161, 681);
  glVertex2f(-155, 650);
  glVertex2f(-187, 606);
  glVertex2f(-186, 569);
  glVertex2f(0, 660);
  glVertex2f(0, 745);
  glVertex2f(-140, 745);
  glEnd();
  glPopMatrix();
  // RIGHT HILL END
}
void drawNearHill() {
  // LEFT-BOTTOM HILL START
  glBegin(GL_POLYGON);
  glColor3ub(148, 36, 40);
  glVertex2f(435, 1080);
  glVertex2f(425, 1050);
  glVertex2f(325, 1007);
  glVertex2f(280, 943);
  glVertex2f(160, 925);
  glVertex2f(0, 860);
  glVertex2f(0, 1080);
  glEnd();

  glBegin(GL_POLYGON); // SHADOW
  glColor3ub(136, 30, 34);
  glVertex2f(310, 1080);
  glVertex2f(310, 1010);
  glVertex2f(265, 975);
  glVertex2f(180, 975);
  glVertex2f(0, 860);
  glVertex2f(0, 975);
  glVertex2f(65, 1030);
  glVertex2f(110, 1040);
  glEnd();

  // LEFT-BOTTOM HILL END

  // RIGHT-BOTTOM HILL START
  glBegin(GL_POLYGON);
  glColor3ub(148, 36, 40);
  glVertex2f(1920, 910);
  glVertex2f(1740, 885);
  glVertex2f(1560, 925);
  glVertex2f(1320, 1020);
  glVertex2f(1290, 1055);
  glVertex2f(1190, 1055);
  glVertex2f(1182, 1080);
  glVertex2f(1920, 1080);
  glEnd();

  glBegin(GL_POLYGON); // SHADOW
  glColor3ub(136, 30, 34);
  glVertex2f(1460, 1080);
  glVertex2f(1547, 1000);
  glVertex2f(1615, 1010);
  glVertex2f(1615, 960);
  glVertex2f(1645, 960);
  glVertex2f(1665, 907);
  glVertex2f(1560, 925);
  glVertex2f(1430, 985);
  glVertex2f(1380, 1080);
  glEnd();

  glBegin(GL_POLYGON); // SHADOW
  glColor3ub(136, 30, 34);
  glVertex2f(1920, 910);
  glVertex2f(1740, 885);
  glVertex2f(1730, 911);
  glVertex2f(1774, 956);
  glVertex2f(1792, 1010);
  glVertex2f(1744, 1080);
  glVertex2f(1920, 1080);
  glEnd();
  // RIGHT-BOTTOM HILL END
}
void drawMidRiverHill() {
  glPushMatrix();
  glTranslatef(0, 20, 0);
  glBegin(GL_POLYGON);
  glColor3ub(148, 36, 40);
  glVertex2f(503, 968);
  glVertex2f(475, 944);
  glVertex2f(463, 945);
  glVertex2f(447, 935);
  glVertex2f(423, 944);
  glVertex2f(392, 968);
  glEnd();

  glBegin(GL_POLYGON); // SHADOW
  glColor3ub(136, 30, 34);
  glVertex2f(447, 935);
  glVertex2f(443, 944);
  glVertex2f(392, 968);
  glVertex2f(428, 968);
  glVertex2f(435, 957);
  glVertex2f(443, 954);
  glVertex2f(440, 946);
  glEnd();
  glPopMatrix();
  /////////////////////
  glPushMatrix();
  glTranslatef(-90, -20, 0);
  glBegin(GL_POLYGON);
  glColor3ub(148, 36, 40);
  glVertex2f(503, 968);
  glVertex2f(475, 944);
  glVertex2f(463, 945);
  glVertex2f(447, 935);
  glVertex2f(423, 944);
  glVertex2f(392, 968);
  glEnd();

  glBegin(GL_POLYGON); // SHADOW
  glColor3ub(136, 30, 34);
  glVertex2f(447, 935);
  glVertex2f(443, 944);
  glVertex2f(392, 968);
  glVertex2f(428, 968);
  glVertex2f(435, 957);
  glVertex2f(443, 954);
  glVertex2f(440, 946);
  glEnd();
  glPopMatrix();

  //////////////
  glPushMatrix();
  glTranslatef(679, 50, 0);
  glBegin(GL_POLYGON);
  glColor3ub(148, 36, 40);
  glVertex2f(503, 968);
  glVertex2f(475, 944);
  glVertex2f(463, 945);
  glVertex2f(447, 935);
  glVertex2f(423, 944);
  glVertex2f(392, 968);
  glEnd();

  glBegin(GL_POLYGON); // SHADOW
  glColor3ub(136, 30, 34);
  glVertex2f(447, 935);
  glVertex2f(443, 944);
  glVertex2f(392, 968);
  glVertex2f(428, 968);
  glVertex2f(435, 957);
  glVertex2f(443, 954);
  glVertex2f(440, 946);
  glEnd();
  glPopMatrix();

  //////////////
  glPushMatrix();
  glTranslatef(794, 15, 0);
  glBegin(GL_POLYGON);
  glColor3ub(148, 36, 40);
  glVertex2f(503, 968);
  glVertex2f(475, 944);
  glVertex2f(463, 945);
  glVertex2f(447, 935);
  glVertex2f(423, 944);
  glVertex2f(392, 968);
  glEnd();

  glBegin(GL_POLYGON); // SHADOW
  glColor3ub(136, 30, 34);
  glVertex2f(447, 935);
  glVertex2f(443, 944);
  glVertex2f(392, 968);
  glVertex2f(428, 968);
  glVertex2f(435, 957);
  glVertex2f(443, 954);
  glVertex2f(440, 946);
  glEnd();

  glPopMatrix();

  //////////////
  glPushMatrix();
  glTranslatef(794, 60, 0);
  glBegin(GL_POLYGON);
  glColor3ub(148, 36, 40);
  glVertex2f(503, 968);
  glVertex2f(475, 944);
  glVertex2f(463, 945);
  glVertex2f(447, 935);
  glVertex2f(423, 944);
  glVertex2f(392, 968);
  glEnd();

  glBegin(GL_POLYGON); // SHADOW
  glColor3ub(136, 30, 34);
  glVertex2f(447, 935);
  glVertex2f(443, 944);
  glVertex2f(392, 968);
  glVertex2f(428, 968);
  glVertex2f(435, 957);
  glVertex2f(443, 954);
  glVertex2f(440, 946);
  glEnd();
  glPopMatrix();
}
void drawSky(unsigned int color) {
  auto _color = Hex2glRGB(color);
  glColor3fv((GLfloat *)&_color);
  drawQuad(0, 0, 1920, 1080);
}
void drawSky(Scene scene) {
  switch (scene) {
  case Scene::sunset:
    drawSky(0xD98539);
    break;
  case Scene::night:
    drawSky(0x403CD6);
    break;
  }
}
void drawRiver() {
  glBegin(GL_QUADS);
  glColor3ub(200, 63, 46);
  glVertex2f(0, 745);
  glVertex2f(0, 1080);
  glVertex2f(1920, 1080);
  glVertex2f(1920, 745);
  glEnd();

  glColor3ub(211, 112, 85);
  drawFilledElipsis(400, 800, 250, 10);
  drawFilledElipsis(900, 900, 400, 10);
  drawFilledElipsis(1385, 800, 450, 6);
  glColor3ub(217, 132, 126);
  drawFilledElipsis(1300, 965, 100, 10);
  glColor4ub(243, 238, 191, 50);
  drawFilledElipsis(1157, 747, 150, 15);
}
void drawShips() {
  static auto ship1 = Ship(0, 800, 60, 50, 0x75181D, HDirection::left)
                          .animate(0, WINDOW_WIDTH, 120);
  static auto ship2 = Ship(0, 730, 30, 25, 0x75181D, HDirection::right)
                          .animate(0, WINDOW_WIDTH, 60);
  ship1.draw();
  ship2.draw();
}
void drawCloud(Scene scene) {
  static auto cloud1 = Cloud(WINDOW_WIDTH, 50, 80, 30, HDirection::left)
                           .animate(0, WINDOW_WIDTH, 30);
  static auto cloud2 = Cloud(WINDOW_WIDTH / 3, 80, 50, 20, HDirection::left)
                           .animate(0, WINDOW_WIDTH, 40);
  static auto cloud3 = Cloud(WINDOW_WIDTH / 5, 150, 60, 10, HDirection::left)
                           .animate(0, WINDOW_WIDTH, 50);
  static auto cloud4 = Cloud(WINDOW_WIDTH / 5, 200, 80, 20, HDirection::left)
                           .animate(0, WINDOW_WIDTH, 60);
  static auto cloud5 = Cloud(WINDOW_WIDTH / 5, 250, 20, 10, HDirection::left)
                           .animate(0, WINDOW_WIDTH, 20);
  cloud1.draw(scene);
  cloud2.draw(scene);
  cloud3.draw(scene);
  cloud4.draw(scene);
  cloud5.draw(scene);
}

void draw() {
  drawSky(scene);
  drawStars(scene);
  drawSun();
  drawCloud(scene);
  drawRiver();
  drawFarHill();
  drawBridge();
  drawShips();
  drawMidRiverHill();
  drawNearHill();
}

void resize(int width, int height) {
  if (height == 0) {
    height = 1;
  }

  float aspect = (float)width / height;

  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, -1);

  glMatrixMode(GL_MODELVIEW);
}

void displayFunc() {
  glClearColor(0, 0, 0, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  draw();

  glutSwapBuffers();
}

bool keyStates[255] = {false};

void keyPressed(unsigned char key, int x, int y) { keyStates[key] = true; }
void keyReleased(unsigned char key, int x, int y) { keyStates[key] = false; }
void keyExecute() {
  if (keyStates['s'])
    scene = Scene::sunset;
  if (keyStates['n'])
    scene = Scene::night;
}

void idleFunc() {
  if (LIMIT_FPS) {
    static const double frame_delay = 1.0 / FPS;
    static double last_render = 0;
    const double current_time = GetCurrentTime();
    if ((current_time - last_render) < frame_delay) {
      last_render = current_time;
      return;
    }
  }
  keyExecute();
  glutPostRedisplay();
}

int main(int argc, char *argv[]) {
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

  glutCreateWindow("CG Final Project");

  glutDisplayFunc(displayFunc);
  glutIdleFunc(idleFunc);
  glutReshapeFunc(resize);

  glutKeyboardFunc(keyPressed);
  glutKeyboardUpFunc(keyReleased);

  glutMainLoop();

  return 0;
}
