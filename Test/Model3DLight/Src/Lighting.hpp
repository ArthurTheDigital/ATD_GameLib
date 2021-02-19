
#pragma once

#include <ATD/Core/Printf.hpp>
#include <ATD/Core/Vector3.hpp>
#include <ATD/Graphics/Pixel.hpp>
#include <ATD/Graphics/Shader.hpp>


class Lighting
{
public:
	struct Light
	{
		inline Light(const ATD::Pixel &n_color, 
				float n_ambient, float n_diffuse, float n_specular)
			: color(n_color)
			, ambient(n_ambient)
			, diffuse(n_diffuse)
			, specular(n_specular)
			, colorVld(false)
			, ambientVld(false)
			, diffuseVld(false)
			, specularVld(false)
		{}

		ATD::Pixel color;
		float ambient;
		float diffuse;
		float specular;

		bool colorVld;
		bool ambientVld;
		bool diffuseVld;
		bool specularVld;
	};

	struct DirLight
	{
		static const unsigned MAX_COUNT;

		inline DirLight(const ATD::Pixel &n_color = ATD::Pixel(), 
				float n_ambient = 0.f, float n_diffuse = 0.f, 
				float n_specular = 0.f, 
				const ATD::Vector3F &n_direction = ATD::Vector3F())
			: lt(n_color, n_ambient, n_diffuse, n_specular)
			, direction(n_direction)
			, directionVld(false)
		{}

		Light lt;
		ATD::Vector3F direction;

		bool directionVld;
	};

	struct PointLight
	{
		static const unsigned MAX_COUNT;

		inline PointLight(const ATD::Pixel &n_color = ATD::Pixel(), 
				float n_ambient = 0.f, float n_diffuse = 0.f, 
				float n_specular = 0.f, 
				const ATD::Vector3F &n_position = ATD::Vector3F(), 
				const ATD::Vector3F &n_attenuation = ATD::Vector3F())
			: lt(n_color, n_ambient, n_diffuse, n_specular)
			, position(n_position)
			, attenuation(n_attenuation)
			, positionVld(false)
			, attenuationVld(false)
		{}

		Light lt;
		ATD::Vector3F position;
		ATD::Vector3F attenuation;

		bool positionVld;
		bool attenuationVld;
	};

	struct SpotLight
	{
		static const unsigned MAX_COUNT;

		inline SpotLight(const ATD::Pixel &n_color = ATD::Pixel(), 
				float n_ambient = 0.f, float n_diffuse = 0.f, 
				float n_specular = 0.f, 
				const ATD::Vector3F &n_position = ATD::Vector3F(), 
				const ATD::Vector3F &n_attenuation = ATD::Vector3F(), 
				const ATD::Vector3F &n_direction = ATD::Vector3F(), 
				float n_cutoff = 0.f)
			: plt(n_color, n_ambient, n_diffuse, n_specular, n_position, 
					n_attenuation)
			, direction(n_direction)
			, cutoff(n_cutoff)
			, directionVld(false)
			, cutoffVld(false)
		{}

		PointLight plt;
		ATD::Vector3F direction;
		float cutoff;

		bool directionVld;
		bool cutoffVld;
	};


	static const float SPECULAR_POWER;

	static const std::vector<DirLight> DFT_DIR_LIGHTS;
	static const std::vector<PointLight> DFT_POINT_LIGHTS;
	static const std::vector<SpotLight> DFT_SPOT_LIGHTS;


	Lighting();

	void setCameraPosition(const ATD::Vector3D &position);

	void onOffDirLights(); /* FIXME: onOff -> toggle */
	void onOffPointLights();
	void onOffSpotLights();

	ATD::Shader3D::Ptr getShader();


private:
	void setUnfDirLight(int index);
	void setUnfPointLight(int index);
	void setUnfSpotLight(int index);

	void setUnfDirLightsEnabled(bool enabled);
	void setUnfPointLightsEnabled(bool enabled);
	void setUnfSpotLightsEnabled(bool enabled);


	ATD::Shader3D::Ptr m_shaderPtr;

	std::vector<DirLight> m_dirLights;
	std::vector<PointLight> m_pointLights;
	std::vector<SpotLight> m_spotLights;

	bool m_dirLightsEnabled;
	bool m_pointLightsEnabled;
	bool m_spotLightsEnabled;
};


/* Lighting substructures constants: */

const unsigned Lighting::DirLight::MAX_COUNT = 
	ATD::Shader3D::LIGHT_DFT_MAX_DIR_LIGHTS;

const unsigned Lighting::PointLight::MAX_COUNT = 
	ATD::Shader3D::LIGHT_DFT_MAX_POINT_LIGHTS;

const unsigned Lighting::SpotLight::MAX_COUNT = 
	ATD::Shader3D::LIGHT_DFT_MAX_SPOT_LIGHTS;

/* Lighting constants: */

