/**
 * @file      
 * @brief     Template texturing functions.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <ATD/Core/Rectangle.hpp>
#include <ATD/Core/Transform2D.hpp>

#include <functional>
#include <vector>


namespace ATD {

namespace ImageImpl {

/**
 * @brief ...
 * @param dstSize  - size of modified texture
 * @param dstData  - data, belonging to modified texture
 * @param position - ...
 * @param value    - cell value to be drawn
 * @return ... */
template<typename T>
void drawCell(const Vector2S &dstSize, 
		T *dstData, 
		const Vector2L &position, 
		const T &value)
{
	if (RectL(dstSize).contains(position)) {
		dstData[position.y * dstSize.x + position.x] = value;
	}
}

/**
 * @brief ...
 * @param dstSize  - size of modified texture
 * @param dstData  - data, belonging to modified texture
 * @param position - ...
 * @param srcSize  - size of texture to be drawn
 * @param srcData  - data, belonging to texture to be drawn
 * @param mixer    - custom cell mixer */
template<typename T>
void drawImage(const Vector2S &dstSize, 
		T *dstData, 
		const Vector2L &position, 
		const Vector2S &srcSize, 
		const T *srcData, 
		std::function<T(const T &, const T&)> mixer)
{
	RectL dstBounds(RectL(position, srcSize).clamped(RectL(dstSize)));
	long srcX = (position.x > 0) ? 0 : -position.x;
	long srcY = (position.y > 0) ? 0 : -position.y;

	for (long iX = 0; iX < dstBounds.w; iX++) {
		for (long iY = 0; iY < dstBounds.h; iY++) {
			size_t iSrc = static_cast<size_t>(
					(srcY + iY) * srcSize.x + (srcX + iX));
			size_t iDst = static_cast<size_t>(
					(dstBounds.y + iY) * dstSize.x + (dstBounds.x + iX));

			dstData[iDst] = mixer(dstData[iDst], srcData[iSrc]);
		}
	}
}

/**
 * @brief ...
 * @param dstSize  - size of modified texture
 * @param dstData  - data, belonging to modified texture
 * @param position - ...
 * @param srcSize  - size of texture to be drawn
 * @param srcData  - data, belonging to texture to be drawn
 * @param bounds   - source texture patch bounds
 * @param repeat   - whether source texture shall repeat within the bounds
 * @param mixer    - custom cell mixer */
template<typename T>
void drawBounded(const Vector2S &dstSize, 
		T *dstData, 
		const Vector2L &position, 
		const Vector2S &srcSize, 
		const T *srcData, 
		const RectL &bounds, 
		bool repeat, 
		std::function<T(const T &, const T&)> mixer)
{
	RectL dstBounds(RectL(position, bounds.size()).clamped(RectL(dstSize)));
	long srcX = ((position.x > 0) ? 0 : -position.x) + bounds.pos().x;
	long srcY = ((position.y > 0) ? 0 : -position.y) + bounds.pos().y;

	if (repeat) {
		if (srcSize.x * srcSize.y) {
			srcX %= srcSize.x;
			srcX += srcSize.x;

			srcY %= srcSize.y;
			srcY += srcSize.y;

			for (long iX = 0; iX < dstBounds.w; iX++) {
				for (long iY = 0; iY < dstBounds.h; iY++) {
					size_t iSrc = static_cast<size_t>(
							((srcY + iY) % srcSize.y) * srcSize.x + 
							((srcX + iX) % srcSize.x));
					size_t iDst = static_cast<size_t>(
							(dstBounds.y + iY) * dstSize.x + 
							(dstBounds.x + iX));

					dstData[iDst] = mixer(dstData[iDst], srcData[iSrc]);
				}
			}
		}
	} else {
		RectL srcBounds(
				RectL(Vector2L(srcX, srcY), dstBounds.size()).clamped(
					RectL(srcSize)));

		for (long iX = 0; iX < srcBounds.w; iX++) {
			for (long iY = 0; iY < srcBounds.h; iY++) {
				size_t iSrc = static_cast<size_t>(
						(srcBounds.y + iY) * srcSize.x + (srcBounds.x + iX));
				size_t iDst = static_cast<size_t>(
						(dstBounds.y + iY) * dstSize.x + (dstBounds.x + iX));

				dstData[iDst] = mixer(dstData[iDst], srcData[iSrc]);
			}
		}
	}
}

