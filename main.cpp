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

inline float area_of_triangle(vec2 *p1, vec2 *p2, vec2 *p3) {
  return std::abs((p1->x*(p2->y - p3->y)+p2->x*(p3->y - p1->y) + p3->x*(p1->y - p2->y)) / 2);
}

int main() {
  uint8_t *data = (uint8_t*) malloc(sizeof(uint8_t) * WIDTH * HEIGHT * COLORCHANNEL);
  
  int index = 0;
  vector<vec2> vertices = {
    vec2(300, 300), vec2(500, 400), vec2(500, 300),
    vec2(100, 100), vec2(200, 100), vec2(200, 200),
  };

  bool b = false;
  for (int y = 0; y<HEIGHT; y++) {
    for (int x = 0; x<WIDTH; x++) {
      for (int i = 0; i<vertices.size(); i+=3) {
        vec2 p = glm::vec2(x,y);
        float at = area_of_triangle(&vertices[i], &vertices[i+1], &vertices[i+2]);
        float a1 = area_of_triangle(&p, &vertices[i], &vertices[i+1]);
        float a2 = area_of_triangle(&p, &vertices[i+1], &vertices[i+2]);
        float a3 = area_of_triangle(&p, &vertices[i], &vertices[i+2]);

        if ((a1+a2+a3) == at) {
          data[index++] = 255;
          data[index++] = 255;
          data[index++] = 255;
          b = true;
          break;
        }
      }
      if (!b) {
        data[index++] = 0;
        data[index++] = 0;
        data[index++] = 0;
      }
      b = false;
    }
  }

  stbi_flip_vertically_on_write(true);
  bool success = stbi_write_png("./image.png", WIDTH, HEIGHT, COLORCHANNEL, data, COLORCHANNEL * WIDTH);
  if (success) cout << "wrote to file";
  else cout << "unable to write to file";
  return 0;
}
