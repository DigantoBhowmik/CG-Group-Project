#include <GL/glut.h>
#include <array>
#include <chrono>
#include <math.h>
#include <random>
#include <tuple>
#include <vector>

using namespace std;

#define PI 3.14159265358
#define PI2 6.28318530718
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#define STAR_COUNT 300

float ship_speed = 1;

union Color {
  unsigned int hex;
#if IS_BIG_ENDIAN
  struct {
    unsigned char r, g, b;
  };
#else
  struct {
    unsigned char b, g, r;
  };
#endif
};
struct GLColorRGB {
  GLfloat r, g, b;
};

GLColorRGB Hex2glRGB(unsigned int hex) {
  union Color _hex;
  _hex.hex = hex;
  GLColorRGB rgb;
  return {_hex.r / 255.f, _hex.g / 255.f, _hex.b / 255.f};
}

void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius) {
  int i;
  int triangleAmount = 255;

  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(x, y);
  for (i = 0; i <= triangleAmount; i++) {
    glVertex2f(x + (radius * cos(i * PI2 / triangleAmount)),
               y + (radius * sin(i * PI2 / triangleAmount)));
  }
  glEnd();
}

void drawLine(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) {
  glBegin(GL_LINES);
  glVertex2f(x1, y1);
  glVertex2f(x2, y2);
  glEnd();
}

