#include "../lib.h"
#include "array"
#include "random"

class AnimatedBird;
class Bird {
protected:
  double x, y, width, height;
  unsigned int color;
  HDirection facing_to = HDirection::left;
  double wing_state = 0;
  GLColorRGB body_color, wing_color;

public:
  void draw(Scene scene) {
    switch (scene) {
    case Scene::sunset:
      color = 0xFEF3C7;
      break;
    case Scene::night:
      color = 0x5156D7;
      break;
    case Scene::day:
      color = 0xFCF3D5;
      break;
    }

    glPushMatrix();
    glTranslatef(x, y, 0);
    if (facing_to == HDirection::left) {
      glScalef(-width, height, 0);
    } else {
      glScalef(width, height, 0);
    }

    glColor3fv((GLfloat *)&body_color);

    glPushMatrix();
    glTranslatef(0, -.2, 0);
    glScalef(1, wing_state, 0);
    glBegin(GL_TRIANGLES);
    glColor3ub(93, 3, 4);
    glVertex2f(0.7f, 0.0f);
    glVertex2f(-0.2f, 0.7f);
    glVertex2f(0.1f, 0.0f);
    glEnd();
    glPopMatrix();

    glBegin(GL_POLYGON);
    glColor3ub(35, 140, 92);
    glVertex2f(-0.9f, 0.0f);
    glVertex2f(-0.7f, -0.4f);
    glVertex2f(0.3f, -0.5f);
    glVertex2f(0.9f, 0.0f);
    glEnd();

    glPushMatrix();
    glTranslatef(0, -.2, 0);
    glScalef(1, wing_state, 0);
    glBegin(GL_TRIANGLES);
    glColor3ub(212, 31, 38);
    glVertex2f(0.5f, 0.0f);
    glVertex2f(-0.4f, 0.7f);
    glVertex2f(-0.1f, 0.0f);
    glEnd();
    glPopMatrix();

    glPopMatrix();
  }

  Bird(int x, int y, int width, int height, HDirection facing_to)
      : x(x), y(y), width(width), height(height), facing_to(facing_to) {}
  AnimatedBird animate(int min_x, int max_x, int speed);
};

class AnimatedBird : Bird {
public:
  AnimatedBird(Bird bird, int min_x, int max_x, int speed)
      : Bird(bird), min_x(min_x), max_x(max_x), speed(speed),
        last_render(GetCurrentTime()), flap_direction(VDirection::top) {}
  void draw(Scene scene) {
    double current_time = GetCurrentTime();
    double epoch = current_time - last_render;
    double x_delta = epoch * speed;

    // Note: size should be pre-calculated... this is an approximination
    double cloud_size = width * 1.5;

    if (facing_to == HDirection::left) {
      x -= x_delta;

      if (x < min_x - cloud_size) {
        x = max_x + cloud_size;
      }
    } else {
      x += x_delta;

      if (x > max_x + cloud_size) {
        x = min_x - cloud_size;
      }
    }

    double flap_delta = fmod(epoch * 2, 2);
    if (flap_direction == VDirection::bottom) {
      wing_state -= flap_delta;
      if (wing_state < -1.2)
        flap_direction = VDirection::top;
    } else {
      wing_state += flap_delta;
      if (wing_state > 1.2)
        flap_direction = VDirection::bottom;
    }

    Bird::draw(scene);

    last_render = current_time;
  }

private:
  double min_x, max_x;
  int speed; // pixel per second;
  double last_render;
  VDirection flap_direction;
};

inline AnimatedBird Bird::animate(int min_x, int max_x, int speed) {
  return AnimatedBird(*this, min_x, max_x, speed);
}
