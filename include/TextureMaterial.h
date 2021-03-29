#ifndef TEXTUREMATERIALH
#define TEXTUREMATERIALH

#include "Material.h"

class TextureMaterial : public Material {
private:
    unsigned char* color_image;
    int c_i_height, c_i_width, c_i_channels;
public:
    TextureMaterial();
    ~TextureMaterial();

    void setColorImage(int width, int height, int channels, unsigned char* image);

    glm::vec3 getColorAtIndex(int x, int y);
    glm::vec3 getBilinearColor(float u, float v); // Returns the color bilinealy interpolated color. IMP: 0 <= u,v <= 1, repeats otherwise
};

TextureMaterial::TextureMaterial() {}

TextureMaterial::~TextureMaterial() {}

void TextureMaterial::setColorImage(int width, int height, int channels, unsigned char* image) {
    c_i_width = width;
    c_i_height = height;
    c_i_channels = channels;

    color_image = image;
}

glm::vec3 TextureMaterial::getColorAtIndex(int x, int y) {
    if (x < c_i_width - 1 && y < c_i_height - 1) {
        int index = c_i_channels * (c_i_width * y + x);


        return glm::vec3(float(color_image[index + 0]) / 255.99f, float(color_image[index + 1]) / 255.99f, float(color_image[index + 2]) / 255.99f);
    }
    return glm::vec3(0.0, 0.0, 0.0);
}

glm::vec3 TextureMaterial::getBilinearColor(float u, float v) {
    float scaled_u = fabs(u * c_i_width);
    float scaled_v = fabs(v * c_i_height);

    float tx = scaled_u - floor(scaled_u);
    float ty = scaled_v - floor(scaled_v);

    int x_0 = int(floor(scaled_u + 0.5));
    int y_0 = int(floor(scaled_v + 0.5));

    glm::vec3 x = getColorAtIndex(x_0 % c_i_width, y_0 % c_i_height) * 255.0f;
    glm::vec3 y = getColorAtIndex((x_0 + 1) % c_i_width, y_0 % c_i_height) * 255.0f;
    glm::vec3 x2 = getColorAtIndex(x_0 % c_i_width, (y_0 + 1) % c_i_height) * 255.0f;
    glm::vec3 y2 = getColorAtIndex((x_0 + 1) % c_i_width, (y_0 + 1) % c_i_height) * 255.0f;

    return ty * (tx * x + (1.0f - tx) * y) + (1.0f - ty) * (tx * x2 + (1.0f - tx) * y2) ;
}
#endif