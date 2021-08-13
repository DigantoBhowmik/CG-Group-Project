#include <exception>
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <stdlib.h>
#else
#include <GL/glut.h>
#endif

#include "./entities/bird.h"
#include "./entities/car.h"
#include "./entities/cloud.h"
#include "./entities/plane.h"
#include "./entities/ship.h"
#include "./lib.h"
#include <array>
#include <chrono>
#include <cmath>
#include <iostream>
#include <math.h>
#include <random>
#include <thread>
#include <tuple>
#include <vector>

using namespace std;

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#define STAR_COUNT 300

#define LIMIT_FPS true
int FPS = 30;

Scene scene = Scene::sunset;
bool rain = false;

void drawSun(GLColorRGB color) {
  auto [r, g, b] = color;
  glColor4f(r, g, b, .05);
  drawFilledCircle(1170, 740, 190);
  glColor4f(r, g, b, .1);
  drawFilledCircle(1170, 740, 160);
  glColor3f(r, g, b);
  drawFilledCircle(1170, 740, 135);
}
void drawSun(Scene scene) {
  switch (scene) {
  case Scene::sunset:
    drawSun(toGLColorRGB(243, 238, 191));
    break;
  case Scene::day:
    glPushMatrix();
    glTranslatef(100, -200, 0);
    glScalef(.5, .5, 0);
    drawSun(toGLColorRGB(0xFBE3B5));
    glPopMatrix();
    break;
  default:
    break;
  }
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
  case Scene::day:
    break;
  }
}
void drawBridgeCables(GLColorRGB color) {
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
  glColor3fv((GLfloat *)&color);
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
void drawBridgeTowers(GLColorRGB color, GLColorRGB shadow_color) {
  glColor3fv((GLfloat *)&shadow_color);
  drawQuad(407.8, 168, 13.5, 76.4);
  glColor3fv((GLfloat *)&color);
  drawQuad(407.8, 168, 9, 76.4);
  glColor3fv((GLfloat *)&shadow_color);
  drawQuad(449.9, 168, 13.5, 76.4);
  glColor3fv((GLfloat *)&color);
  drawQuad(449.9, 168, 9, 76.4);
  glColor3fv((GLfloat *)&color);
  drawQuad(407.8, 183.2, 55.6, 6.4);

  glColor3fv((GLfloat *)&shadow_color);
  drawQuad(396.5, 243.7, 17.4, 65.5);
  glColor3fv((GLfloat *)&color);
  drawQuad(396.5, 243.7, 11.6, 65.5);
  glColor3fv((GLfloat *)&shadow_color);
  drawQuad(450.8, 243.7, 17.4, 65.5);
  glColor3fv((GLfloat *)&color);
  drawQuad(450.8, 243.7, 11.6, 65.5);
  glColor3fv((GLfloat *)&color);
  drawQuad(396.5, 240.6, 71.7, 4.1);

  glColor3fv((GLfloat *)&shadow_color);
  drawQuad(386.1, 313.9, 22.4, 84.4);
  glColor3fv((GLfloat *)&color);
  drawQuad(386.1, 313.9, 15, 84.4);
  glColor3fv((GLfloat *)&shadow_color);
  drawQuad(456.1, 313.9, 22.4, 84.4);
  glColor3fv((GLfloat *)&color);
  drawQuad(456.1, 313.9, 15, 84.4);
  glColor3fv((GLfloat *)&color);
  drawQuad(386.1, 309, 92.5, 5.3);

  glColor3fv((GLfloat *)&shadow_color);
  drawQuad(372.7, 405, 28.9, 108.8);
  glColor3fv((GLfloat *)&color);
  drawQuad(372.7, 405, 19.3, 108.8);
  glColor3fv((GLfloat *)&shadow_color);
  drawQuad(463, 405, 28.9, 108.8);
  glColor3fv((GLfloat *)&color);
  drawQuad(463, 405, 19.3, 108.8);
  glColor3fv((GLfloat *)&color);
  drawQuad(372.7, 398.1, 119.2, 6.9);
}
void drawBridge(GLColorRGB bridge_color, GLColorRGB cables_color,
                GLColorRGB shadow_color) {
  drawBridgeCables(cables_color);
  // left
  drawBridgeTowers(bridge_color, shadow_color);
  // right
  glPushMatrix();
  glTranslated(1062, 0, 0);
  drawBridgeTowers(bridge_color, shadow_color);
  glPopMatrix();

  // BRIDGE START
  glBegin(GL_QUADS);
  glColor3fv((GLfloat *)&bridge_color);
  glVertex2f(0, 560);
  glVertex2f(1920, 560);
  glVertex2f(1920, 500);
  glVertex2f(0, 500);
  glEnd();
  glBegin(GL_QUADS); // UPPER
  glColor3fv((GLfloat *)&shadow_color);
  glVertex2f(0, 510);
  glVertex2f(1920, 510);
  glVertex2f(1920, 500);
  glVertex2f(0, 500);
  glEnd();
  // LEFT PILLER START
  glBegin(GL_QUADS);
  glColor3fv((GLfloat *)&bridge_color);
  glVertex2f(325, 560);
  glVertex2f(325, 585);
  glVertex2f(415, 585);
  glVertex2f(415, 560);
  glEnd();
  glBegin(GL_QUADS);
  glColor3fv((GLfloat *)&bridge_color);
  glVertex2f(350, 585);
  glVertex2f(350, 745);
  glVertex2f(390, 745);
  glVertex2f(390, 585);
  glEnd();
  glBegin(GL_QUADS); // UPPER
  glColor3fv((GLfloat *)&shadow_color);
  glVertex2f(378, 585);
  glVertex2f(378, 745);
  glVertex2f(390, 745);
  glVertex2f(390, 585);
  glEnd();

  glPushMatrix();
  glTranslated(120, 0, 0);
  glBegin(GL_QUADS);
  glColor3fv((GLfloat *)&bridge_color);
  glVertex2f(325, 560);
  glVertex2f(325, 585);
  glVertex2f(415, 585);
  glVertex2f(415, 560);
  glEnd();
  glBegin(GL_QUADS);
  glColor3fv((GLfloat *)&bridge_color);
  glVertex2f(350, 585);
  glVertex2f(350, 745);
  glVertex2f(390, 745);
  glVertex2f(390, 585);
  glEnd();
  glBegin(GL_QUADS); // UPPER
  glColor3fv((GLfloat *)&shadow_color);
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
  glColor3fv((GLfloat *)&bridge_color);
  glVertex2f(325, 560);
  glVertex2f(325, 585);
  glVertex2f(415, 585);
  glVertex2f(415, 560);
  glEnd();
  glBegin(GL_QUADS);
  glColor3fv((GLfloat *)&bridge_color);
  glVertex2f(350, 585);
  glVertex2f(350, 745);
  glVertex2f(390, 745);
  glVertex2f(390, 585);
  glEnd();
  glBegin(GL_QUADS); // UPPER
  glColor3fv((GLfloat *)&shadow_color);
  glVertex2f(378, 585);
  glVertex2f(378, 745);
  glVertex2f(390, 745);
  glVertex2f(390, 585);
  glEnd();
  glPushMatrix();
  glTranslated(120, 0, 0);
  glBegin(GL_QUADS);
  glColor3fv((GLfloat *)&bridge_color);
  glVertex2f(325, 560);
  glVertex2f(325, 585);
  glVertex2f(415, 585);
  glVertex2f(415, 560);
  glEnd();
  glBegin(GL_QUADS);
  glColor3fv((GLfloat *)&bridge_color);
  glVertex2f(350, 585);
  glVertex2f(350, 745);
  glVertex2f(390, 745);
  glVertex2f(390, 585);
  glEnd();
  glBegin(GL_QUADS); // UPPER
  glColor3fv((GLfloat *)&shadow_color);
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
void drawBridge(Scene scene) {
  switch (scene) {
  case Scene::sunset:
    drawBridge(toGLColorRGB(149, 36, 49), toGLColorRGB(0xbe1e2d),
               toGLColorRGB(182, 60, 74));
    break;
  case Scene::night:
    drawBridge(toGLColorRGB(0x050217), toGLColorRGB(0x1822A0),
               toGLColorRGB(0x24189E));
    break;
  case Scene::day:
    drawBridge(toGLColorRGB(0xFBB78B), toGLColorRGB(0xAC9C96),
               toGLColorRGB(0xD0937F));
  }
}

void drawFarHill(GLColorRGB color, GLColorRGB shadow_color) {
  // LEFT HILL START
  glBegin(GL_POLYGON);
  glColor3fv((GLfloat *)&color);
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
  glColor3fv((GLfloat *)&shadow_color);
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
  glColor3fv((GLfloat *)&shadow_color);
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
  glColor3fv((GLfloat *)&color);
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
  glColor3fv((GLfloat *)&shadow_color);
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
  glColor3fv((GLfloat *)&shadow_color);
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
void drawFarHill(Scene scene) {
  switch (scene) {
  case Scene::sunset:
    drawFarHill(toGLColorRGB(170, 43, 41), toGLColorRGB(196, 76, 88));
    break;
  case Scene::night:
    drawFarHill(toGLColorRGB(0x24189E), toGLColorRGB(0x0A0940));
    break;
  case Scene::day:
    drawFarHill(toGLColorRGB(0x928980), toGLColorRGB(0x433743));
    break;
  }
}
void drawNearHill(GLColorRGB color, GLColorRGB shadow_color) {
  // LEFT-BOTTOM HILL START
  glBegin(GL_POLYGON);
  glColor3fv((GLfloat *)&color);
  glVertex2f(435, 1080);
  glVertex2f(425, 1050);
  glVertex2f(325, 1007);
  glVertex2f(280, 943);
  glVertex2f(160, 925);
  glVertex2f(0, 860);
  glVertex2f(0, 1080);
  glEnd();

  glBegin(GL_POLYGON); // SHADOW
  glColor3fv((GLfloat *)&shadow_color);
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
  glColor3fv((GLfloat *)&color);
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
  glColor3fv((GLfloat *)&shadow_color);
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
  glColor3fv((GLfloat *)&shadow_color);
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
void drawNearHill(Scene scene) {
  switch (scene) {
  case Scene::sunset:
    drawNearHill(toGLColorRGB(148, 36, 40), toGLColorRGB(136, 30, 34));
    break;
  case Scene::night:
    drawNearHill(toGLColorRGB(0x24189E), toGLColorRGB(0x050217));
    break;
  case Scene::day:
    drawNearHill(toGLColorRGB(0xA87763), toGLColorRGB(0x462430));
    break;
  }
}

void drawMidRiverHill(GLColorRGB color, GLColorRGB shadow_color) {
  glPushMatrix();
  glTranslatef(0, 20, 0);
  glBegin(GL_POLYGON);
  glColor3fv((GLfloat *)&color);
  glVertex2f(503, 968);
  glVertex2f(475, 944);
  glVertex2f(463, 945);
  glVertex2f(447, 935);
  glVertex2f(423, 944);
  glVertex2f(392, 968);
  glEnd();

  glBegin(GL_POLYGON); // SHADOW
  glColor3fv((GLfloat *)&shadow_color);
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
  glColor3fv((GLfloat *)&color);
  glVertex2f(503, 968);
  glVertex2f(475, 944);
  glVertex2f(463, 945);
  glVertex2f(447, 935);
  glVertex2f(423, 944);
  glVertex2f(392, 968);
  glEnd();

  glBegin(GL_POLYGON); // SHADOW
  glColor3fv((GLfloat *)&shadow_color);
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
  glColor3fv((GLfloat *)&color);
  glVertex2f(503, 968);
  glVertex2f(475, 944);
  glVertex2f(463, 945);
  glVertex2f(447, 935);
  glVertex2f(423, 944);
  glVertex2f(392, 968);
  glEnd();

  glBegin(GL_POLYGON); // SHADOW
  glColor3fv((GLfloat *)&shadow_color);
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
  glColor3fv((GLfloat *)&color);
  glVertex2f(503, 968);
  glVertex2f(475, 944);
  glVertex2f(463, 945);
  glVertex2f(447, 935);
  glVertex2f(423, 944);
  glVertex2f(392, 968);
  glEnd();

  glBegin(GL_POLYGON); // SHADOW
  glColor3fv((GLfloat *)&shadow_color);
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
  glColor3fv((GLfloat *)&color);
  glVertex2f(503, 968);
  glVertex2f(475, 944);
  glVertex2f(463, 945);
  glVertex2f(447, 935);
  glVertex2f(423, 944);
  glVertex2f(392, 968);
  glEnd();

  glBegin(GL_POLYGON); // SHADOW
  glColor3fv((GLfloat *)&shadow_color);
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

void drawMidRiverHill(Scene scene) {
  switch (scene) {
  case Scene::sunset:
    drawMidRiverHill(toGLColorRGB(148, 36, 40), toGLColorRGB(136, 30, 34));
    break;
  case Scene::night:
    drawMidRiverHill(toGLColorRGB(0x24189E), toGLColorRGB(0x050217));
    break;
  case Scene::day:
    drawMidRiverHill(toGLColorRGB(0x8C7569), toGLColorRGB(0x4A3343));
    break;
  }
}

void drawSky(unsigned int color) {
  auto _color = toGLColorRGB(color);
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
  case Scene::day:
    drawSky(0x64C8C5);
    break;
  }
}
void drawRiver(GLColorRGB color) {
  glColor3fv((GLfloat *)&color);
  glBegin(GL_QUADS);
  glVertex2f(0, 745);
  glVertex2f(0, 1080);
  glVertex2f(1920, 1080);
  glVertex2f(1920, 745);
  glEnd();

  glColor4f(1, 1, 1, .1);
  drawFilledElipsis(400, 800, 250, 10);
  drawFilledElipsis(900, 900, 400, 10);
  drawFilledElipsis(1385, 800, 450, 6);
  glColor4f(1, 1, 1, .3);
  drawFilledElipsis(1300, 965, 100, 10);
}
void drawRiver(Scene scene) {
  switch (scene) {
  case Scene::sunset:
    drawRiver(toGLColorRGB(200, 63, 46));
    break;
  case Scene::night:
    drawRiver(toGLColorRGB(0x2C30A1));
    break;
  case Scene::day:
    drawRiver(toGLColorRGB(0x1C8A8B));
    break;
  }
}
void drawShips(Scene scene) {
  static auto ship1 =
      Ship(400, 800, 60, 50, HDirection::left).animate(0, WINDOW_WIDTH, 90);
  static auto ship2 =
      Ship(0, 730, 30, 25, HDirection::right).animate(0, WINDOW_WIDTH, 60);
  ship1.draw(scene);
  ship2.draw(scene);
}
void drawCloud(Scene scene) {
  static auto cloud1 = Cloud(WINDOW_WIDTH, 50, 80, 30, HDirection::left)
                           .animate(0, WINDOW_WIDTH, 20);
  static auto cloud2 = Cloud(WINDOW_WIDTH / 3, 80, 50, 20, HDirection::left)
                           .animate(0, WINDOW_WIDTH, 30);
  static auto cloud3 = Cloud(WINDOW_WIDTH / 5, 150, 60, 10, HDirection::left)
                           .animate(0, WINDOW_WIDTH, 40);
  static auto cloud4 = Cloud(WINDOW_WIDTH / 5, 200, 80, 20, HDirection::left)
                           .animate(0, WINDOW_WIDTH, 50);
  static auto cloud5 = Cloud(WINDOW_WIDTH / 5, 250, 20, 10, HDirection::left)
                           .animate(0, WINDOW_WIDTH, 10);
  static auto cloud6 =
      Cloud(100, 50, 80, 30, HDirection::left).animate(0, WINDOW_WIDTH, 20);
  static auto cloud7 =
      Cloud(100, 80, 50, 20, HDirection::left).animate(0, WINDOW_WIDTH, 30);
  static auto cloud8 =
      Cloud(-100, 150, 60, 10, HDirection::left).animate(0, WINDOW_WIDTH, 40);
  static auto cloud9 =
      Cloud(-100, 200, 80, 20, HDirection::left).animate(0, WINDOW_WIDTH, 50);
  static auto cloud10 =
      Cloud(-100, 250, 20, 10, HDirection::left).animate(0, WINDOW_WIDTH, 10);

  cloud1.draw(scene);
  cloud2.draw(scene);
  cloud3.draw(scene);
  cloud4.draw(scene);
  cloud5.draw(scene);
  cloud6.draw(scene);
  cloud7.draw(scene);
  cloud8.draw(scene);
  cloud9.draw(scene);
  cloud10.draw(scene);
}
void drawLightReflection(Scene scene) {
  switch (scene) {
  case Scene::sunset:
    glColor4f(1, 1, 1, .2);
    drawFilledElipsis(1157, 747, 150, 15);
    break;
  case Scene::night:
    break;
  case Scene::day:
    break;
  }
}

void drawMoon() {
  glPushMatrix();
  glTranslatef(580, 200, 0.f);
  glScalef(400, 400, 1.0f);

  glColor4f(1, 1, 1, 0.05);
  drawFilledCircle(0, 0, .15);
  glColor4f(1, 1, 1, 0.1);
  drawFilledCircle(0, 0, .12);
  glColor3f(1, 1, 1);
  drawFilledCircle(0, 0, .1);

  glPopMatrix();
}

void drawBirds(Scene scene) {
  static auto bird1 =
      Bird(400, 200, 20, 15, HDirection::left).animate(0, WINDOW_WIDTH, 60);
  static auto bird2 =
      Bird(0, 250, 20, 15, HDirection::right).animate(0, WINDOW_WIDTH, 50);
  bird1.draw(scene);
  bird2.draw(scene);
}
void drawPlane(Scene scene) {
  static auto plane =
      Plane(200, 50, 30, 25, HDirection::right).animate(0, WINDOW_WIDTH, 60);
  plane.draw(scene);
}
void drawCar(Scene scene) {
  static auto car =
      Car(200, 490, 30, 25, HDirection::right).animate(0, WINDOW_WIDTH, 180);
  car.draw(scene);
}
void drawRain() {}
void draw(Scene scene) {
  drawSky(scene);
  drawStars(scene);
  if (scene == Scene::sunset || scene == Scene::day)
    drawSun(scene);
  if (scene == Scene::night)
    drawMoon();
  drawCloud(scene);
  drawPlane(scene);
  drawBirds(scene);
  drawRiver(scene);
  drawLightReflection(scene);
  drawFarHill(scene);
  drawCar(scene);
  drawBridge(scene);
  drawShips(scene);
  drawMidRiverHill(scene);
  drawNearHill(scene);

  drawRain();
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

  draw(scene);

  glutSwapBuffers();
}

void playAudioFunc(Scene scene) {
  const char *day = "Sunny Day-SoundBible.com-2064222612(1).mp3";
  const char *sunset = "Light Rain And Crickets-SoundBible.com-1664737469.mp3";
  const char *night = "Nightime-SoundBible.com-952309297.mp3";

  static thread audio_thread(playAudio, sunset);

  // switch (scene) {
  // case Scene::sunset:
  //   thread(playAudio, sunset);
  //   break;
  // case Scene::night:
  //   thread(playAudio, night);
  //   break;
  // case Scene::day:
  //   thread(playAudio, day);
  //   break;
  // }
}

bool keyStates[255] = {false};

void keyPressed(unsigned char key, int x, int y) { keyStates[key] = true; }
void keyReleased(unsigned char key, int x, int y) { keyStates[key] = false; }
void keyExecute() {
  static double last_triggered = GetCurrentTime();
  double now = GetCurrentTime();

  if (now - last_triggered < .3)
    return;

  if (keyStates['s'])
    scene = Scene::sunset;
  if (keyStates['n'])
    scene = Scene::night;
  if (keyStates['d'])
    scene = Scene::day;
  if (keyStates['r']) {
    // scene = Scene::rain;
  }

  last_triggered = now;
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
  playAudioFunc(scene);
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
