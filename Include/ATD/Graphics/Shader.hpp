/**
 * @file      
 * @brief     Wrap around OpenGL shader.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <ATD/Core/Matrix3.hpp>
#include <ATD/Core/Matrix4.hpp>
#include <ATD/Core/Vector2.hpp>
#include <ATD/Core/Vector3.hpp>
#include <ATD/Core/Vector4.hpp>
#include <ATD/Graphics/Gl.hpp>
#include <ATD/Graphics/Texture.hpp>
#include <ATD/Graphics/VertexBuffer2D.hpp>
#include <ATD/Graphics/VertexBuffer3D.hpp>

#include <map>
#include <memory>
#include <string>


namespace ATD {

/**
 * @brief ...
 * @class ... */
class Shader
{
public:
	typedef std::shared_ptr<Shader> Ptr;
	typedef std::shared_ptr<const Shader> CPtr;

	/**
	 * @brief Shader use automation.
	 * @class ...
	 *
	 * Create an instance of this class to use the shader while drawing. */
	class Usage
	{
	public:
		/**
		 * @brief ...
		 * @param shader - shader to be used */
		Usage(const Shader &shader);

		/**
		 * @brief ... */
		~Usage();
	private:
		Gl::Uint m_prevProgram;
		bool m_activated;
	};

	/* Plain 3D: */

	/* Light 3D: */


	/**
	 * @brief ...
	 * @param vertexSource   - ...
	 * @param fragmentSource - ... */
	Shader(const std::string &vertexSource, 
			const std::string &fragmentSource);

	/**
	 * @brief ... */
	~Shader();

	/**
	 * @brief ...
	 * @param name - ...
	 * @param value - ... */
	void setUniform(const std::string &name, float value);

	/**
	 * @brief ...
	 * @param name - ...
	 * @param value - ... */
	void setUniform(const std::string &name, int value);

	/**
	 * @brief ...
	 * @param name - ...
	 * @param value - ... */
	void setUniform(const std::string &name, const Vector2F &value);

	/**
	 * @brief ...
	 * @param name - ...
	 * @param value - ... */
	void setUniform(const std::string &name, const Vector2I &value);

	/**
	 * @brief ...
	 * @param name - ...
	 * @param value - ... */
	void setUniform(const std::string &name, const Vector3F &value);

	/**
	 * @brief ...
	 * @param name - ...
	 * @param value - ... */
	void setUniform(const std::string &name, const Vector3I &value);

	/**
	 * @brief ...
	 * @param name - ...
	 * @param value - ... */
	void setUniform(const std::string &name, const Vector4F &value);

	/**
	 * @brief ...
	 * @param name - ...
	 * @param value - ... */
	void setUniform(const std::string &name, const Vector4I &value);

	/**
	 * @brief ...
	 * @param name - ...
	 * @param value - ... */
	void setUniform(const std::string &name, const Matrix3F &value);

	/**
	 * @brief ...
	 * @param name - ...
	 * @param value - ... */
	void setUniform(const std::string &name, const Matrix4F &value);

	/**
	 * @brief Enables given texture unit for uniform sampler 'name'
	 * @param name - ...
	 * @param unit - ... */
	void setUniformSampler2DUnit(const std::string &name, 
			const Texture::Unit &unit);

	/**
	 * @brief Enables given texture unit for uniform sampler 'name'
	 * @param name - ...
	 * @param unit - ... */
	void setUniformSamplerCubeUnit(const std::string &name, 
			const Texture::Unit &unit);

	// TODO: size_t getUniformArraySize(const std::string &name);

	/**
	 * @brief ... */
	void checkIfUniformsSet() const;

	/* TODO: checkIfSamplersSet(); */

	/**
	 * @brief ...
	 * @return ... */
	inline Gl::Uint glId() const
	{ return m_program; }

	/**
	 * @brief ...
	 * @return ... */
	inline bool isCanvasRequired() const
	{ return m_isCanvasRequired; }

	// TODO: Debug functions for listing all unoptimized uniforms,
	// TODO: Debug functions for listing all unoptimized attributes.

private:
	struct UniformLocationDesc
	{
		inline UniformLocationDesc(const Gl::Int &n_location, 
				const Gl::Enum &n_type)
			: location(n_location), type(n_type), isSet(false)
		{}