/**
 * @brief ...
 * @param dstSize   - size of modified texture
 * @param dstData   - data, belonging to modified texture
 * @param position  - position of center on canvas
 * @param transform - ...
 * @param srcSize   - size of texture to be drawn
 * @param srcData   - data, belonging to texture to be drawn
 * @param bounds    - source texture patch bounds
 * @param center    - position of center on initial rectangle
 * @param repeat    - whether source texture shall repeat within the bounds
 * @param mixer     - custom cell mixer
 * @param cellRatio - cell width / cell height */
template<typename T>
void drawTransformed(const Vector2S &dstSize, 
		T *dstData, 
		const Vector2D &position, 
		const Transform2D &transform, 
		const Vector2S &srcSize, 
		const T *srcData, 
		const RectL &bounds, 
		const Vector2D &center, 
		bool repeat, 
		std::function<T(const T &, const T&)> mixer, 
		double cellRatio = 1.)
{
	Transform2D transformMain(transform); /* Bounds to canvas. */
	transformMain.setOffset(position - (Vector2D(bounds.pos()) + center));

	Transform2D transformCell(Vector2D(cellRatio, 1.));

	std::vector<Vector2L> srcVertices = bounds.vertices();
	std::vector<Vector2L> dstVertices;
	for (auto &v : srcVertices) {
		dstVertices.push_back(static_cast<Vector2L>(
					transformCell.applyReverse(
						transformMain.apply(
							transformCell.apply(static_cast<Vector2D>(v))))));
	}
	RectL dstBounds(RectL(dstVertices).clamped(RectL(dstSize)));

	if (repeat) {
		if (srcSize.x * srcSize.y) {
			for (long iX = 0; iX < dstBounds.w; iX++) {
				for (long iY = 0; iY < dstBounds.h; iY++) {
					Vector2L dst = dstBounds.pos() + Vector2L(iX, iY);
					Vector2L src = 
						static_cast<Vector2L>(
								transformCell.apply(
									transformMain.applyReverse(
										transformCell.applyReverse(
											static_cast<Vector2D>(dst)))));

					if (bounds.contains(src)) {
						src.x %= srcSize.x;
						src.x += src.x < 0 ? srcSize.x : 0;

						src.y %= srcSize.y;
						src.y += src.y < 0 ? srcSize.y : 0;

						size_t iSrc = static_cast<size_t>(
								src.y * srcSize.x + src.x);
						size_t iDst = static_cast<size_t>(
								dst.y * dstSize.x + dst.x);

						dstData[iDst] = mixer(dstData[iDst], srcData[iSrc]);
					}
				}
			}
		}
	} else {
		RectL srcBounds(bounds.clamped(RectL(srcSize)));

		for (long iX = 0; iX < dstBounds.w; iX++) {
			for (long iY = 0; iY < dstBounds.h; iY++) {
				Vector2L dst = dstBounds.pos() + Vector2L(iX, iY);
				Vector2L src = 
					static_cast<Vector2L>(
							transformCell.apply(
								transformMain.applyReverse(
									transformCell.applyReverse(
										static_cast<Vector2D>(dst)))));

				if (srcBounds.contains(src)) {
					size_t iSrc = static_cast<size_t>(
							src.y * srcSize.x + src.x);
					size_t iDst = static_cast<size_t>(
							dst.y * dstSize.x + dst.x);

					dstData[iDst] = mixer(dstData[iDst], srcData[iSrc]);
				}
			}
		}
	}
}

/* FIXME: Add drawTriangle(). */

} /* namespace ImageImpl */

} /* namespace ATD */


