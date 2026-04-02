#include "stb_image_write.h"
#include <cmath>
#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <glm/common.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include <map>

#define WIDTH 800
#define HEIGHT 600
#define COLORCHANNEL 3

using namespace std;
using namespace glm;

bool draw_line(float x, float y, float gradient, float intercept) {
  if ((x*gradient + intercept) == y) {
    return true;
  }
  return false;
}

int main() {
  uint8_t *data = (uint8_t*) malloc(sizeof(uint8_t) * WIDTH * HEIGHT * COLORCHANNEL);
  
  int index = 0;
  vector<vec2> vertices = {
    // vec2(300, 300), vec2(500, 400),
    // vec2(300, 300), vec2(500, 300),
    vec2(500, 300), vec2(500, 400)
  };
  
  map<float, float> lines;
  for (int x = 0; x<vertices.size(); x+=2) {
    if ((vertices[x+1].x - vertices[x].x) != 0) {
      float gradient = (vertices[x+1].y - vertices[x].y) / (vertices[x+1].x - vertices[x].x);
      float intercept = vertices[x+1].y - (gradient * vertices[x+1].x);
      lines[gradient] = intercept;
    } else {
      lines[0] = vertices[x].x;
    }
  }
  bool done = false;
  for (int y = 0; y<HEIGHT; y++) {
    for (int x = 0; x<WIDTH; x++) {
      int i = 0;
      for (auto const& [key, value]: lines) {
        if (draw_line(x, y, key, value) && x>=std::min(vertices[i].x, vertices[i+1].x) && x<=std::max(vertices[i].x, vertices[i+1].x)) {
          cout << x << endl;
          data[index++] = 255;
          data[index++] = 255;
          data[index++] = 255;
          done = true;
        }
        i+=2;
      }
      if (!done) {
        data[index++] = 0;
        data[index++] = 0;
        data[index++] = 0;
      }
      done = false;
    }
  }

  stbi_flip_vertically_on_write(true);
  bool success = stbi_write_png("./image.png", WIDTH, HEIGHT, COLORCHANNEL, data, COLORCHANNEL * WIDTH);
  if (success) cout << "wrote to file";
  else cout << "unable to write to file";
  return 0;
}
