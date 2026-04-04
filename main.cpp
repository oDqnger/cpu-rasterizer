#include "stb_image_write.h"
#include <cmath>
#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <glm/common.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <vector>
#include <map>

#define WIDTH 800
#define HEIGHT 600
#define COLORCHANNEL 3

using namespace std;
using namespace glm;

inline float area_of_triangle(vec3 *p1, vec3 *p2, vec3 *p3) {
  return std::abs((p1->x*(p2->y - p3->y)+p2->x*(p3->y - p1->y) + p3->x*(p1->y - p2->y)) / 2);
}

void convert_ndc(vector<vec3> *vertices) {
  for (int i = 0; i<vertices->size(); i++) {
    if (vertices->at(i).x < 0) vertices->at(i).x = round(fabs(vertices->at(i).x) * WIDTH / 2);
    else vertices->at(i).x = round(vertices->at(i).x * WIDTH / 2 + WIDTH / 2);
    if (vertices->at(i).y < 0) vertices->at(i).y = round(fabs(vertices->at(i).y) * HEIGHT / 2);
    else vertices->at(i).y = round(vertices->at(i).y * HEIGHT / 2 + HEIGHT / 2);
 }
}

void projection_matrix(vector<vec3> *vertices) {
  for (int i = 0; i<vertices->size(); i++) {
    vertices->at(i).x = (vertices->at(i).x) / vertices->at(i).z;
    vertices->at(i).y = (vertices->at(i).y) / vertices->at(i).z;
  }
}

int main() {
  uint8_t *data = (uint8_t*) malloc(sizeof(uint8_t) * WIDTH * HEIGHT * COLORCHANNEL);
  
  int index = 0;

  // input data from user (requires it to be in ndc)
vector<vec3> vertices = {
    // Front face
    vec3(-0.5, -0.5,  2.5), vec3( 0.5, -0.5,  2.5), vec3( 0.5,  0.5,  2.5),
    vec3(-0.5, -0.5,  2.5), vec3( 0.5,  0.5,  2.5), vec3(-0.5,  0.5,  2.5),
    // Back face
    vec3( 0.5, -0.5,  1.5), vec3(-0.5, -0.5,  1.5), vec3(-0.5,  0.5,  1.5),
    vec3( 0.5, -0.5,  1.5), vec3(-0.5,  0.5,  1.5), vec3( 0.5,  0.5,  1.5),

    // Left face
    vec3(-0.5, -0.5,  1.5), vec3(-0.5, -0.5,  2.5), vec3(-0.5,  0.5,  2.5),
    vec3(-0.5, -0.5,  1.5), vec3(-0.5,  0.5,  2.5), vec3(-0.5,  0.5,  1.5),

    // Right face
    vec3( 0.5, -0.5,  2.5), vec3( 0.5, -0.5,  1.5), vec3( 0.5,  0.5,  1.5),
    vec3( 0.5, -0.5,  2.5), vec3( 0.5,  0.5,  1.5), vec3( 0.5,  0.5,  2.5),

    // Top face
    vec3(-0.5,  0.5,  2.5), vec3( 0.5,  0.5,  2.5), vec3( 0.5,  0.5,  1.5),
    vec3(-0.5,  0.5,  2.5), vec3( 0.5,  0.5,  1.5), vec3(-0.5,  0.5,  1.5),

    // Bottom face
    vec3(-0.5, -0.5,  1.5), vec3( 0.5, -0.5,  1.5), vec3( 0.5, -0.5,  2.5),
    vec3(-0.5, -0.5,  1.5), vec3( 0.5, -0.5,  2.5), vec3(-0.5, -0.5,  2.5),
};
  projection_matrix(&vertices);
  // for (int i = 0; i<vertices.size(); i++) {
  //   vertices[i].x *= sinf(glm::radians(30.0f));
  //   vertices[i].y *= cosf(glm::radians(-30.0f));
  // } 
  convert_ndc(&vertices);
  bool b = false;
  for (int y = 0; y<HEIGHT; y++) {
    for (int x = 0; x<WIDTH; x++) {
      vec3 p = glm::vec3(x,y, 0);
      for (int i = 0; i<vertices.size(); i+=3) {
        float at = area_of_triangle(&vertices[i], &vertices[i+1], &vertices[i+2]);
        float a1 = area_of_triangle(&p, &vertices[i], &vertices[i+1]);
        float a2 = area_of_triangle(&p, &vertices[i+1], &vertices[i+2]);
        float a3 = area_of_triangle(&p, &vertices[i], &vertices[i+2]);

        if ((a1+a2+a3) == at) {
          data[index++] = a1/at * 255;
          data[index++] = a2/at * 255;
          data[index++] = a3/at * 255;
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
