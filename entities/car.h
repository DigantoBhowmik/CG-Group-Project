#include "../lib.h"

class AnimatedCar;
class Car {
protected:
  double x, y, width, height;
  HDirection facing_to = HDirection::left;

public:
  void draw(Scene scene) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    if (facing_to == HDirection::left) {
      glScalef(-width, -height, 0);
    } else {
      glScalef(width, -height, 0);
    }

    glBegin(GL_POLYGON);
    glColor3f(0.0f, 0.0f, 1.0f); /// CAR Body///
    glVertex2f(0.9f, -0.45f);
    glVertex2f(-0.9f, -0.45f);
    glVertex2f(-.9f, 0.0f);
    glVertex2f(.6f, -0.0f);
    glVertex2f(.9f, -0.25f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 1.0f); /// Window///
    glVertex2f(-0.8f, 0.0f);
    glVertex2f(-.6f, 0.4f);
    glVertex2f(-.15f, 0.4f);
    glVertex2f(-.15f, .0f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 1.0f); /// Window///
    glVertex2f(-.15f, 0.4f);
    glVertex2f(-.15f, .0f);
    glVertex2f(.55f, 0.0f);
    glVertex2f(.3f, .4f);
    glEnd();

    glLineWidth(10);
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(-0.8f, 0.0f);
    glVertex2f(.55f, 0.0f);

    glVertex2f(-0.8f, 0.0f);
    glVertex2f(-.6f, 0.4f);

    glVertex2f(-.6f, 0.4f);
    glVertex2f(.3f, .4f);

    glVertex2f(.3f, .4f);
    glVertex2f(.55f, 0.0f);
    glEnd();

    glLineWidth(20);
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(-0.15f, 0.4f);
    glVertex2f(-.15f, 0.0f);
    glEnd();

    int i;
    GLfloat x = -0.6f;
    GLfloat y = -0.45f;
    GLfloat radius = .1f;
    int triangleAmount = 20; //# of triangles used to draw circle
    GLfloat twicePi = 2.0f * PI;
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(x, y);
    for (i = 0; i <= triangleAmount; i++) {
      glVertex2f(x + (radius * cos(i * twicePi / triangleAmount)),
                 y + (radius * sin(i * twicePi / triangleAmount)));
    }

    glEnd();

    int j;
    GLfloat x1 = 0.6f;
    GLfloat y1 = -0.45f;
    GLfloat radius1 = .1f;
    int triangleAmount1 = 20; //# of triangles used to draw circle
    GLfloat twicePi1 = 2.0f * PI;
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(x1, y1);
    for (j = 0; j <= triangleAmount1; j++) {
      glVertex2f(x1 + (radius1 * cos(j * twicePi1 / triangleAmount1)),
                 y1 + (radius1 * sin(j * twicePi1 / triangleAmount1)));
    }
    glEnd();

    glPopMatrix();
  }

  Car(int x, int y, int width, int height, HDirection facing_to)
      : x(x), y(y), width(width), height(height), facing_to(facing_to) {}
  AnimatedCar animate(int min_x, int max_x, int speed);
};

class AnimatedCar : Car {
public:
  AnimatedCar(Car car, int min_x, int max_x, int speed)
      : Car(car), min_x(min_x), max_x(max_x), speed(speed), start_x(x),
        last_render(GetCurrentTime()) {}
  void draw(Scene scene) {
    double current_time = GetCurrentTime();
    double epoch = current_time - last_render;
    double x_delta = epoch * speed;

    // Note: size should be pre-calculated... this is an approximination
    double car_size = width * 2;

    if (facing_to == HDirection::left) {
      x -= x_delta;

      if (x < min_x - car_size)
        facing_to = HDirection::right;
    } else {
      x += x_delta;

      if (x > max_x + car_size)
        facing_to = HDirection::left;
    }
    Car::draw(scene);

    last_render = current_time;
  }

private:
  double start_x, min_x, max_x;
  int speed; // pixel per second;
  double last_render;
};

inline AnimatedCar Car::animate(int min_x, int max_x, int speed) {
  return AnimatedCar(*this, min_x, max_x, speed);
}