void drawQuadUnit() {
  glBegin(GL_QUADS);
  float coords[] = {0, 0, 1, 0, 1, 1, 0, 1};
  glVertex2fv(coords);
  glVertex2fv(coords + 2);
  glVertex2fv(coords + 4);
  glVertex2fv(coords + 6);
  glEnd();
}
void drawQuad(GLfloat width, GLfloat height) {
  glPushMatrix();
  glScalef(width, height, 0);
  drawQuadUnit();
  glPopMatrix();
};
void drawQuad(GLfloat x, GLfloat y, GLfloat width, GLfloat height) {
  glPushMatrix();
  glTranslatef(x, y, 0.f);
  drawQuad(width, height);
  glPopMatrix();
}
void drawQuad(GLfloat size) { drawQuad(size, size); };
void drawQuad(GLfloat x, GLfloat y, GLfloat size) {
  drawQuad(x, y, size, size);
};
struct Vertex {
  GLfloat x;
  GLfloat y;
};
void drawVertices(vector<Vertex> vertices) {
  for (auto cord : vertices) {
    glVertex2fv((GLfloat *)&cord);
  }
}
vector<Vertex> getCubicBezierCurvePoints(Vertex controls[4]) {
  Vertex p0 = controls[0], p1 = controls[1], p2 = controls[2], p3 = controls[3];

  auto precision = .01;

  vector<Vertex> vertices;
  for (float t = 0.f; t <= 1.f; t += precision) {
    Vertex l0 = {(1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y};
    Vertex l1 = {(1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y};
    Vertex l2 = {(1 - t) * p2.x + t * p3.x, (1 - t) * p2.y + t * p3.y};

    Vertex q0 = {(1 - t) * l0.x + t * l1.x, (1 - t) * l0.y + t * l1.y};
    Vertex q1 = {(1 - t) * l1.x + t * l2.x, (1 - t) * l1.y + t * l2.y};

    Vertex c0 = {(1 - t) * q0.x + t * q1.x, (1 - t) * q0.y + t * q1.y};

    vertices.push_back(c0);
  }
  return vertices;
}
vector<Vertex> getQuadBezierCurvePoints(Vertex controls[3]) {
  Vertex p0 = controls[0], p1 = controls[1], p2 = controls[2];

  auto precision = .01;

  vector<Vertex> vertices;
  for (float t = 0.f; t <= 1.f; t += precision) {
    Vertex l0 = {(1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y};
    Vertex l1 = {(1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y};

    Vertex q0 = {(1 - t) * l0.x + t * l1.x, (1 - t) * l0.y + t * l1.y};

    vertices.push_back(q0);
  }
  return vertices;
}
void drawSun() {
  glColor4ub(243, 238, 191, 255 * .05);
  drawFilledCircle(1170, 740, 190);
  glColor4ub(243, 238, 191, 255 * .1);
  drawFilledCircle(1170, 740, 160);
  glColor3ub(243, 238, 191);
  drawFilledCircle(1170, 740, 135);
}
void glDrawStar() {
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
void drawStars(GLfloat widht, GLfloat height) {
  static bool ran_once = false;
  static STAR_META stars_meta[STAR_COUNT];

  if (!ran_once) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> u_rand_x(0, widht);
    uniform_int_distribution<> u_rand_y(0, height);
    uniform_int_distribution<> u_rand_r(15, 30);

    for (int n = 0; n < STAR_COUNT; ++n) {
      int x = u_rand_x(gen), y = u_rand_y(gen), r = u_rand_r(gen);
      STAR_META star = {x, y, r};
      stars_meta[n] = star;
    }
    ran_once = true;
  }

  for (auto star : stars_meta) {
    glPushMatrix();
    glTranslatef(star.x, star.y, 0.f);
    glScalef(star.r, star.r, 1.0f);
    glDrawStar();
    glPopMatrix();
  }
  return;
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

void drawShip() {
  int size = 200;
  static int x = WINDOW_WIDTH - size;
  auto ship_color = Hex2glRGB(0x75181D);
  glColor3fv((GLfloat *)&ship_color);

  glPushMatrix();
  glTranslatef(x, 720, 0);
  glScalef(60, 50, 0);
  glBegin(GL_TRIANGLES);
  glVertex2f(0, 1);
  glVertex2f(.40, 0);
  glVertex2f(.40, 1);
  glEnd();
  glBegin(GL_QUADS);
  float pal[] = {.45, 0, .45, 1, .50, 1, .50, .2};
  glVertex2fv(pal);
  glVertex2fv(pal + 2);
  glVertex2fv(pal + 4);
  glVertex2fv(pal + 6);
  glEnd();
  glBegin(GL_TRIANGLES);
  glVertex2f(.55, .2);
  glVertex2f(.55, 1);
  glVertex2f(1, 1);
  glEnd();
  glBegin(GL_QUADS);
  float base[] = {-.2, 1.1, 0, 1.4, 1.1, 1.4, 1.2, 1.1};
  glVertex2fv(base);
  glVertex2fv(base + 2);
  glVertex2fv(base + 4);
  glVertex2fv(base + 6);
  glEnd();
  glPopMatrix();

  x -= ship_speed;
  if (x < 0)
    x = WINDOW_WIDTH - 200;
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
void drawSunset() {
  // SKY START
  glBegin(GL_QUADS);
  glColor3ub(220, 134, 48);
  glVertex2f(0, 0);
  glVertex2f(0, 1080);
  glVertex2f(1920, 1080);
  glVertex2f(1920, 0);
  glEnd();
  // SKY END

  drawStars(WINDOW_WIDTH, 650);
  drawSun();

  // RIVER START
  glBegin(GL_QUADS);
  glColor3ub(200, 63, 46);
  glVertex2f(0, 745);
  glVertex2f(0, 1080);
  glVertex2f(1920, 1080);
  glVertex2f(1920, 745);
  glEnd();
  // RIVER END

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
  /////////////////////
  glPushMatrix();
  glTranslatef(-106, -67, 0);
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
  glTranslatef(679, 0, 0);
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
  glTranslatef(794, -5, 0);
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
  glTranslatef(794, -44, 0);
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

  drawBridge();
  drawShip();
}

void draw() { drawSunset(); }

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

void render(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glClearColor(0, 0, 0, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  draw();

  glutSwapBuffers();
}

int main(int argc, char *argv[]) {
  glutInit(&argc, argv);

  // glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

  glutCreateWindow("CG Final Project");

  glutDisplayFunc(render);
  glutReshapeFunc(resize);
  glutIdleFunc(render);

  glutMainLoop();

  return 0;
}
