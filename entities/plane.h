#include "../lib.h"

class AnimatedPlane;
class Plane {
protected:
  double x, y, width, height;
  unsigned int color;
  HDirection facing_to = HDirection::left;

public:
  void draw(Scene scene) {
    switch (scene) {
    case Scene::sunset:
      color = 0x1E3936;
      break;
    case Scene::night:
      color = 0x1E3936;
      break;
    case Scene::day:
      color = 0x1E3936;
      break;
    }
    auto _color = toGLColorRGB(color);
    auto _wing_color = toGLColorRGB(0x120213);

    glPushMatrix();
    glTranslatef(x, y, 0);
    if (facing_to == HDirection::left) {
      glScalef(width, height, 0);
    } else {
      glScalef(-width, height, 0);
    }

    glColor3fv((GLfloat *)&_color);
    glBegin(GL_TRIANGLES); // body
    glVertex2f(-0.8f, 0.15f);
    glVertex2f(-0.4f, 0.45f);
    glVertex2f(0.8f, 0.0f);
    glEnd();

    glColor3fv((GLfloat *)&_wing_color);
    glBegin(GL_QUADS); // wing1
    glVertex2f(-0.15f, 0.355f);
    glVertex2f(0.15f, 0.75f);
    glVertex2f(0.25f, 0.77f);
    glVertex2f(0.1f, 0.26f);
    glEnd();
    glBegin(GL_QUADS); // wing2
    glVertex2f(-0.13f, 0.09f);
    glVertex2f(0.13f, -0.6f);
    glVertex2f(0.23f, -0.62f);
    glVertex2f(0.12f, 0.065f);
    glEnd();
    glBegin(GL_QUADS); // wing3
    glVertex2f(0.6f, 0.07f);
    glVertex2f(0.7f, 0.5f);
    glVertex2f(0.75f, 0.45f);
    glVertex2f(0.8f, 0.0f);
    glEnd();
    glBegin(GL_QUADS); // wing4
    glVertex2f(0.65f, 0.015f);
    glVertex2f(0.75f, -0.15f);
    glVertex2f(0.78f, -0.12f);
    glVertex2f(0.76f, 0.005f);
    glEnd();
    glColor3fv((GLfloat *)&_color);
    glBegin(GL_TRIANGLES); // body
    glVertex2f(-0.8f, 0.15f);
    glVertex2f(-0.4f, 0.45f);
    glVertex2f(0.8f, 0.0f);
    glEnd();

    glColor3fv((GLfloat *)&_wing_color);
    glBegin(GL_QUADS); // wing1
    glVertex2f(-0.15f, 0.355f);
    glVertex2f(0.15f, 0.75f);
    glVertex2f(0.25f, 0.77f);
    glVertex2f(0.1f, 0.26f);
    glEnd();
    glBegin(GL_QUADS); // wing2
    glVertex2f(-0.13f, 0.09f);
    glVertex2f(0.13f, -0.6f);
    glVertex2f(0.23f, -0.62f);
    glVertex2f(0.12f, 0.065f);
    glEnd();
    glBegin(GL_QUADS); // wing3
    glVertex2f(0.6f, 0.07f);
    glVertex2f(0.7f, 0.5f);
    glVertex2f(0.75f, 0.45f);
    glVertex2f(0.8f, 0.0f);
    glEnd();
    glBegin(GL_QUADS); // wing4
    glVertex2f(0.65f, 0.015f);
    glVertex2f(0.75f, -0.15f);
    glVertex2f(0.78f, -0.12f);
    glVertex2f(0.76f, 0.005f);
    glEnd();

    glPopMatrix();
  }

  Plane(int x, int y, int width, int height, HDirection facing_to)
      : x(x), y(y), width(width), height(height), facing_to(facing_to) {}
  AnimatedPlane animate(int min_x, int max_x, int speed);
};

class AnimatedPlane : Plane {
public:
  AnimatedPlane(Plane plane, int min_x, int max_x, int speed)
      : Plane(plane), min_x(min_x), max_x(max_x), speed(speed), start_x(x),
        last_render(GetCurrentTime()) {}
  void draw(Scene scene) {
    double current_time = GetCurrentTime();
    double epoch = current_time - last_render;
    double x_delta = epoch * speed;

    // Note: size should be pre-calculated... this is an approximination
    double plane_size = width * 2;

    if (facing_to == HDirection::left) {
      x -= x_delta;

      if (x < min_x - plane_size)
        facing_to = HDirection::right;
    } else {
      x += x_delta;

      if (x > max_x + plane_size)
        facing_to = HDirection::left;
    }
    Plane::draw(scene);

    last_render = current_time;
  }

private:
  double start_x, min_x, max_x;
  int speed; // pixel per second;
  double last_render;
};

inline AnimatedPlane Plane::animate(int min_x, int max_x, int speed) {
  return AnimatedPlane(*this, min_x, max_x, speed);
}
