/**
 * @file      
 * @brief     Common Shader3D sources.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#include <ATD/Core/Printf.hpp>
#include <ATD/Graphics/Shader.hpp>


/* Basic Shader3D for drawing textured model without normals: */

const std::string ATD::Shader3D::PLAIN_VERTEX_SOURCE = 
"#version 130\n"
"\n"
"attribute vec3 atrPosition;\n"
"attribute vec2 atrTexCoord;\n"
"attribute vec4 atrColor;\n"
"\n"
"varying vec2 varTexCoord;\n"
"varying vec4 varColor;\n"
"\n"
"uniform mat4 unfTransform;\n"
"uniform mat4 unfProject;\n"
"\n"
"\n"
"void main()\n"
"{\n"
"    vec4 position = unfTransform * vec4(atrPosition, 1.f);\n"
"\n"
"    varTexCoord = atrTexCoord;\n"
"    varColor = atrColor;\n"
"    gl_Position = unfProject * position;\n"
"}\n";

const std::string ATD::Shader3D::PLAIN_FRAGMENT_SOURCE = 
"#version 130\n"
"\n"
"varying vec2 varTexCoord;\n"
"varying vec4 varColor;\n"
"\n"
"uniform sampler2D unfTexture0;\n"
/* "uniform sampler2D unfTextureCanvas;\n" */
/* "uniform sampler2D unfTexture1;\n" */
"\n"
"\n"
"void main()\n"
"{\n"
"    vec4 txColor = texture2D(unfTexture0, varTexCoord);\n"
"    gl_FragColor = txColor * varColor;\n"
"}\n";


/* Shader for drawing illuminated model with normals: */

const std::string ATD::Shader3D::LIGHT_VERTEX_SOURCE = 
"#version 130\n"
"\n"
"attribute vec3 atrPosition;\n"
"attribute vec2 atrTexCoord;\n"
"attribute vec3 atrNormal;\n"
"attribute vec4 atrColor;\n"
"\n"
"varying vec3 varPosition;\n"
"varying vec2 varTexCoord;\n"
"varying vec3 varNormal;\n"
"varying vec4 varColor;\n"
"\n"
"uniform mat4 unfTransform;\n"
"uniform mat4 unfProject;\n"
"\n"
"\n"
"void main()\n"
"{\n"
"    mat3 vec3Transform;\n"
"    vec3Transform[0] = unfTransform[0].xyz;\n"
"    vec3Transform[1] = unfTransform[1].xyz;\n"
"    vec3Transform[2] = unfTransform[2].xyz;\n"
"\n"
"    vec4 position = unfTransform * vec4(atrPosition, 1.f);\n"
"\n"
"    varPosition = position.xyz;\n"
"    varTexCoord = atrTexCoord;\n"
"    varNormal = normalize(vec3Transform * atrNormal);\n"
"    varColor = atrColor;\n"
"    gl_Position = unfProject * position;\n"
"}\n";

