#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include <string>
#include <vector>

#include "../shader/vector_uniform.h"

// material attribute selection bits
const unsigned char MAT_ATTR_AMBIENT_COLOR       = (0x1 << 0x0);
const unsigned char MAT_ATTR_DIFFUSE_COLOR       = (0x1 << 0x1);
const unsigned char MAT_ATTR_OPTICAL_DENSITY     = (0x1 << 0x2);
const unsigned char MAT_ATTR_SPECULAR_COLOR      = (0x1 << 0x3);
const unsigned char MAT_ATTR_SPECULAR_EXPONENT   = (0x1 << 0x4);
const unsigned char MAT_ATTR_TRANSPARENCY        = (0x1 << 0x5);
const unsigned char MAT_ATTR_TRANSMISSION_FILTER = (0x1 << 0x6);
const unsigned char MAT_ATTR_ALL                 = 0b1111111;
// material property uniform names
static const char *MAT_UNIF_AMBIENT_COLOR       = "u_AmbientColor";
static const char *MAT_UNIF_DIFFUSE_COLOR       = "u_DiffuseColor";
static const char *MAT_UNIF_OPTICAL_DENSITY     = "u_OpticalDensity";
static const char *MAT_UNIF_SPECULAR_COLOR      = "u_SpecularColor";
static const char *MAT_UNIF_SPECULAR_EXPONENT   = "u_SpecularExponent";
static const char *MAT_UNIF_TRANSPARENCY        = "u_Transparency";
static const char *MAT_UNIF_TRANSMISSION_FILTER = "u_TransmissionFilter";

struct rgb_color_t { float r, g, b; };

struct Material {
  std::string name;

  // material attributes
  rgb_color_t ambientColor,
              diffuseColor,
              specularColor,
              transmissionFilterColor;
  float       opticalDensity,
              transparency,
              specularExponent;

  // texture maps
  std::string ambientTxMap,
              diffuseTxMap,
              specularTxMap;

  // returns a vector of uniforms for the attributes which are selected by the
  // attribute selection bits `MAT_ATTR_*`.
  std::vector<Uniform> compileUniforms(const unsigned char attrBits) {
    std::vector<Uniform> uniforms;
    if (attrBits & MAT_ATTR_AMBIENT_COLOR)
      uniforms.push_back(VectorUniform<float>{
        MAT_UNIF_AMBIENT_COLOR, 3,
        ambientColor.r, ambientColor.g, ambientColor.b
      });
    if (attrBits & MAT_ATTR_DIFFUSE_COLOR)
      uniforms.push_back(VectorUniform<float>{
        MAT_UNIF_DIFFUSE_COLOR, 3,
        diffuseColor.r, diffuseColor.g, diffuseColor.b
      });
    if (attrBits & MAT_ATTR_SPECULAR_COLOR)
      uniforms.push_back(VectorUniform<float>{
        MAT_UNIF_SPECULAR_COLOR, 3,
        specularColor.r, specularColor.g, specularColor.b
      });
    if (attrBits & MAT_ATTR_TRANSMISSION_FILTER)
      uniforms.push_back(VectorUniform<float>{
        MAT_UNIF_TRANSMISSION_FILTER, 3,
        transmissionFilterColor.r, transmissionFilterColor.g, transmissionFilterColor.b
      });
    if (attrBits & MAT_ATTR_OPTICAL_DENSITY)
      uniforms.push_back(VectorUniform<float>{
        MAT_UNIF_TRANSMISSION_FILTER, 1, opticalDensity
      });
    if (attrBits & MAT_ATTR_TRANSPARENCY)
      uniforms.push_back(VectorUniform<float>{
        MAT_UNIF_TRANSMISSION_FILTER, 1, transparency
      });
    if (attrBits & MAT_ATTR_SPECULAR_EXPONENT)
      uniforms.push_back(VectorUniform<float>{
        MAT_UNIF_TRANSMISSION_FILTER, 1, specularExponent
      });
    return uniforms;
  }
};

#endif
