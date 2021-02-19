/**
 * @file      
 * @brief     Common Shader3D sources.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#include <ATD/Core/Printf.hpp>
#include <ATD/Graphics/Shader.hpp>

/* '#version 130' Should be added within the macro, 
 * because it confuses C++ compilers when unquoted. */
#define STRINGIFY_SHADER_130(code) \
	std::string("#version 130\n\n") + std::string(#code)


/* Basic Shader3D for drawing textured model without normals: */

const std::string ATD::Shader3D::PLAIN_VERTEX_SOURCE = 
STRINGIFY_SHADER_130(
	attribute vec3 atrPosition;
	attribute vec2 atrTexCoord;
	attribute vec4 atrColor;

	varying vec2 varTexCoord;
	varying vec4 varColor;

	uniform mat4 unfTransform;
	uniform mat4 unfProject;


	void main()
	{
		vec4 position = unfTransform * vec4(atrPosition, 1.f);

		varTexCoord = atrTexCoord;
		varColor = atrColor;
		gl_Position = unfProject * position;
	}
);

const std::string ATD::Shader3D::PLAIN_FRAGMENT_SOURCE = 
STRINGIFY_SHADER_130(
	varying vec2 varTexCoord;
	varying vec4 varColor;

	uniform sampler2D unfTexture0;
/*	uniform sampler2D unfTextureCanvas; */
/*	uniform sampler2D unfTexture1; */


	void main()\n
	{
		vec4 txColor = texture2D(unfTexture0, varTexCoord);
		gl_FragColor = txColor * varColor;
	}
);


/* Shader for drawing illuminated model with normals: */

const std::string ATD::Shader3D::LIGHT_VERTEX_SOURCE = 
STRINGIFY_SHADER_130(
	attribute vec3 atrPosition;
	attribute vec2 atrTexCoord;
	attribute vec3 atrNormal;
	attribute vec4 atrColor;

	varying vec3 varPosition;
	varying vec2 varTexCoord;
	varying vec3 varNormal;
	varying vec4 varColor;

	uniform mat4 unfTransform;
	uniform mat4 unfProject;


	void main()
	{
		mat3 vec3Transform;
		vec3Transform[0] = unfTransform[0].xyz;
		vec3Transform[1] = unfTransform[1].xyz;
		vec3Transform[2] = unfTransform[2].xyz;

		vec4 position = unfTransform * vec4(atrPosition, 1.f);

		varPosition = position.xyz;
		varTexCoord = atrTexCoord;
		varNormal = normalize(vec3Transform * atrNormal);
		varColor = atrColor;
		gl_Position = unfProject * position;
	}
);

const std::string ATD::Shader3D::LIGHT_FRAGMENT_SOURCE_TEMPLATE = 
STRINGIFY_SHADER_130(
	varying vec3 varPosition;
	varying vec2 varTexCoord;
	varying vec3 varNormal;
	varying vec4 varColor;

	uniform sampler2D unfTexture0;
/*	uniform sampler2D unfTextureCanvas; */
/*	uniform sampler2D unfTexture1; */

	uniform vec3 unfCameraPos;


	/* Lighting types: */

	/* Parameters, common for all lights. */
	struct Light
	{
		vec3 color;
		float ambient;
		float diffuse;
		float specular;
	};

	/* Directional light. */
	struct DirLight
	{
		Light lt;
		vec3 direction;
	};

	/* Point light. */
	struct PointLight
	{
		Light lt;
		vec3 position;
		vec3 attenuation;
	};

	/* Spot light. */
	struct SpotLight
	{
		PointLight plt;
		vec3 direction;
		float cutoff;
	};

	/* Lighting parameters: */

	const int MAX_DIR_LIGHTS = %u; /* Template arg #0. */
	uniform int unfDirLightsNum;
	uniform DirLight unfDirLights[MAX_DIR_LIGHTS];

	const int MAX_POINT_LIGHTS = %u; /* Template arg #1. */
	uniform int unfPointLightsNum;
	uniform PointLight unfPointLights[MAX_POINT_LIGHTS];

	const int MAX_SPOT_LIGHTS = %u; /* Template arg #2. */
	uniform int unfSpotLightsNum;
	uniform SpotLight unfSpotLights[MAX_SPOT_LIGHTS];


	/* Material parameters: */

	uniform float unfSpecularPower;


	vec3 ApplyLight(vec3 fgPosition, vec3 fgNormal, 
		Light lt, vec3 ltDirection, float ltIntensity)
	{
		vec3 ltDirection1 = normalize(ltDirection);
		vec3 fgNormal1 = normalize(fgNormal);
		vec3 cmDirection1 = normalize(unfCameraPos - fgPosition);

		float ftDiffuse = dot(ltDirection1, fgNormal1 * (-1.f));
		float ftSpecular = pow(
			max(
				dot(
					cmDirection1, 
					normalize(reflect(ltDirection1, fgNormal1))
				), 
				0.f), 
			unfSpecularPower);

		vec3 ltAmbient = lt.color * lt.ambient * ltIntensity;
		vec3 ltDiffuse = (ftDiffuse > 0.f) ? 
			lt.color * lt.diffuse * ltIntensity * ftDiffuse : 
			vec3(0.f);
		vec3 ltSpecular = (ftDiffuse > 0.f && ftSpecular > 0.f) ? 
			lt.color * lt.specular * ltIntensity * ftSpecular * 
				smoothstep(0.0f, 0.2f, ftDiffuse) : 
			vec3(0.f);

		vec3 light = ltAmbient + ltDiffuse + ltSpecular;
		return light;
	}

	vec3 ApplyDirLight(vec3 fgPosition, vec3 fgNormal, DirLight dlt)
	{
		return ApplyLight(fgPosition, fgNormal, dlt.lt, dlt.direction, 
			1.f);
	}

	vec3 ApplyPointLight(vec3 fgPosition, vec3 fgNormal, PointLight plt)
	{
		float distance = length(fgPosition - plt.position);

		vec3 ltDirection = normalize(fgPosition - plt.position);
		float ltIntensity = 1.f / (plt.attenuation.x + 
			plt.attenuation.y * distance + 
			plt.attenuation.z * distance * distance);

		return ApplyLight(fgPosition, fgNormal, plt.lt, ltDirection, 
			ltIntensity);
	}

	vec3 ApplySpotLight(vec3 fgPosition, vec3 fgNormal, SpotLight slt)
	{
		float distance = length(fgPosition - slt.plt.position);
		vec3 ltDirection = normalize(fgPosition - slt.plt.position);
		float ftSpot = dot(ltDirection, slt.direction);

		if (ftSpot > slt.cutoff) {
			float ltIntensity = (1.f / (slt.plt.attenuation.x + 
						slt.plt.attenuation.y * distance + 
						slt.plt.attenuation.z * distance * distance)) * 
				(1.f - (1.f - ftSpot) / (1.f - slt.cutoff));

			return ApplyLight(fgPosition, fgNormal, slt.plt.lt, ltDirection, 
				ltIntensity);
		} else {
			return vec3(0.f);
		}
	}


	void main()
	{
		/*   Light. */
		vec3 light = vec3(0.f);
		for(int dltIndex = 0; dltIndex < unfDirLightsNum; dltIndex++) {
			light += ApplyDirLight(varPosition, varNormal, 
				unfDirLights[dltIndex]);
		}
		for(int pltIndex = 0; pltIndex < unfPointLightsNum; pltIndex++) {
			light += ApplyPointLight(varPosition, varNormal, 
				unfPointLights[pltIndex]);
		}
		for(int sltIndex = 0; sltIndex < unfSpotLightsNum; sltIndex++) {
			light += ApplySpotLight(varPosition, varNormal, 
				unfSpotLights[sltIndex]);
		}

		/*   Texel color. */
		vec4 txColor = texture2D(unfTexture0, varTexCoord);

		gl_FragColor = txColor * varColor * vec4(light, 1.f);
	}
);

const unsigned ATD::Shader3D::LIGHT_DFT_MAX_DIR_LIGHTS   = 1;
const unsigned ATD::Shader3D::LIGHT_DFT_MAX_POINT_LIGHTS = 3;
const unsigned ATD::Shader3D::LIGHT_DFT_MAX_SPOT_LIGHTS  = 1;

const std::string ATD::Shader3D::LIGHT_FRAGMENT_SOURCE = 
	ATD::Aux::printf(ATD::Shader3D::LIGHT_FRAGMENT_SOURCE_TEMPLATE.c_str(), 
			ATD::Shader3D::LIGHT_DFT_MAX_DIR_LIGHTS, 
			ATD::Shader3D::LIGHT_DFT_MAX_POINT_LIGHTS, 
			ATD::Shader3D::LIGHT_DFT_MAX_SPOT_LIGHTS);