const std::string ATD::Shader3D::LIGHT_FRAGMENT_SOURCE_TEMPLATE = 
"#version 130\n"
"\n"
"varying vec3 varPosition;\n"
"varying vec2 varTexCoord;\n"
"varying vec3 varNormal;\n"
"varying vec4 varColor;\n"
"\n"
"uniform sampler2D unfTexture0;\n"
/* "uniform sampler2D unfTextureCanvas;\n" */
/* "uniform sampler2D unfTexture1;\n" */
"\n"
"uniform vec3 unfCameraPos;\n"
"\n"
"\n"
/* Lighting types: */
"\n"
/* Parameters, common for all lights. */
"struct Light\n"
"{\n"
"    vec3 color;\n"
"    float ambient;\n"
"    float diffuse;\n"
"    float specular;\n"
"};\n"
"\n"
/* Directional light. */
"struct DirLight\n"
"{\n"
"    Light lt;\n"
"    vec3 direction;\n"
"};\n"
"\n"
/* Point light. */
"struct PointLight\n"
"{\n"
"    Light lt;\n"
"    vec3 position;\n"
"    vec3 attenuation;\n"
"};\n"
"\n"
/* Spot light. */
"struct SpotLight\n"
"{\n"
"    PointLight plt;\n"
"    vec3 direction;\n"
"    float cutoff;\n"
"};\n"
"\n"
/* Lighting parameters: */
"\n"
"const int MAX_DIR_LIGHTS = %u;\n" /* Template arg #0. */
"uniform int unfDirLightsNum;\n"
"uniform DirLight unfDirLights[MAX_DIR_LIGHTS];\n"
"\n"
"const int MAX_POINT_LIGHTS = %u;\n" /* Template arg #1. */
"uniform int unfPointLightsNum;\n"
"uniform PointLight unfPointLights[MAX_POINT_LIGHTS];\n"
"\n"
"const int MAX_SPOT_LIGHTS = %u;\n" /* Template arg #2. */
"uniform int unfSpotLightsNum;\n"
"uniform SpotLight unfSpotLights[MAX_SPOT_LIGHTS];\n"
"\n"
"\n"
/* Material parameters: */
"\n"
"uniform float unfSpecularPower;\n"
"\n"
"\n"
"vec3 ApplyLight(vec3 fgPosition, vec3 fgNormal, \n"
"    Light lt, vec3 ltDirection, float ltIntensity)\n"
"{\n"
"    vec3 ltDirection1 = normalize(ltDirection);\n"
"    vec3 fgNormal1 = normalize(fgNormal);\n"
"    vec3 cmDirection1 = normalize(unfCameraPos - fgPosition);\n"
"\n"
"    float ftDiffuse = dot(ltDirection1, fgNormal1 * (-1.f));\n"
"    float ftSpecular = pow(\n"
"        max(\n"
"            dot(\n"
"                cmDirection1, \n"
"                normalize(reflect(ltDirection1, fgNormal1))\n"
"                ), \n"
"            0.f), \n"
"        unfSpecularPower);\n"
"\n"
"    vec3 ltAmbient = lt.color * lt.ambient * ltIntensity;\n"
"    vec3 ltDiffuse = (ftDiffuse > 0.f) ? \n"
"        lt.color * lt.diffuse * ltIntensity * ftDiffuse : \n"
"        vec3(0.f);\n"
"    vec3 ltSpecular = (ftDiffuse > 0.f && ftSpecular > 0.f) ? \n"
"        lt.color * lt.specular * ltIntensity * ftSpecular * \n"
"            smoothstep(0.0f, 0.2f, ftDiffuse): \n"
"        vec3(0.f);\n"
"\n"
"    vec3 light = ltAmbient + ltDiffuse + ltSpecular;\n"
"    return light;\n"
"}\n"
"\n"
"vec3 ApplyDirLight(vec3 fgPosition, vec3 fgNormal, DirLight dlt)\n"
"{\n"
"    return ApplyLight(fgPosition, fgNormal, dlt.lt, dlt.direction, \n"
"        1.f);\n"
"}\n"
"\n"
"vec3 ApplyPointLight(vec3 fgPosition, vec3 fgNormal, PointLight plt)\n"
"{\n"
"    float distance = length(fgPosition - plt.position);\n"
"\n"
"    vec3 ltDirection = normalize(fgPosition - plt.position);\n"
"    float ltIntensity = 1.f / (plt.attenuation.x + \n"
"        plt.attenuation.y * distance + \n"
"        plt.attenuation.z * distance * distance);\n"
"\n"
"    return ApplyLight(fgPosition, fgNormal, plt.lt, ltDirection, \n"
"        ltIntensity);\n"
"}\n"
"\n"
"vec3 ApplySpotLight(vec3 fgPosition, vec3 fgNormal, SpotLight slt)\n"
"{\n"
"    float distance = length(fgPosition - slt.plt.position);\n"
"    vec3 ltDirection = normalize(fgPosition - slt.plt.position);\n"
"    float ftSpot = dot(ltDirection, slt.direction);\n"
"\n"
"    if (ftSpot > slt.cutoff) {\n"
"        float ltIntensity = (1.f / (slt.plt.attenuation.x + \n"
"                    slt.plt.attenuation.y * distance + \n"
"                    slt.plt.attenuation.z * distance * distance)) * \n"
"            (1.f - (1.f - ftSpot) / (1.f - slt.cutoff));\n"
"\n"
"        return ApplyLight(fgPosition, fgNormal, slt.plt.lt, ltDirection, \n"
"            ltIntensity);\n"
"    } else {\n"
"        return vec3(0.f);\n"
"    }\n"
"}\n"
"\n"
"\n"
"void main()\n"
"{\n"
/*   Light. */
"    vec3 light = vec3(0.f);\n"
"    for(int dltIndex = 0; dltIndex < unfDirLightsNum; dltIndex++) {\n"
"        light += ApplyDirLight(varPosition, varNormal, \n"
"            unfDirLights[dltIndex]);\n"
"    }\n"
"    for(int pltIndex = 0; pltIndex < unfPointLightsNum; pltIndex++) {\n"
"        light += ApplyPointLight(varPosition, varNormal, \n"
"            unfPointLights[pltIndex]);\n"
"    }\n"
"    for(int sltIndex = 0; sltIndex < unfSpotLightsNum; sltIndex++) {\n"
"        light += ApplySpotLight(varPosition, varNormal, \n"
"            unfSpotLights[sltIndex]);\n"
"    }\n"
"\n"
/*   Texel color. */
"    vec4 txColor = texture2D(unfTexture0, varTexCoord);\n"
"\n"
"\n"
"    gl_FragColor = txColor * varColor * vec4(light, 1.f);\n"
"}\n";

const unsigned ATD::Shader3D::LIGHT_DFT_MAX_DIR_LIGHTS   = 1;
const unsigned ATD::Shader3D::LIGHT_DFT_MAX_POINT_LIGHTS = 3;
const unsigned ATD::Shader3D::LIGHT_DFT_MAX_SPOT_LIGHTS  = 1;

const std::string ATD::Shader3D::LIGHT_FRAGMENT_SOURCE = 
	ATD::Printf(ATD::Shader3D::LIGHT_FRAGMENT_SOURCE_TEMPLATE.c_str(), 
			ATD::Shader3D::LIGHT_DFT_MAX_DIR_LIGHTS, 
			ATD::Shader3D::LIGHT_DFT_MAX_POINT_LIGHTS, 
			ATD::Shader3D::LIGHT_DFT_MAX_SPOT_LIGHTS);


