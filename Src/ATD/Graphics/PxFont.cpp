/**
 * @file      
 * @brief     PxFont (for pixel fonts) class implementation
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#include <ATD/Graphics/PxFont.hpp>

#include <ATD/Core/Printf.hpp>

#include <nlohmann/json.hpp>

#include <fstream>
#include <stdexcept>


/* ATD::PxFont::Glyph: */

ATD::PxFont::Glyph::Glyph()
	: m_textureRect()
	, m_joint()
{}

ATD::PxFont::Glyph::Glyph(const ATD::RectL &textureRect, 
		const ATD::Vector2L &joint)
	: m_textureRect(textureRect)
	, m_joint(joint)
{}

ATD::RectL ATD::PxFont::Glyph::GetTextureRect() const
{ return m_textureRect; }

ATD::Vector2L ATD::PxFont::Glyph::GetJointRight() const
{ return m_joint; }

ATD::Vector2L ATD::PxFont::Glyph::GetJointLeft() const
{ return Vector2L(0, m_joint.y); }


/* ATD::PxFont: */

ATD::PxFont::PxFont()
	: Loadable()
	, m_dftGlyph(RectL(Vector2L(1, 1)), Vector2L(1, 1))
	, m_glyphs()
	, m_texturePtr(nullptr)
	, m_imagePtr(new Image(Vector2S(1, 1)))
{
	Loadable::AddDependency(static_cast<Loadable *>(m_imagePtr.get()));

	UpdateTextureFromImage();

	/* Now the font texture is a black square 1x1, which does not make sence, 
	 * but leaves the PxFont in consistent state, so there will not happen 
	 * segmentation fault. */
}

ATD::Texture::CPtr ATD::PxFont::GetTexturePtr() const
{
	return static_cast<Texture::CPtr>(m_texturePtr);
}

const ATD::PxFont::Glyph &ATD::PxFont::GetGlyph(
		const ATD::Unicode::Glyph &glyph) const
{
	auto gIter = m_glyphs.find(glyph);
	if (gIter != m_glyphs.end()) {
		return gIter->second;
	} else {
		return m_dftGlyph;
	}
}

void ATD::PxFont::OnLoad(const ATD::Fs::Path &filename)
{
	/* Open .json file, containing PxFont data. */
	std::ifstream file(filename.Native());
	if (!file.is_open()) {
		throw std::runtime_error(
				Printf(
					"Failed to open %s for reading as PxFont", 
					filename.Native().c_str()));
	}

	/* Obtain the size of the file. */
	file.seekg(0, std::ios_base::end);
	size_t fileSize = static_cast<size_t>(file.tellg());
	file.seekg(0, std::ios_base::beg);

	/* Read the whole .json file into jsonBuf. */
	std::string jDataBuf(fileSize, '\0');
	file.read(&jDataBuf[0], jDataBuf.size());
	file.close();

	auto jData = nlohmann::json::parse(&jDataBuf[0]);

	/* Order is optional (and is not used in PxFont now). */
	/*
	if (json.find("order") != json.end()) {
		m_order = 
			(json["order"].get<std::string>() == "BWD") ? 
			Order::BWD : Order::FWD;
	}
	*/

	/* Get image filename. */
	auto jTexture = jData["texture"];
	Fs::Path imageFilenameRelative(jTexture.get<std::string>());

	Fs::Path imageFilename = 
		filename.ParentDir().Joined(imageFilenameRelative);

	Loadable::SetDependencyPath(static_cast<Loadable *>(m_imagePtr.get()), 
			imageFilename);

	/* Get default glyph. */
	auto jDefault = jData["default"];
	m_dftGlyph = Glyph(
			RectL(
				jDefault["x"].get<long>(), 
				jDefault["y"].get<long>(), 
				jDefault["w"].get<long>(), 
				jDefault["h"].get<long>()), 
			Vector2L(
				jDefault["jX"].get<long>(), 
				jDefault["jY"].get<long>()));

	/* Get glyphs. */
	auto jGlyphs = jData["glyphs"];
	for (size_t gIndex = 0; gIndex < jGlyphs.size(); gIndex++) {
		/* Get Glyph key. */
		const std::string keyUtf = jGlyphs[gIndex]["key"].get<std::string>();
		const Unicode::Glyph key = Unicode(keyUtf).Front();

		/* Get Glyph value */
		auto jValue = jGlyphs[gIndex]["value"];
		{
			RectL textureRect = m_dftGlyph.GetTextureRect();
			{
				textureRect.x = jValue["x"].get<long>();
				textureRect.y = jValue["y"].get<long>();

				auto jvWIter = jValue.find("w");
				auto jvHIter = jValue.find("h");

				if (jvWIter != jValue.end()) {
					textureRect.w = jvWIter->get<long>();
				}
				if (jvHIter != jValue.end()) {
					textureRect.h = jvHIter->get<long>();
				}
			}

			Vector2L joint = m_dftGlyph.GetJointRight();
			{
				auto jvJXIter = jValue.find("jX");
				auto jvJYIter = jValue.find("jY");

				if (jvJXIter != jValue.end()) {
					joint.x = jvJXIter->get<long>();
				}
				if (jvJYIter != jValue.end()) {
					joint.y = jvJYIter->get<long>();
				}
			}

			Glyph newGlyph(textureRect, joint);
			m_glyphs.insert(std::pair<Unicode::Glyph, Glyph>(key, newGlyph));
		}
	}
}