const float DFT_AMBIENT = 0.2f;
const float DFT_DIFFUSE = 0.4f;
const float DFT_SPECULAR = 0.4f;

const ATD::Vector3F DFT_ATTENUATION = ATD::Vector3F(1.00f, 0.00f, 0.10f);

const std::vector<Lighting::DirLight> Lighting::DFT_DIR_LIGHTS = {
	Lighting::DirLight(ATD::Pixel(0xFF, 0xF8, 0xE0), 
			DFT_AMBIENT, DFT_DIFFUSE, DFT_SPECULAR, 
			ATD::Vector3F( 0.f, -1.f,  0.2f))
};

const std::vector<Lighting::PointLight> Lighting::DFT_POINT_LIGHTS = {
	Lighting::PointLight(ATD::Pixel(0xFF, 0xFF, 0x80),
			DFT_AMBIENT, DFT_DIFFUSE, DFT_SPECULAR, 
			ATD::Vector3F( 0.000f,  1.000f,  0.000f), 
			DFT_ATTENUATION), 

	Lighting::PointLight(ATD::Pixel(0xFF, 0x80, 0xFF),
			DFT_AMBIENT, DFT_DIFFUSE, DFT_SPECULAR, 
			ATD::Vector3F( 0.866f, -0.500f,  0.000f), 
			DFT_ATTENUATION), 

	Lighting::PointLight(ATD::Pixel(0x80, 0xFF, 0xFF),
			DFT_AMBIENT, DFT_DIFFUSE, DFT_SPECULAR, 
			ATD::Vector3F(-0.866f, -0.500f,  0.000f), 
			DFT_ATTENUATION)
};

const std::vector<Lighting::SpotLight> Lighting::DFT_SPOT_LIGHTS = {
	Lighting::SpotLight(ATD::Pixel(0x80, 0xC0, 0xFF),
			DFT_AMBIENT, DFT_DIFFUSE, DFT_SPECULAR, 
			ATD::Vector3F(0.000f, 0.000f, -10.000f), 
			DFT_ATTENUATION, 
			ATD::Vector3F(0.f, 0.f, 1.f), 
			0.940f) /* Spot light cutoff - cos(angle). */
};

const float Lighting::SPECULAR_POWER = 3.0f;


/* === */

Lighting::Lighting()
	: m_shaderPtr(new ATD::Shader3D(ATD::Shader3D::LIGHT_VERTEX_SOURCE, 
				ATD::Shader3D::LIGHT_FRAGMENT_SOURCE))
	, m_dirLights(Lighting::DFT_DIR_LIGHTS)
	, m_pointLights(Lighting::DFT_POINT_LIGHTS)
	, m_spotLights(Lighting::DFT_SPOT_LIGHTS)
	, m_dirLightsEnabled(true)
	, m_pointLightsEnabled(false)
	, m_spotLightsEnabled(false)
{
	m_shaderPtr->setUniform("unfCameraPos", 
			static_cast<ATD::Vector3F>(ATD::Vector3D(0., 0., 0.)));

	/* Set light uniforms in shader. */
	m_shaderPtr->setUniform("unfSpecularPower", SPECULAR_POWER);

	if (m_dirLights.size() > DirLight::MAX_COUNT) {
		m_dirLights.resize(DirLight::MAX_COUNT);
	}
	for (int dlIndex = 0; dlIndex < static_cast<int>(m_dirLights.size()); 
			dlIndex++) {
		setUnfDirLight(dlIndex);
	}
	setUnfDirLightsEnabled(m_dirLightsEnabled);

	if (m_pointLights.size() > PointLight::MAX_COUNT) {
		m_pointLights.resize(PointLight::MAX_COUNT);
	}
	for (int plIndex = 0; plIndex < static_cast<int>(m_pointLights.size()); 
			plIndex++) {
		setUnfPointLight(plIndex);
	}
	setUnfPointLightsEnabled(m_pointLightsEnabled);

	if (m_spotLights.size() > SpotLight::MAX_COUNT) {
		m_spotLights.resize(SpotLight::MAX_COUNT);
	}
	for (int slIndex = 0; slIndex < static_cast<int>(m_spotLights.size()); 
			slIndex++) {
		setUnfSpotLight(slIndex);
	}
	setUnfSpotLightsEnabled(m_spotLightsEnabled);
}

void Lighting::setCameraPosition(const ATD::Vector3D &position)
{
	m_shaderPtr->setUniform("unfCameraPos", 
			static_cast<ATD::Vector3F>(position));
}

void Lighting::onOffDirLights()
{
	m_dirLightsEnabled = !m_dirLightsEnabled;
	setUnfDirLightsEnabled(m_dirLightsEnabled);
}

void Lighting::onOffPointLights()
{
	m_pointLightsEnabled = !m_pointLightsEnabled;
	setUnfPointLightsEnabled(m_pointLightsEnabled);
}

void Lighting::onOffSpotLights()
{
	m_spotLightsEnabled = !m_spotLightsEnabled;
	setUnfSpotLightsEnabled(m_spotLightsEnabled);
}

