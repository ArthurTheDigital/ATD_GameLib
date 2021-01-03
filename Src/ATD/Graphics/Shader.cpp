/**
 * @file      
 * @brief     Wrap around OpenGL shader.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#include <ATD/Core/Debug.hpp>
#include <ATD/Core/Printf.hpp>
#include <ATD/Graphics/GlCheck.hpp>
#include <ATD/Graphics/Shader.hpp>

#include <set>
#include <stdexcept>


/* ATD::Shader auxiliary: */

/*
static bool IsSamplerType(const ATD::Gl::Enum type)
{
	const std::set<ATD::Gl::Enum> unfmTypeSamplerSet = {
		ATD::Gl::SAMPLER_2D, 
		ATD::Gl::SAMPLER_CUBE
		// FIXME: Add more
	};

	return (unfmTypeSamplerSet.find(type) != unfmTypeSamplerSet.end());
}
*/

static std::string UniformToStr(const std::string &unfmName, 
		const ATD::Gl::Enum &unfmType)
{
	const std::map<ATD::Gl::Enum, std::string> unfmTypeMap = {
		{ ATD::Gl::FLOAT, "float" }, 
		{ ATD::Gl::FLOAT_VEC2, "vec2" }, 
		{ ATD::Gl::FLOAT_VEC3, "vec3" }, 
		{ ATD::Gl::FLOAT_VEC4, "vec4" }, 

		{ ATD::Gl::FLOAT_MAT2, "mat2" }, 
		{ ATD::Gl::FLOAT_MAT3, "mat3" }, 
		{ ATD::Gl::FLOAT_MAT4, "mat4" }, 

		{ ATD::Gl::INT, "int" }, 
		{ ATD::Gl::INT_VEC2, "ivec2" }, 
		{ ATD::Gl::INT_VEC3, "ivec3" }, 
		{ ATD::Gl::INT_VEC4, "ivec4" }, 

		{ ATD::Gl::SAMPLER_2D, "sampler2D" }, 
		{ ATD::Gl::SAMPLER_CUBE, "samplerCube" }

		/* FIXME: Add more
		 * (full table can be found on the site)
		 * https://docs.gl/gl4/glGetActiveUniform */
	};

	auto unfmTypeIter = unfmTypeMap.find(unfmType);
	std::string unfmTypeStr = unfmTypeIter != unfmTypeMap.end() ? 
		unfmTypeIter->second : 
		std::string("????");

	return (std::string("uniform ") + unfmTypeStr + std::string(" ") + 
			unfmName);
}


typedef std::pair<std::string, ATD::Gl::Enum> AttrDesc;
typedef std::map<AttrDesc, ATD::Gl::Uint> AttrIndexMap;

static void FillAttrIndexMap(const ATD::Shader &shader, 
		AttrIndexMap &indexMap)
{
	ATD::Shader::Usage use(shader);
	ATD::Gl::Uint attrCount;
	ATD::gl._GetProgramiv(shader.GetGlId(), ATD::Gl::ACTIVE_ATTRIBUTES, 
			reinterpret_cast<ATD::Gl::Int *>(&attrCount));

	const size_t attrNameBufferSize = 1024;
	std::string attrNameBuffer(attrNameBufferSize, '\0');
	for (ATD::Gl::Uint attrIndex = 0; attrIndex < attrCount; attrIndex++) {

		ATD::Gl::Sizei attrNameLength;
		ATD::Gl::Int attrSize;
		ATD::Gl::Enum attrType;

		ATD::gl._GetActiveAttrib(shader.GetGlId(), 
				attrIndex, 
				attrNameBufferSize, 
				&attrNameLength, 
				&attrSize, 
				&attrType, 
				reinterpret_cast<ATD::Gl::Char *>(&attrNameBuffer[0]));

		std::string attrName = attrNameBuffer.substr(0, attrNameLength);
		AttrDesc attrDesc(attrName, attrType);

		indexMap[attrDesc] = attrIndex;
	}
}

