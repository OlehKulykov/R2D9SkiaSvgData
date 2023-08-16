// MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <cstddef>

#include "include/core/SkStream.h"
#include "include/core/SkSurface.h"
#include "include/core/SkColorSpace.h"
#include "include/core/SkColorType.h"
#include "include/core/SkAlphaType.h"
#include "include/core/SkPixmap.h"

#include "modules/svg/include/SkSVGDOM.h"
#include "modules/svg/include/SkSVGNode.h"
#include "modules/svg/include/SkSVGSVG.h"

#include "modules/svg/include/r2d9_skia_svg_data.h"

int R2D9SkiaSvgDataProcess(struct R2D9SkiaSvgData * data) {
    if (!data || !data->svgData || (data->svgDataSize == 0)) {
        return __LINE__;
    }
    
    SkMemoryStream inMemStream(data->svgData, data->svgDataSize, false);
    const auto svgDom = SkSVGDOM::MakeFromStream(inMemStream);
    if (!svgDom) {
        return __LINE__;
    }
    
    SkSVGSVG * root = svgDom->getRoot();
    if (!root) {
        return __LINE__;
    }
    
    data->svgWidth = root->getWidth().value();
    data->svgHeight = root->getHeight().value();
    
    SkSize size;
    
    if ((data->scaleToWidth > 0.0f) || (data->scaleToHeight > 0.0f)) {
        const auto srcX = root->getX().value();
        const auto srcY = root->getY().value();
        size = SkSize::Make((data->scaleToWidth > 0.0f) ? data->scaleToWidth : data->svgWidth,
                            (data->scaleToHeight > 0.0f) ? data->scaleToHeight : data->svgHeight);
        root->setTransform(SkSVGTransformType::RectToRect(SkRect::MakeXYWH(srcX, srcY, data->svgWidth, data->svgHeight), SkRect::MakeSize(size)));
    } else if ((data->scaleWidth > 0.0f) || (data->scaleHeight > 0.0f)) {
        const auto srcX = root->getX().value();
        const auto srcY = root->getY().value();
        size = SkSize::Make((data->scaleWidth > 0.0f) ? (data->svgWidth * data->scaleWidth) : data->svgWidth,
                            (data->scaleHeight > 0.0f) ? (data->svgHeight * data->scaleHeight) : data->svgHeight);
        root->setTransform(SkSVGTransformType::RectToRect(SkRect::MakeXYWH(srcX, srcY, data->svgWidth, data->svgHeight), SkRect::MakeSize(size)));
    } else {
        size = svgDom->containerSize();
    }
    
    if ((size.width() <= 0.0f) || (size.height() <= 0.0f)) {
        return __LINE__;
    }
    
    sk_sp<SkColorSpace> colorSpace;
    switch (data->colorSpace) {
        case R2D9SkiaSvgDataColorSpace_sRGB:
            colorSpace = SkColorSpace::MakeSRGB();
            break;
        default:
            return __LINE__;
    }
    SkColorType colorType;
    SkAlphaType alphaType;
    switch (data->colorType) {
        case R2D9SkiaSvgDataColorType_RGBA_8888:
            colorType = kRGBA_8888_SkColorType;
            alphaType = kPremul_SkAlphaType;
            break;
        case R2D9SkiaSvgDataColorType_RGB_888:
            colorType = kRGB_888x_SkColorType;
            alphaType = kOpaque_SkAlphaType;
            break;
        default:
            return __LINE__;
    }
    const auto surface = SkSurface::MakeRaster(SkImageInfo::Make(size.width(), size.height(), colorType, alphaType, colorSpace));
    auto * canvas = surface ? surface->getCanvas() : nullptr;
    if (!canvas) {
        return __LINE__;
    }
    svgDom->render(canvas);
    
    SkPixmap pixmap;
    if (!surface->peekPixels(&pixmap)) {
        return __LINE__;
    }
    
    if ((pixmap.width() < 0) || (pixmap.height() < 0)) {
        return __LINE__;
    }
    
    data->pixelsWidth = pixmap.width();
    data->pixelsHeight = pixmap.height();
    
    if (pixmap.isOpaque() || (data->colorType == R2D9SkiaSvgDataColorType_RGB_888)) {
        data->colorType = R2D9SkiaSvgDataColorType_RGB_888;
        const size_t pixelsCount = data->pixelsWidth * data->pixelsHeight;
        const size_t byteSize = pixelsCount * 3; // ...RGB_888
        if (byteSize > UINT_MAX) {
            return __LINE__;
        }
        if (data->pixelsDataAlloc) {
            data->pixelsData = data->pixelsDataAlloc(static_cast<unsigned int>(byteSize), data->context);
        } else {
            data->pixelsData = ::malloc(byteSize);
        }
        if (!data->pixelsData) {
            return __LINE__;
        }
        data->pixelsDataSize = static_cast<unsigned int>(byteSize);
        uint8_t * pixelsData = static_cast<uint8_t *>(data->pixelsData);
        const uint8_t * pixmapAddr = static_cast<const uint8_t *>(pixmap.addr());
        for (size_t i = 0; i < pixelsCount; i++) {
            *pixelsData++ = *pixmapAddr++;
            *pixelsData++ = *pixmapAddr++;
            *pixelsData++ = *pixmapAddr++;
            pixmapAddr++;
        }
    } else {
        const size_t byteSize = pixmap.computeByteSize();
        if (byteSize > UINT_MAX) {
            return __LINE__;
        }
        if (data->pixelsDataAlloc) {
            data->pixelsData = data->pixelsDataAlloc(static_cast<unsigned int>(byteSize), data->context);
        } else {
            data->pixelsData = ::malloc(byteSize);
        }
        if (!data->pixelsData) {
            return __LINE__;
        }
        data->pixelsDataSize = static_cast<unsigned int>(byteSize);
        ::memcpy(data->pixelsData, pixmap.addr(), byteSize);
    }
    
    return 0;
}