ATD::Shader3D::Ptr Lighting::getShader()
{ return m_shaderPtr; }

/* === */

void Lighting::setUnfDirLight(int index)
{
	const std::string structName = ATD::Aux::printf("unfDirLights[%d].", index);
	DirLight &dl = m_dirLights[index];

	if (!dl.lt.colorVld) {
		m_shaderPtr->setUniform(structName + "lt.color", 
				dl.lt.color.glColor3());
		dl.lt.colorVld = true;
	}

	if (!dl.lt.ambientVld) {
		m_shaderPtr->setUniform(structName + "lt.ambient", dl.lt.ambient);
		dl.lt.ambientVld = true;
	}

	if (!dl.lt.diffuseVld) {
		m_shaderPtr->setUniform(structName + "lt.diffuse", dl.lt.diffuse);
		dl.lt.diffuseVld = true;
	}

	if (!dl.lt.specularVld) {
		m_shaderPtr->setUniform(structName + "lt.specular", dl.lt.specular);
		dl.lt.specularVld = true;
	}

	if (!dl.directionVld) {
		m_shaderPtr->setUniform(structName + "direction", dl.direction);
		dl.directionVld = true;
	}
}

void Lighting::setUnfPointLight(int index)
{
	const std::string structName = ATD::Aux::printf("unfPointLights[%d].", index);
	PointLight &pl = m_pointLights[index];

	if (!pl.lt.colorVld) {
		m_shaderPtr->setUniform(structName + "lt.color", pl.lt.color.glColor3());
		pl.lt.colorVld = true;
	}

	if (!pl.lt.ambientVld) {
		m_shaderPtr->setUniform(structName + "lt.ambient", pl.lt.ambient);
		pl.lt.ambientVld = true;
	}

	if (!pl.lt.diffuseVld) {
		m_shaderPtr->setUniform(structName + "lt.diffuse", pl.lt.diffuse);
		pl.lt.diffuseVld = true;
	}

	if (!pl.lt.specularVld) {
		m_shaderPtr->setUniform(structName + "lt.specular", pl.lt.specular);
		pl.lt.specularVld = true;
	}

	if (!pl.positionVld) {
		m_shaderPtr->setUniform(structName + "position", pl.position);
		pl.positionVld = true;
	}

	if (!pl.attenuationVld) {
		m_shaderPtr->setUniform(structName + "attenuation", pl.attenuation);
		pl.attenuationVld = true;
	}
}

void Lighting::setUnfSpotLight(int index)
{
	const std::string structName = ATD::Aux::printf("unfSpotLights[%d].", index);
	SpotLight &sl = m_spotLights[index];

	if (!sl.plt.lt.colorVld) {
		m_shaderPtr->setUniform(structName + "plt.lt.color", 
				sl.plt.lt.color.glColor3());
		sl.plt.lt.colorVld = true;
	}

	if (!sl.plt.lt.ambientVld) {
		m_shaderPtr->setUniform(structName + "plt.lt.ambient", 
				sl.plt.lt.ambient);
		sl.plt.lt.ambientVld = true;
	}

	if (!sl.plt.lt.diffuseVld) {
		m_shaderPtr->setUniform(structName + "plt.lt.diffuse", 
				sl.plt.lt.diffuse);
		sl.plt.lt.diffuseVld = true;
	}

	if (!sl.plt.lt.specularVld) {
		m_shaderPtr->setUniform(structName + "plt.lt.specular", 
				sl.plt.lt.specular);
		sl.plt.lt.specularVld = true;
	}

	if (!sl.plt.positionVld) {
		m_shaderPtr->setUniform(structName + "plt.position", 
				sl.plt.position);
		sl.plt.positionVld = true;
	}

	if (!sl.plt.attenuationVld) {
		m_shaderPtr->setUniform(structName + "plt.attenuation", 
				sl.plt.attenuation);
		sl.plt.attenuationVld = true;
	}

	if (!sl.directionVld) {
		m_shaderPtr->setUniform(structName + "direction", sl.direction);
		sl.directionVld = true;
	}

	if (!sl.cutoffVld) {
		m_shaderPtr->setUniform(structName + "cutoff", sl.cutoff);
		sl.cutoffVld = true;
	}
}


void Lighting::setUnfDirLightsEnabled(bool enabled)
{
	m_shaderPtr->setUniform("unfDirLightsNum", 
			enabled ? static_cast<int>(m_dirLights.size()) : 0);
}

void Lighting::setUnfPointLightsEnabled(bool enabled)
{
	m_shaderPtr->setUniform("unfPointLightsNum", 
			enabled ? static_cast<int>(m_pointLights.size()) : 0);
}

void Lighting::setUnfSpotLightsEnabled(bool enabled)
{
	m_shaderPtr->setUniform("unfSpotLightsNum", 
			enabled ? static_cast<int>(m_spotLights.size()) : 0);
}