		Gl::Int location;
		Gl::Enum type;
		bool isSet;
	};

	typedef std::map<std::string, UniformLocationDesc> UniformLocationMap;


	/**
	 * @brief ...
	 * @param shaderSource - ...
	 * @param shaderType   - ...
	 * @param shaderId     - ... */
	void compileShader(const std::string &shaderSource, 
			Gl::Enum shaderType, 
			Gl::Uint &shaderId);

	/**
	 * @brief ... */
	void initUniforms();


	Gl::Uint m_program;
	Gl::Uint m_vertexShaderId;
	Gl::Uint m_fragmentShaderId;
	UniformLocationMap m_uniformLocationMap;
	bool m_isCanvasRequired;
};

/**
 * @brief shader for drawing 2D vertices
 * @class ... */
class Shader2D : public Shader
{
public:
	/* Does not need own Usage subclass, Shader::Usage will work fine. */

	typedef std::shared_ptr<Shader2D> Ptr;
	typedef std::shared_ptr<const Shader2D> CPtr;

	/* Basic 2D shader for drawing texture2D. */
	static const std::string PLAIN_VERTEX_SOURCE;
	static const std::string PLAIN_FRAGMENT_SOURCE;

	/* Simple 2D shader for drawing textures with transparency. */
	static const std::string ALPHA_VERTEX_SOURCE;
	static const std::string ALPHA_FRAGMENT_SOURCE;

	/* FIXME: add more! */

	/* Default 2D shader. */
	static const std::string DFT_VERTEX_SOURCE;
	static const std::string DFT_FRAGMENT_SOURCE;

	/**
	 * @brief ...
	 * @param vertexSource   - ...
	 * @param fragmentSource - ... */
	Shader2D(const std::string &vertexSource = DFT_VERTEX_SOURCE, 
			const std::string &fragmentSource = DFT_FRAGMENT_SOURCE);

	/**
	 * @brief ...
	 * @return ... */
	const VertexBuffer2D::AttrIndices &getAttrIndices() const;

private:
	/**
	 * @brief ...
	 * @throws ... */
	void checkAttributes();

	// TODO: CheckUniforms() ? (unfProject and unfTransform)


	VertexBuffer2D::AttrIndices m_attrIndices;
};

/**
 * @brief shader for drawing 3D vertices
 * @class ... */
class Shader3D : public Shader
{
public:
	/* Does not need own Usage subclass, Shader::Usage will work fine. */

	typedef std::shared_ptr<Shader3D> Ptr;
	typedef std::shared_ptr<const Shader3D> CPtr;

	/* Basic Shader3D for drawing textured model without normals. */
	static const std::string PLAIN_VERTEX_SOURCE;
	static const std::string PLAIN_FRAGMENT_SOURCE;

	/* Shader for drawing illuminated model with normals. */
	static const std::string LIGHT_VERTEX_SOURCE;

	static const std::string LIGHT_FRAGMENT_SOURCE_TEMPLATE;
	static const unsigned LIGHT_DFT_MAX_DIR_LIGHTS;
	static const unsigned LIGHT_DFT_MAX_POINT_LIGHTS;
	static const unsigned LIGHT_DFT_MAX_SPOT_LIGHTS;
	static const std::string LIGHT_FRAGMENT_SOURCE;

	/* Default 3D shader: */
	static const std::string DFT_VERTEX_SOURCE;
	static const std::string DFT_FRAGMENT_SOURCE;

	/* FIXME: Add more! */


	/**
	 * @brief ...
	 * @param vertexSource   - ...
	 * @param fragmentSource - ... */
	Shader3D(const std::string &vertexSource = DFT_VERTEX_SOURCE, 
			const std::string &fragmentSource = DFT_FRAGMENT_SOURCE);

	/**
	 * @brief ...
	 * @return ... */
	const VertexBuffer3D::AttrIndices &getAttrIndices() const;

private:
	/**
	 * @brief ...
	 * @throws ... */
	void checkAttributes();

	// TODO: CheckUniforms() ? (unfProject and unfTransform)


	VertexBuffer3D::AttrIndices m_attrIndices;
};

} /* namespace ATD */