static std::string AttrDescToStr(const AttrDesc &attrDesc)
{
	/* There is a small amount of types, that OpenGL allow to be shader 
	 * attributes. Hence, I think, it is faster to just if/else through 
	 * them, than creating an std::map. */
	std::string typeStr = 
		attrDesc.second == ATD::Gl::FLOAT ? "float" : 
		attrDesc.second == ATD::Gl::FLOAT_VEC2 ? "vec2" : 
		attrDesc.second == ATD::Gl::FLOAT_VEC3 ? "vec3" : 
		attrDesc.second == ATD::Gl::FLOAT_VEC4 ? "vec4" : 
		attrDesc.second == ATD::Gl::FLOAT_MAT2 ? "mat2" : 
		attrDesc.second == ATD::Gl::FLOAT_MAT3 ? "mat3" : 
		attrDesc.second == ATD::Gl::FLOAT_MAT4 ? "mat4" : 
		"????";

	return (std::string("attribute ") + typeStr + std::string(" ") + 
			attrDesc.first);
}


/* ATD::Shader::Usage */

ATD::Shader::Usage::Usage(const ATD::Shader &shader)
	: m_prevProgram(0)
	, m_activated(false)
{
	gl._GetIntegerv(Gl::CURRENT_PROGRAM, 
			reinterpret_cast<Gl::Int *>(&m_prevProgram));

	if (m_prevProgram != shader.GetGlId()) {
		gl._UseProgram(shader.GetGlId());
		m_activated = true;
	}
}

ATD::Shader::Usage::~Usage()
{
	if (m_activated) { gl._UseProgram(m_prevProgram); }
}


/* ATD::Shader */

ATD::Shader::Shader(const std::string &vertexSource, 
		const std::string &fragmentSource)
	: m_program(0)
	, m_vertexShaderId(0)
	, m_fragmentShaderId(0)
	, m_uniformLocationMap()
	, m_isCanvasRequired(false)
{
	m_program = gl._CreateProgram();

	if (m_program == 0) {
		throw std::runtime_error("Cannot create new shader program");
	}
	/* IPRINTF("", "created new shader program %u", m_program); // DEBUG */


	CompileShader(vertexSource, Gl::VERTEX_SHADER, &m_vertexShaderId);
	CompileShader(fragmentSource, Gl::FRAGMENT_SHADER, &m_fragmentShaderId);

	Gl::Int success = 0;
	const size_t errorLogSize = 1024;
	std::string errorLog(errorLogSize, '\0');

	gl._LinkProgram(m_program);
	gl._GetProgramiv(m_program, Gl::LINK_STATUS, &success);
	if (!success) {
		gl._GetProgramInfoLog(m_program, errorLogSize, nullptr, 
				reinterpret_cast<Gl::Char *>(&errorLog[0]));

		throw std::runtime_error(Printf(
					"Cannot link shader program %u\n%s\n", 
					m_program, errorLog.c_str()));
	}

	gl._ValidateProgram(m_program);
	gl._GetProgramiv(m_program, Gl::VALIDATE_STATUS, &success);
	if (!success) {
		gl._GetProgramInfoLog(m_program, errorLogSize, nullptr, 
				reinterpret_cast<Gl::Char *>(&errorLog[0]));

		throw std::runtime_error(Printf(
					"Cannot validate shader program %u\n%s\n", 
					m_program, errorLog.c_str()));
	}

	InitUniforms();
}

ATD::Shader::~Shader()
{
	if (m_program) { gl._DeleteProgram(m_program); }
	if (m_vertexShaderId) { gl._DeleteShader(m_vertexShaderId); }
	if (m_fragmentShaderId) { gl._DeleteShader(m_fragmentShaderId); }
}

void ATD::Shader::SetUniform(const std::string &name, float value)
{
	const Gl::Enum type = Gl::FLOAT;
	auto unfmLDIter = m_uniformLocationMap.find(name);
	if (unfmLDIter != m_uniformLocationMap.end()) {
		if (unfmLDIter->second.type == type) {
			Usage use(*this);
			gl._Uniform1f(unfmLDIter->second.location, 
					static_cast<Gl::Float>(value));

			unfmLDIter->second.isSet = true;
		} else {
			throw std::runtime_error(Printf("no '%s' in shader %u", 
						UniformToStr(name, type).c_str(), m_program));
		}
	} else {
		throw std::runtime_error(Printf("no '%s' in shader %u", 
					UniformToStr(name, type).c_str(), m_program));
	}
}

