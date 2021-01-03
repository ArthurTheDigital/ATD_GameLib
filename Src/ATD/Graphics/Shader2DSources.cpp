/**
 * @file      
 * @brief     Common Shader2D sources.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#include <ATD/Graphics/Shader.hpp>


/* Basic 2D shader for drawing texture2D: */

const std::string ATD::Shader2D::PLAIN_VERTEX_SOURCE = 
"#version 130\n"
"\n"
"attribute vec2 atrPosition;\n"
"attribute vec2 atrTexCoord;\n"
"attribute vec4 atrColor;\n"
"\n"
"varying vec2 varTexCoord;\n"
"varying vec4 varColor;\n"
"\n"
"uniform mat3 unfProject;\n"
"uniform mat3 unfTransform;\n"
"\n"
"\n"
"void main()\n"
"{\n"
"    vec3 position = unfTransform * vec3(atrPosition, 1.f);\n"
"    vec3 projection = unfProject * position;\n"
"\n"
"    varTexCoord = atrTexCoord;\n"
"    varColor = atrColor;\n"
"    gl_Position = vec4(projection.xy, 0.f, 1.f);\n"
"}\n";

const std::string ATD::Shader2D::PLAIN_FRAGMENT_SOURCE = 
"#version 130\n"
"\n"
"varying vec2 varTexCoord;\n"
"varying vec4 varColor;\n"
"\n"
"uniform sampler2D unfTexture0;\n"
"\n"
"\n"
"void main()\n"
"{\n"
"    vec4 txColor = texture2D(unfTexture0, varTexCoord);\n"
"    vec4 rsColor = txColor * varColor;\n"
"    gl_FragColor = rsColor;\n"
"}\n";


/* Simple 2D shader for drawing textures with transparency. */

const std::string ATD::Shader2D::ALPHA_VERTEX_SOURCE = 
	ATD::Shader2D::PLAIN_VERTEX_SOURCE;

const std::string ATD::Shader2D::ALPHA_FRAGMENT_SOURCE = 
"#version 130\n"
"\n"
"varying vec2 varTexCoord;\n"
"varying vec4 varColor;\n"
"\n"
"uniform vec2 unfCanvasSize;\n"
"uniform sampler2D unfCanvasTexture;\n"
"\n"
"uniform sampler2D unfTexture1;\n"
"\n"
"\n"
"void main()\n"
"{\n"
/* Probe canvas - used for transparency handling. */
"    vec2 cvCoord = gl_FragCoord.xy / unfCanvasSize;\n"
"    vec4 cvColor = texture2D(unfCanvasTexture, cvCoord);\n"
"\n"
/* Probe the texture, being drawn. */
"    vec4 txColor = texture2D(unfTexture1, varTexCoord);\n"
"    vec4 rsColor = txColor * varColor;\n"
"\n"
/* Mix it up. */
"    gl_FragColor = \n"
"        vec4(\n"
"            mix(cvColor.xyz, rsColor.xyz, rsColor.w), \n"
"            clamp(cvColor.w + rsColor.w, 0.f, 1.f));\n"
/*
"    gl_FragColor = \n"
"        vec4(\n"
"            vec3(\n"
"                clamp(rsColor.x + rsColor.y + rsColor.z, 0.f, 1.f)), \n"
"            1.f);\n"
*/
"}\n";


