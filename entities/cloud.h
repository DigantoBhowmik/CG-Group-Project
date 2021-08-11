#include "../lib.h"
#include "array"
#include "random"

class AnimatedCloud;
class Cloud {
private:
  void unitCloud() {
    for (auto cloud_chunk : cloud_chunks) {
      auto [x, y, r] = cloud_chunk;
      drawFilledCircle(x, y, r);
    }
  }

protected:
  double x, y, width, height;
  unsigned int color;
  HDirection facing_to = HDirection::left;
  vector<array<double, 3>> cloud_chunks;

  auto genCloudChunks() {
    vector<array<double, 3>> chunks;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> u_rand_x(0, 10), u_rand_y(0, 10),
        u_rand_r(5, 10), u_rand_chunk(4, 8);

    for (int n = 0; n < u_rand_chunk(gen); ++n) {
      double x = u_rand_x(gen) / 10.f, y = u_rand_y(gen) / 10.f,
             r = u_rand_r(gen) / 10.f;
      auto chunk = array<double, 3>{x, y, r};
      chunks.push_back(chunk);
    }
    return chunks;
  }

public:
  void draw(Scene scene) {
    switch (scene) {
    case Scene::sunset:
      color = 0xFEF3C7;
      break;
    case Scene::night:
      color = 0x5156D7;
      break;
    }
    auto cloud_color = Hex2glRGB(color);

    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(width, height, 0);

    glColor3fv((GLfloat *)&cloud_color);

    unitCloud();

    glPopMatrix();
  }

  Cloud(int x, int y, int width, int height, unsigned int color,
        HDirection facing_to)
      : x(x), y(y), width(width), height(height), facing_to(facing_to),
        color(color), cloud_chunks(genCloudChunks()) {}
  AnimatedCloud animate(int min_x, int max_x, int speed);
};

class AnimatedCloud : Cloud {
public:
  AnimatedCloud(Cloud ship, int min_x, int max_x, int speed)
      : Cloud(ship), min_x(min_x), max_x(max_x), speed(speed),
        last_render(GetCurrentTime()) {}
  void draw(Scene scene) {
    double current_time = GetCurrentTime();
    double epoch = current_time - last_render;
    double x_delta = epoch * speed;

    double cloud_size = width * 1.5;

    if (facing_to == HDirection::left) {
      x -= x_delta;

      if (x < min_x - cloud_size) {
        x = max_x + cloud_size;
        cloud_chunks = genCloudChunks();
      }
    } else {
      x += x_delta;

      if (x > max_x + cloud_size) {
        x = min_x - cloud_size;
        cloud_chunks = genCloudChunks();
      }
    }
    Cloud::draw(scene);

    last_render = current_time;
  }

private:
  double min_x, max_x;
  int speed; // pixel per second;
  double last_render;
};

inline AnimatedCloud Cloud::animate(int min_x, int max_x, int speed) {
  return AnimatedCloud(*this, min_x, max_x, speed);
}