void ATD::Shader::SetUniform(const std::string &name, int value)
{
	const Gl::Enum type = Gl::INT;
	auto unfmLDIter = m_uniformLocationMap.find(name);
	if (unfmLDIter != m_uniformLocationMap.end()) {
		if (unfmLDIter->second.type == type) {
			Usage use(*this);
			gl._Uniform1i(unfmLDIter->second.location, 
					static_cast<Gl::Int>(value));

			unfmLDIter->second.isSet = true;
		} else {
			throw std::runtime_error(Printf("no '%s' in shader %u", 
						UniformToStr(name, type).c_str(), m_program));
		}
	} else {
		throw std::runtime_error(Printf("no '%s' in shader %u", 
					UniformToStr(name, type).c_str(), m_program));
	}
}

void ATD::Shader::SetUniform(const std::string &name, 
		const ATD::Vector2F &value)
{
	const Gl::Enum type = Gl::FLOAT_VEC2;
	auto unfmLDIter = m_uniformLocationMap.find(name);
	if (unfmLDIter != m_uniformLocationMap.end()) {
		if (unfmLDIter->second.type == type) {
			Usage use(*this);
			gl._Uniform2f(unfmLDIter->second.location, 
					static_cast<Gl::Float>(value.x), 
					static_cast<Gl::Float>(value.y));

			unfmLDIter->second.isSet = true;
		} else {
			throw std::runtime_error(Printf("no '%s' in shader %u", 
						UniformToStr(name, type).c_str(), m_program));
		}
	} else {
		throw std::runtime_error(Printf("no '%s' in shader %u", 
					UniformToStr(name, type).c_str(), m_program));
	}
}

void ATD::Shader::SetUniform(const std::string &name, 
		const ATD::Vector2I &value)
{
	const Gl::Enum type = Gl::INT_VEC2;
	auto unfmLDIter = m_uniformLocationMap.find(name);
	if (unfmLDIter != m_uniformLocationMap.end()) {
		if (unfmLDIter->second.type == type) {
			Usage use(*this);
			gl._Uniform2i(unfmLDIter->second.location, 
					static_cast<Gl::Int>(value.x), 
					static_cast<Gl::Int>(value.y));

			unfmLDIter->second.isSet = true;
		} else {
			throw std::runtime_error(Printf("no '%s' in shader %u", 
						UniformToStr(name, type).c_str(), m_program));
		}
	} else {
		throw std::runtime_error(Printf("no '%s' in shader %u", 
					UniformToStr(name, type).c_str(), m_program));
	}
}

void ATD::Shader::SetUniform(const std::string &name, 
		const ATD::Vector3F &value)
{
	const Gl::Enum type = Gl::FLOAT_VEC3;
	auto unfmLDIter = m_uniformLocationMap.find(name);
	if (unfmLDIter != m_uniformLocationMap.end()) {
		if (unfmLDIter->second.type == type) {
			Usage use(*this);
			gl._Uniform3f(unfmLDIter->second.location, 
					static_cast<Gl::Float>(value.x), 
					static_cast<Gl::Float>(value.y), 
					static_cast<Gl::Float>(value.z));

			unfmLDIter->second.isSet = true;
		} else {
			throw std::runtime_error(Printf("no '%s' in shader %u", 
						UniformToStr(name, type).c_str(), m_program));
		}
	} else {
		throw std::runtime_error(Printf("no '%s' in shader %u", 
					UniformToStr(name, type).c_str(), m_program));
	}
}

void ATD::Shader::SetUniform(const std::string &name, 
		const ATD::Vector3I &value)
{
	const Gl::Enum type = Gl::INT_VEC3;
	auto unfmLDIter = m_uniformLocationMap.find(name);
	if (unfmLDIter != m_uniformLocationMap.end()) {
		if (unfmLDIter->second.type == type) {
			Usage use(*this);
			gl._Uniform3i(unfmLDIter->second.location, 
					static_cast<Gl::Int>(value.x), 
					static_cast<Gl::Int>(value.y), 
					static_cast<Gl::Int>(value.z));

			unfmLDIter->second.isSet = true;
		} else {
			throw std::runtime_error(Printf("no '%s' in shader %u", 
						UniformToStr(name, type).c_str(), m_program));
		}
	} else {
		throw std::runtime_error(Printf("no '%s' in shader %u", 
					UniformToStr(name, type).c_str(), m_program));
	}
}

