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

int main() {
  uint8_t *data = (uint8_t*) malloc(sizeof(uint8_t) * WIDTH * HEIGHT * COLORCHANNEL);
  
  int index = 0;
  vector<vec2> vertices = {
    vec2(300, 300), vec2(500, 400), vec2(500, 300),
    vec2(100, 100), vec2(200, 100), vec2(200, 200),
  };

  vector<vector<float>> lines;
  for (int i = 0; i<vertices.size(); i+=3) {
    lines.push_back({vertices[i].y - vertices[i+1].y, vertices[i+1].x - vertices[i].x, vertices[i+1].x * vertices[i].y - vertices[i].x * vertices[i+1].y});
    lines.push_back({vertices[i].y - vertices[i+2].y, vertices[i+2].x - vertices[i].x, vertices[i+2].x * vertices[i].y - vertices[i].x * vertices[i+2].y});
    lines.push_back({vertices[i+1].y - vertices[i+2].y, vertices[i+2].x - vertices[i+1].x, vertices[i+2].x * vertices[i+1].y - vertices[i+1].x * vertices[i+2].y});
  }
  bool b = false;
  for (int y = 0; y<HEIGHT; y++) {
    for (int x = 0; x<WIDTH; x++) {
      for (int i = 0; i<lines.size(); i+=3) {
        if ((lines[i][0]*x + lines[i][1]*y - lines[i][2]) <= 0 && (lines[i+1][0]*x + lines[i+1][1]*y - lines[i+1][2]) >= 0 && (lines[i+2][0]*x + lines[i+2][1]*y - lines[i+2][2]) <= 0) {
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
