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

// Skia branch: chrome/m132

#ifndef __R2D9_SKIA_SVG_DATA_H__
#define __R2D9_SKIA_SVG_DATA_H__ 1

#include <string.h>

// C / C++ extern
#if !defined(R2D9_C_EXTERN)
#  if defined(__cplusplus)
#    define R2D9_C_EXTERN extern "C"
#  else
#    define R2D9_C_EXTERN extern
#  endif
#endif

// attribute
#if !defined(R2D9_ATTRIB)
#  if defined(__GNUC__) && (__GNUC__ >= 4)
#    define R2D9_ATTRIB __attribute__((visibility("default")))
#    define R2D9_ATTRIB_PRIVATE __attribute__((visibility("hidden")))
#  endif
#endif

// check attrib and define empty if not defined
#if !defined(R2D9_ATTRIB)
#  define R2D9_ATTRIB
#  define R2D9_ATTRIB_PRIVATE
#endif

// check dll api and define empty if not defined
#if !defined(R2D9_DYLIB_API)
#  define R2D9_DYLIB_API
#endif

// combined lib api
#if !defined(R2D9_C_API)
#  define R2D9_C_API(return_type) R2D9_C_EXTERN R2D9_ATTRIB R2D9_DYLIB_API return_type
#endif

#define R2D9SkiaSvgDataColorSpace_sRGB 1

#define R2D9SkiaSvgDataColorType_RGBA_8888 1
#define R2D9SkiaSvgDataColorType_RGB_888 2

struct R2D9SkiaSvgData {
    void * context;                 // in, optional, any user defined, 'pixelsDataAlloc(..., context)'
    const void * svgData;           // in, required, SVG file
    void * pixelsData;              // out
    void * (*pixelsDataAlloc)(unsigned int size, void * context); // in, optional, ::malloc(...)/::free(...) instead
    unsigned int svgDataSize;       // in, required, size of 'svgData' in bytes
    unsigned int pixelsWidth;       // out
    unsigned int pixelsHeight;      // out
    unsigned int pixelsDataSize;    // out
    float svgWidth;                 // out
    float svgHeight;                // out
    float scaleToWidth;             // in, scale method #1, optional, > 0
    float scaleToHeight;            // in, scale method #1, optional, > 0
    float scaleWidth;               // in, scale method #2, optional, > 0, svgWidth * scaleWidth
    float scaleHeight;              // in, scale method #2, optional, > 0, svgHeight * scaleHeight
    unsigned char colorSpace;       // in, required, R2D9SkiaSvgDataColorSpace_xxxxx
    unsigned char colorType;        // in, out, required, R2D9SkiaSvgDataColorType_xxxxx
};

R2D9_C_API(int) R2D9SkiaSvgDataProcess(struct R2D9SkiaSvgData * data);

#endif // !__R2D9_SKIA_SVG_DATA_H__