void ATD::Shader::SetUniform(const std::string &name, 
		const ATD::Vector4F &value)
{
	const Gl::Enum type = Gl::FLOAT_VEC4;
	auto unfmLDIter = m_uniformLocationMap.find(name);
	if (unfmLDIter != m_uniformLocationMap.end()) {
		if (unfmLDIter->second.type == type) {
			Usage use(*this);
			gl._Uniform4f(unfmLDIter->second.location, 
					static_cast<Gl::Float>(value.x), 
					static_cast<Gl::Float>(value.y), 
					static_cast<Gl::Float>(value.z), 
					static_cast<Gl::Float>(value.w));

			unfmLDIter->second.isSet = true;
		} else {
			throw std::runtime_error(Printf("no '%s' in shader %u", 
						UniformToStr(name, type).c_str(), m_program));
		}
	} else {
		throw std::runtime_error(Printf("no '%s' in shader %u", 
					UniformToStr(name, type).c_str(), m_program));
	}
}

void ATD::Shader::SetUniform(const std::string &name, 
		const ATD::Vector4I &value)
{
	const Gl::Enum type = Gl::INT_VEC4;
	auto unfmLDIter = m_uniformLocationMap.find(name);
	if (unfmLDIter != m_uniformLocationMap.end()) {
		if (unfmLDIter->second.type == type) {
			Usage use(*this);
			gl._Uniform4i(unfmLDIter->second.location, 
					static_cast<Gl::Int>(value.x), 
					static_cast<Gl::Int>(value.y), 
					static_cast<Gl::Int>(value.z), 
					static_cast<Gl::Int>(value.w));

			unfmLDIter->second.isSet = true;
		} else {
			throw std::runtime_error(Printf("no '%s' in shader %u", 
						UniformToStr(name, type).c_str(), m_program));
		}
	} else {
		throw std::runtime_error(Printf("no '%s' in shader %u", 
					UniformToStr(name, type).c_str(), m_program));
	}
}

void ATD::Shader::SetUniform(const std::string &name, 
		const ATD::Matrix3F &value)
{
	const Gl::Enum type = Gl::FLOAT_MAT3;
	auto unfmLDIter = m_uniformLocationMap.find(name);
	if (unfmLDIter != m_uniformLocationMap.end()) {
		if (unfmLDIter->second.type == type) {
			Usage use(*this);
			gl._UniformMatrix3fv(unfmLDIter->second.location, 1, Gl::TRUE, 
					reinterpret_cast<const Gl::Float *>(&value));

			unfmLDIter->second.isSet = true;
		} else {
			throw std::runtime_error(Printf("no '%s' in shader %u", 
						UniformToStr(name, type).c_str(), m_program));
		}
	} else {
		throw std::runtime_error(Printf("no '%s' in shader %u", 
					UniformToStr(name, type).c_str(), m_program));
	}
}

void ATD::Shader::SetUniform(const std::string &name, 
		const ATD::Matrix4F &value)
{
	const Gl::Enum type = Gl::FLOAT_MAT4;
	auto unfmLDIter = m_uniformLocationMap.find(name);
	if (unfmLDIter != m_uniformLocationMap.end()) {
		if (unfmLDIter->second.type == type) {
			Usage use(*this);
			gl._UniformMatrix4fv(unfmLDIter->second.location, 1, Gl::TRUE, 
					reinterpret_cast<const Gl::Float *>(&value));

			unfmLDIter->second.isSet = true;
		} else {
			throw std::runtime_error(Printf("no '%s' in shader %u", 
						UniformToStr(name, type).c_str(), m_program));
		}
	} else {
		throw std::runtime_error(Printf("no '%s' in shader %u", 
					UniformToStr(name, type).c_str(), m_program));
	}
}

void ATD::Shader::SetUniformSampler2DUnit(const std::string &name, 
		const ATD::Texture::Unit &unit)
{
	const Gl::Enum type = Gl::SAMPLER_2D;
	auto unfmLDIter = m_uniformLocationMap.find(name);
	if (unfmLDIter != m_uniformLocationMap.end()) {
		if (unfmLDIter->second.type == type) {
			Usage use(*this);
			/* Yes, that's right. Int. */
			gl._Uniform1i(
					unfmLDIter->second.location, 
					static_cast<Gl::Int>(
						static_cast<unsigned>(unit)));

			unfmLDIter->second.isSet = true;
		} else {
			throw std::runtime_error(Printf("no '%s' in shader %u", 
						UniformToStr(name, type).c_str(), m_program));
		}
	} else {
		throw std::runtime_error(Printf("no '%s' in shader %u", 
					UniformToStr(name, type).c_str(), m_program));
	}
}

