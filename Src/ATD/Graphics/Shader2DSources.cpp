/**
 * @file      
 * @brief     Common Shader2D sources.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#include <ATD/Graphics/Shader.hpp>

/* '#version 130' Should be added within the macro, 
 * because it confuses C++ compilers when unquoted. */
#define STRINGIFY_SHADER_130(code) \
	std::string("#version 130\n\n") + std::string(#code)


const std::string ATD::Shader2D::PLAIN_VERTEX_SOURCE = 
STRINGIFY_SHADER_130(
	attribute vec2 atrPosition;
	attribute vec2 atrTexCoord;
	attribute vec4 atrColor;

	varying vec2 varTexCoord;
	varying vec4 varColor;

	uniform mat3 unfProject;
	uniform mat3 unfTransform;


	void main()
	{
		vec3 position = unfTransform * vec3(atrPosition, 1.f);
		vec3 projection = unfProject * position;

		varTexCoord = atrTexCoord;
		varColor = atrColor;
		gl_Position = vec4(projection.xy, 0.f, 1.f);
	}
);


const std::string ATD::Shader2D::PLAIN_FRAGMENT_SOURCE = 
STRINGIFY_SHADER_130(
	varying vec2 varTexCoord;
	varying vec4 varColor;

	uniform sampler2D unfTexture0;


	void main()
	{
		vec4 txColor = texture2D(unfTexture0, varTexCoord);
		vec4 rsColor = txColor * varColor;
		gl_FragColor = rsColor;
	}
);


/* Simple 2D shader for drawing textures with transparency. */

const std::string ATD::Shader2D::ALPHA_VERTEX_SOURCE = 
	ATD::Shader2D::PLAIN_VERTEX_SOURCE;

const std::string ATD::Shader2D::ALPHA_FRAGMENT_SOURCE = 
STRINGIFY_SHADER_130(
	varying vec2 varTexCoord;
	varying vec4 varColor;

	uniform vec2 unfCanvasSize;
	uniform sampler2D unfCanvasTexture;

	uniform sampler2D unfTexture1;


	void main()
	{
		/* Probe canvas - used for transparency handling. */
		vec2 cvCoord = gl_FragCoord.xy / unfCanvasSize;
		vec4 cvColor = texture2D(unfCanvasTexture, cvCoord);

		/* Probe the texture, being drawn. */
		vec4 txColor = texture2D(unfTexture1, varTexCoord);
		vec4 rsColor = txColor * varColor;

		/* Mix it up. */
		gl_FragColor = 
			vec4(
				mix(cvColor.xyz, rsColor.xyz, rsColor.w), 
				clamp(cvColor.w + rsColor.w, 0.f, 1.f));
	}
);