void ATD::PxFont::OnLoadFinished()
{
	UpdateTextureFromImage();
}

void ATD::PxFont::OnSave(const ATD::Fs::Path &filename) const
{
	nlohmann::json jData;

	/* Set order. */
	/* FIXME: Do I ever need 'order' value? */
	jData["order"] = std::string("FWD");

	/* Set image filename. */
	{
		Fs::Path fileParentDir = filename.ParentDir();

		Fs::Path imageFilename = 
			Loadable::GetDependencyPath(
					static_cast<Loadable *>(
						m_imagePtr.get())).Relative(fileParentDir);
		jData["texture"] = imageFilename.Common();
	}

	/* Set default glyph. */
	jData["default"] = {
		{"x", m_dftGlyph.GetTextureRect().x}, 
		{"y", m_dftGlyph.GetTextureRect().y}, 
		{"w", m_dftGlyph.GetTextureRect().w}, 
		{"h", m_dftGlyph.GetTextureRect().h}, 
	
		{"jX", m_dftGlyph.GetJointRight().x}, 
		{"jY", m_dftGlyph.GetJointRight().y}
	};

	/* Set glyphs. */
	std::vector<nlohmann::json> jGlyphs;
	for (auto glyphPair : m_glyphs) {
		nlohmann::json jGlyph;

		std::string glyphKey = Unicode::StrFromGlyph(glyphPair.first);

		nlohmann::json jGlyphValue;
		{
			jGlyphValue["x"] = glyphPair.second.GetTextureRect().x;
			jGlyphValue["y"] = glyphPair.second.GetTextureRect().y;

			if (Vector2L(
						glyphPair.second.GetTextureRect().w, 
						glyphPair.second.GetTextureRect().h) != 
					Vector2L(
						m_dftGlyph.GetTextureRect().w, 
						m_dftGlyph.GetTextureRect().h)) {

				jGlyphValue["w"] = glyphPair.second.GetTextureRect().w;
				jGlyphValue["h"] = glyphPair.second.GetTextureRect().h;
			}

			if (glyphPair.second.GetJointRight() != 
					m_dftGlyph.GetJointRight()) {

				jGlyphValue["jX"] = glyphPair.second.GetJointRight().x;
				jGlyphValue["jY"] = glyphPair.second.GetJointRight().y;
			}
		}

		jGlyph["key"] = glyphKey;
		jGlyph["value"] = jGlyphValue;

		jGlyphs.push_back(jGlyph);
	}

	std::string jDataBuf = jData.dump();

	/* Rewrite the json file, containing PxFont data. */
	std::ofstream file(filename.Native());
	if (!file.is_open()) {
		throw std::runtime_error(
				Printf(
					"Failed to open %s for writing as PxFont", 
					filename.Native().c_str()));
	}

	file.write(&jDataBuf[0], jDataBuf.size());
	file.close();
}

void ATD::PxFont::UpdateTextureFromImage()
{
	if (m_imagePtr) {
		m_texturePtr = Texture::Ptr(new Texture(*m_imagePtr));
	}
}