void ATD::Shader::SetUniformSamplerCubeUnit(const std::string &name, 
		const ATD::Texture::Unit &unit)
{
	const Gl::Enum type = Gl::SAMPLER_CUBE;
	auto unfmLDIter = m_uniformLocationMap.find(name);
	if (unfmLDIter != m_uniformLocationMap.end()) {
		if (unfmLDIter->second.type == type) {
			Usage use(*this);
			/* Yes, that's right. Int. */
			gl._Uniform1i(
					unfmLDIter->second.location, 
					static_cast<Gl::Int>(
						static_cast<unsigned>(unit)));

			unfmLDIter->second.isSet = true;
		} else {
			throw std::runtime_error(Printf("no '%s' in shader %u", 
						UniformToStr(name, type).c_str(), m_program));
		}
	} else {
		throw std::runtime_error(Printf("no '%s' in shader %u", 
					UniformToStr(name, type).c_str(), m_program));
	}
}

void ATD::Shader::CheckIfUniformsSet() const
{
	std::vector<std::string> unfmNamesUnset;
	for (auto unfmLDIter = m_uniformLocationMap.begin(); 
			unfmLDIter != m_uniformLocationMap.end(); 
			unfmLDIter++) {
		if (!unfmLDIter->second.isSet) {
			unfmNamesUnset.push_back(UniformToStr(unfmLDIter->first, 
						unfmLDIter->second.type));
		}
	}
	if (unfmNamesUnset.size()) {
		std::string uUnsetStr = unfmNamesUnset[0];
		for (size_t uIndex = 1; uIndex < unfmNamesUnset.size(); uIndex++) {
			uUnsetStr += std::string("; ") + unfmNamesUnset[uIndex];
		}
		throw std::runtime_error(Printf(
					"shader %u has uniforms uninitialized:\n%s", 
					m_program, uUnsetStr.c_str()));
	}
}

ATD::Gl::Uint ATD::Shader::GetGlId() const
{ return m_program; }

bool ATD::Shader::IsCanvasRequired() const
{ return m_isCanvasRequired; }

void ATD::Shader::CompileShader(const std::string &shaderSource, 
		ATD::Gl::Enum shaderType, 
		ATD::Gl::Uint *shaderId)
{
	*shaderId = gl._CreateShader(shaderType);
	if (*shaderId == 0) {
		throw std::runtime_error(
				Printf(
					"Cannot create new %s shader", 
					shaderType == Gl::VERTEX_SHADER ? "vertex" : 
					shaderType == Gl::FRAGMENT_SHADER ? "fragment" : 
					"unknown type"
					)
				);
	}

	const Gl::Char *shaderSourceStr = 
		reinterpret_cast<const Gl::Char *>(shaderSource.data());

	Gl::Int shaderSourceLen = static_cast<Gl::Int>(shaderSource.size());
	gl._ShaderSource(*shaderId, 1, &shaderSourceStr, &shaderSourceLen);

	Gl::Int success = 0;
	const size_t errorLogSize = 1024;
	std::string errorLog(errorLogSize, '\0');

	gl._CompileShader(*shaderId);
	gl._GetShaderiv(*shaderId, Gl::COMPILE_STATUS, &success);
	if (!success) {
		gl._GetShaderInfoLog(*shaderId, errorLogSize, nullptr, 
				reinterpret_cast<Gl::Char *>(&errorLog[0]));

		throw std::runtime_error(Printf(
					"Cannot compile shader %u\n%s\n", 
					*shaderId, errorLog.c_str()));
	}

	gl._AttachShader(m_program, *shaderId);
}

