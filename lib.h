#ifndef LIB_H
#define LIB_H

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <stdlib.h>
#else
#include <GL/glut.h>
#endif

#include <chrono>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

using namespace std;

#define PI 3.14159265358
#define PI2 6.28318530718

enum Scene { sunset, night, day };

enum HDirection { left, right };
enum VDirection { top, bottom };

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

inline GLColorRGB toGLColorRGB(GLubyte r, GLubyte g, GLubyte b) {
  return {r / 255.f, g / 255.f, b / 255.f};
}
inline GLColorRGB toGLColorRGB(unsigned int hex) {
  union Color _hex;
  _hex.hex = hex;
  GLColorRGB rgb;
  return toGLColorRGB(_hex.r, _hex.g, _hex.b);
}

inline void drawFilledElipsis(GLfloat x, GLfloat y, GLfloat width,
                              GLfloat height) {
  int i;
  int triangleAmount = 255;

  glPushMatrix();
  glTranslatef(x, y, 0);
  glPushMatrix();
  glScalef(width, height, 0);
  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(0, 0);
  for (i = 0; i <= triangleAmount; i++) {
    glVertex2f((1 * cos(i * PI2 / triangleAmount)),
               (1 * sin(i * PI2 / triangleAmount)));
  }
  glEnd();
  glPopMatrix();
  glPopMatrix();
}
inline void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius) {
  drawFilledElipsis(x, y, radius, radius);
}

inline void drawLine(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) {
  glBegin(GL_LINES);
  glVertex2f(x1, y1);
  glVertex2f(x2, y2);
  glEnd();
}

inline void drawQuadUnit() {
  glBegin(GL_QUADS);
  float coords[] = {0, 0, 1, 0, 1, 1, 0, 1};
  glVertex2fv(coords);
  glVertex2fv(coords + 2);
  glVertex2fv(coords + 4);
  glVertex2fv(coords + 6);
  glEnd();
}
inline void drawQuad(GLfloat width, GLfloat height) {
  glPushMatrix();
  glScalef(width, height, 0);
  drawQuadUnit();
  glPopMatrix();
};
inline void drawQuad(GLfloat x, GLfloat y, GLfloat width, GLfloat height) {
  glPushMatrix();
  glTranslatef(x, y, 0.f);
  drawQuad(width, height);
  glPopMatrix();
}
inline void drawQuad(GLfloat size) { drawQuad(size, size); };
inline void drawQuad(GLfloat x, GLfloat y, GLfloat size) {
  drawQuad(x, y, size, size);
};
struct Vertex {
  GLfloat x;
  GLfloat y;
};
inline void drawVertices(vector<Vertex> vertices) {
  for (auto cord : vertices) {
    glVertex2fv((GLfloat *)&cord);
  }
}
inline vector<Vertex> getCubicBezierCurvePoints(Vertex controls[4]) {
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
inline vector<Vertex> getQuadBezierCurvePoints(Vertex controls[3]) {
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

inline double GetCurrentTime() {
  using Duration = chrono::duration<double>;
  return chrono::duration_cast<Duration>(
             chrono::high_resolution_clock::now().time_since_epoch())
      .count();
}

#endif