void ATD::Shader::InitUniforms()
{
	/* Fill uniform location map. */
	Usage use(*this);
	Gl::Uint unfmCount;
	gl._GetProgramiv(m_program, Gl::ACTIVE_UNIFORMS, 
			reinterpret_cast<Gl::Int *>(&unfmCount));

	const size_t unfmNameBufferSize = 1024;
	std::string unfmNameBuffer(unfmNameBufferSize, '\0');

	for (Gl::Uint unfmIndex = 0; unfmIndex < unfmCount; unfmIndex++) {

		Gl::Sizei unfmNameLength;
		Gl::Int unfmSize;
		Gl::Enum unfmType;

		gl._GetActiveUniform(m_program, 
				unfmIndex, 
				unfmNameBufferSize, 
				&unfmNameLength, 
				&unfmSize, 
				&unfmType, 
				reinterpret_cast<Gl::Char *>(&unfmNameBuffer[0]));

		std::string unfmName = unfmNameBuffer.substr(0, unfmNameLength);
		Gl::Int unfmLocation = gl._GetUniformLocation(m_program, 
				unfmName.c_str());

		if (unfmLocation != static_cast<Gl::Int>(-1)) {
			m_uniformLocationMap.insert(
					std::pair<std::string, UniformLocationDesc>(
						unfmName, 
						UniformLocationDesc(unfmLocation, unfmType)));
		}
	}

	/* Check, whether it has uniform variables for canvas. */
	auto cvSizeLdIter = m_uniformLocationMap.find("unfCanvasSize");
	auto cvTextureLdIter = m_uniformLocationMap.find("unfCanvasTexture");

	if (cvSizeLdIter != m_uniformLocationMap.end() && 
			cvTextureLdIter != m_uniformLocationMap.end()) {

		if (cvSizeLdIter->second.type == Gl::FLOAT_VEC2 && 
				cvTextureLdIter->second.type == Gl::SAMPLER_2D) {

			m_isCanvasRequired = true;
		}
	}

	/* DEBUG */
	std::string uniformsStr;
	for (auto &udPair : m_uniformLocationMap) {
		if (uniformsStr.size()) {
			uniformsStr += 
				std::string(", ") + 
				UniformToStr(udPair.first, udPair.second.type);
		} else {
			uniformsStr = 
				UniformToStr(udPair.first, udPair.second.type);
		}
	}
	IPRINTF("", "%lu uniforms initialized in shader %u: [%s]", 
			m_uniformLocationMap.size(), 
			m_program, 
			uniformsStr.c_str()); // DEBUG */
}


/* ATD::Shader2D constants: */

/* ATD::Shader2d::PLAIN_VERTEX_SOURCE is set in Shader2DSources.cpp. */
/* ATD::Shader2d::PLAIN_FRAGMENT_SOURCE is set in Shader2DSources.cpp. */


/* Default 2D shader: */

const std::string ATD::Shader2D::DFT_VERTEX_SOURCE = 
ATD::Shader2D::PLAIN_VERTEX_SOURCE;

const std::string ATD::Shader2D::DFT_FRAGMENT_SOURCE = 
ATD::Shader2D::PLAIN_FRAGMENT_SOURCE;


/* ATD::Shader2D: */

ATD::Shader2D::Shader2D(const std::string &vertexSource, 
		const std::string &fragmentSource)
	: Shader(vertexSource, fragmentSource)
	, m_attrIndices()
{
	CheckAttributes();
}

const ATD::VertexBuffer2D::AttrIndices &ATD::Shader2D::GetAttrIndices() const
{ return m_attrIndices; }

void ATD::Shader2D::CheckAttributes()
{
	const AttrDesc positionADesc("atrPosition", Gl::FLOAT_VEC2);
	const AttrDesc texCoordsADesc("atrTexCoord", Gl::FLOAT_VEC2);
	const AttrDesc colorADesc("atrColor", Gl::FLOAT_VEC4);

	AttrIndexMap attrIndexMap;
	FillAttrIndexMap(*this, attrIndexMap);

	/* Check attributes index map: */
	{
		auto positionAIter = attrIndexMap.find(positionADesc);
		if (positionAIter != attrIndexMap.end()) {
			m_attrIndices.positionIndex = positionAIter->second;
			attrIndexMap.erase(positionAIter);
		} else {
			throw std::runtime_error(Printf(
						"cannot create Shader2D without '%s'", 
						AttrDescToStr(positionADesc).c_str()));
		}
	}
	{
		auto texCoordsAIter = attrIndexMap.find(texCoordsADesc);
		if (texCoordsAIter != attrIndexMap.end()) {
			m_attrIndices.texCoordsIndex = texCoordsAIter->second;
			m_attrIndices.texCoordsAreRequired = true;
			attrIndexMap.erase(texCoordsAIter);
		}
	}
	{
		auto colorAIter = attrIndexMap.find(colorADesc);
		if (colorAIter != attrIndexMap.end()) {
			m_attrIndices.colorIndex = colorAIter->second;
			m_attrIndices.colorIsRequired = true;
			attrIndexMap.erase(colorAIter);
		}
	}
	if (attrIndexMap.size()) {
		auto attrIter = attrIndexMap.begin();
		std::string attrStr = AttrDescToStr(attrIter->first);

		attrIter++;
		for (; attrIter != attrIndexMap.end(); attrIter++) {
			attrStr += std::string("; ") + AttrDescToStr(attrIter->first);
		}
		throw std::runtime_error(Printf(
					"unsupported attributes in Shader2D:\n%s", 
					attrStr.c_str()));
	}
}


/* ATD::Shader3D constants: */

/* ATD::Shader3D::PLAIN_VERTEX_SOURCE is set in Shader3DSources.cpp */
/* ATD::Shader3D::PLAIN_FRAGMENT_SOURCE is set in Shader3DSources.cpp */

/* ATD::Shader3D::LIGHT_VERTEX_SOURCE is set in Shader3DSources.cpp */
/* ATD::Shader3D::LIGHT_FRAGMENT_SOURCE is set in Shader3DSources.cpp */

/* ATD::Shader3D::LIGHT_FRAGMENT_SOURCE_TEMPLATE is set in 
   Shader3DSources.cpp */
/* ATD::Shader3D::LIGHT_DFT_MAX_DIR_LIGHTS is set in Shader3DSources.cpp */
/* ATD::Shader3D::LIGHT_DFT_MAX_POINT_LIGHTS is set in Shader3DSources.cpp */
/* ATD::Shader3D::LIGHT_DFT_MAX_SPOT_LIGHTS is set in Shader3DSources.cpp */


/* Default 3D shader: */

const std::string ATD::Shader3D::DFT_VERTEX_SOURCE = 
	ATD::Shader3D::PLAIN_VERTEX_SOURCE;

const std::string ATD::Shader3D::DFT_FRAGMENT_SOURCE = 
	ATD::Shader3D::PLAIN_FRAGMENT_SOURCE;


/* ATD::Shader3D: */

ATD::Shader3D::Shader3D(const std::string &vertexSource, 
		const std::string &fragmentSource)
	: Shader(vertexSource, fragmentSource)
	, m_attrIndices()
{
	CheckAttributes();
}

const ATD::VertexBuffer3D::AttrIndices &ATD::Shader3D::GetAttrIndices() const
{ return m_attrIndices; }

void ATD::Shader3D::CheckAttributes()
{
	const AttrDesc positionADesc("atrPosition", Gl::FLOAT_VEC3);
	const AttrDesc texCoordsADesc("atrTexCoord", Gl::FLOAT_VEC2);
	const AttrDesc normalADesc("atrNormal", Gl::FLOAT_VEC3);
	const AttrDesc colorADesc("atrColor", Gl::FLOAT_VEC4);

	AttrIndexMap attrIndexMap;
	FillAttrIndexMap(*this, attrIndexMap);

	/* Check attributes index map: */
	{
		auto positionAIter = attrIndexMap.find(positionADesc);
		if (positionAIter != attrIndexMap.end()) {
			m_attrIndices.positionIndex = positionAIter->second;
			attrIndexMap.erase(positionAIter);
		} else {
			throw std::runtime_error(Printf(
						"cannot create Shader3D without '%s'", 
						AttrDescToStr(positionADesc).c_str()));
		}
	}
	{
		auto texCoordsAIter = attrIndexMap.find(texCoordsADesc);
		if (texCoordsAIter != attrIndexMap.end()) {
			m_attrIndices.texCoordsIndex = texCoordsAIter->second;
			m_attrIndices.texCoordsAreRequired = true;
			attrIndexMap.erase(texCoordsAIter);
		}
	}
	{
		auto normalAIter = attrIndexMap.find(normalADesc);
		if (normalAIter != attrIndexMap.end()) {
			m_attrIndices.normalIndex = normalAIter->second;
			m_attrIndices.normalIsRequired = true;
			attrIndexMap.erase(normalAIter);
		}
	}
	{
		auto colorAIter = attrIndexMap.find(colorADesc);
		if (colorAIter != attrIndexMap.end()) {
			m_attrIndices.colorIndex = colorAIter->second;
			m_attrIndices.colorIsRequired = true;
			attrIndexMap.erase(colorAIter);
		}
	}
	if (attrIndexMap.size()) {
		auto attrIter = attrIndexMap.begin();
		std::string attrStr = AttrDescToStr(attrIter->first);

		attrIter++;
		for (; attrIter != attrIndexMap.end(); attrIter++) {
			attrStr += std::string("; ") + AttrDescToStr(attrIter->first);
		}
		throw std::runtime_error(Printf(
					"unsupported attributes in Shader3D:\n%s", 
					attrStr.c_str()));
	}
}


