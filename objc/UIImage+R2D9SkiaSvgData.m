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


#import "UIImage+R2D9SkiaSvgData.h"

#include <limits.h>
#include <stdint.h>
#include <float.h>

#if !defined(__has_include)
#  define __has_include 0
#endif

#if __has_include(<r2d9_skia_svg_data.h>)
#  import <r2d9_skia_svg_data.h>
#endif

#if (defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) || (defined(TARGET_OS_IOS) && TARGET_OS_IOS) || (defined(TARGET_OS_SIMULATOR) && TARGET_OS_SIMULATOR)
#  if __has_include(<r2d9_ios_skia_svg/r2d9_skia_svg_data.h>)
#    import <r2d9_ios_skia_svg/r2d9_skia_svg_data.h>
#  endif
#endif

#if (defined(TARGET_OS_MAC) && TARGET_OS_MAC) || (defined(TARGET_OS_OSX) && TARGET_OS_OSX)
#  if __has_include(<r2d9_macos_skia_svg/r2d9_skia_svg_data.h>)
#    import <r2d9_macos_skia_svg/r2d9_skia_svg_data.h>
#  endif
#endif

#if !defined(__R2D9_SKIA_SVG_DATA_H__)
#  error "R2D9 skia Svg data framework not found."
#endif

static void * R2D9SkiaSvgDataAllocDataCb(unsigned int size, void * context) {
    return malloc(size);
}

static void R2D9SkiaSvgDataFreeDataCb(void * info, const void * data, size_t size) {
    if (data) {
        free((void *)data);
    }
}

static NSInteger R2D9SkiaSvgDataBaseInit(struct R2D9SkiaSvgData * data,
                                         NSData * _Nonnull svgData,
                                         const CGImageAlphaInfo alphaInfo) {
    switch (alphaInfo) {
        case kCGImageAlphaNone: 
            data->colorType = R2D9SkiaSvgDataColorType_RGB_888;
            break;
            
        case kCGImageAlphaLast: 
            data->colorType = R2D9SkiaSvgDataColorType_RGBA_8888;
            break;
            
        default: 
            return 1;
    }
    
    data->svgData = svgData.bytes;
    const NSUInteger svgDataSize = svgData.length;
#if defined(UINT_MAX)
    if (svgDataSize > UINT_MAX) {
        return 2;
    }
#endif
    data->svgDataSize = (unsigned int)svgDataSize;
    data->colorSpace = R2D9SkiaSvgDataColorSpace_sRGB;
    data->pixelsDataAlloc = &R2D9SkiaSvgDataAllocDataCb;
    
    return 0;
}

static UIImage * R2D9SkiaSvgDataCreateImage(struct R2D9SkiaSvgData * data) {
    if (!data->pixelsData) {
        return nil;
    }
    
    if (data->pixelsDataSize == 0) {
        free(data->pixelsData);
        return nil;
    }
    
    size_t componentsCount = 0;
    CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault;
    
    switch (data->colorType) {
        case R2D9SkiaSvgDataColorType_RGB_888: 
            componentsCount = 3;
            break;
            
        case R2D9SkiaSvgDataColorType_RGBA_8888: 
            componentsCount = 4;
            bitmapInfo |= kCGImageAlphaLast;
            break;
            
        default:
            free(data->pixelsData);
            return nil;
    }
    
    const size_t bitsPerComponent = 8;
    const size_t bitsPerPixel = componentsCount * bitsPerComponent;
    const size_t bytesPerRow = componentsCount * (size_t)data->pixelsWidth;
    
    CGDataProviderRef dataProvider = CGDataProviderCreateWithData(NULL, data->pixelsData, data->pixelsDataSize, &R2D9SkiaSvgDataFreeDataCb);
    CGColorSpaceRef colorSpace = dataProvider ? CGColorSpaceCreateDeviceRGB() : NULL;
    CGImageRef cgImage = colorSpace ? CGImageCreate(data->pixelsWidth,
                                                    data->pixelsHeight,
                                                    bitsPerComponent,
                                                    bitsPerPixel,
                                                    bytesPerRow,
                                                    colorSpace,
                                                    bitmapInfo,
                                                    dataProvider,
                                                    NULL,
                                                    false,
                                                    kCGRenderingIntentDefault) : NULL;
    UIImage * uiImage = cgImage ? [UIImage imageWithCGImage:cgImage] : nil;
    
    if (dataProvider) {
        CGDataProviderRelease(dataProvider);
    } else {
        free(data->pixelsData);
    }
    if (colorSpace) {
        CGColorSpaceRelease(colorSpace);
    }
    if (cgImage) {
        CGImageRelease(cgImage);
    }
    
    return uiImage;
}

@implementation UIImage(R2D9SkiaSvgData)

+ (nullable UIImage *) imageWithSvgData:(nonnull NSData *) svgData 
                           andAlphaInfo:(const CGImageAlphaInfo) alphaInfo {
    if (!svgData) {
        return nil;
    }
    
    struct R2D9SkiaSvgData data = { 0 };
    
    if (R2D9SkiaSvgDataBaseInit(&data, svgData, alphaInfo) != 0) {
        return nil;
    }
    
    if (R2D9SkiaSvgDataProcess(&data) != 0) {
        return nil;
    }
    
    return R2D9SkiaSvgDataCreateImage(&data);
}

+ (nullable UIImage *) imageWithSvgData:(nonnull NSData *) svgData
                              alphaInfo:(const CGImageAlphaInfo) alphaInfo
                           scaledToSize:(const CGSize) newSize
                        andOriginalSize:(nullable CGSize *) originalSize {
    if (!svgData) {
        return nil;
    }
#if defined(FLT_MIN)
    if ((newSize.width < FLT_MIN) || (newSize.height < FLT_MIN)) {
        return nil;
    }
#endif
#if defined(FLT_MAX)
    if ((newSize.width > FLT_MAX) || (newSize.height > FLT_MAX)) {
        return nil;
    }
#endif
    
    struct R2D9SkiaSvgData data = { 0 };
    
    if (R2D9SkiaSvgDataBaseInit(&data, svgData, alphaInfo) != 0) {
        return nil;
    }
    
    data.scaleToWidth = (float)newSize.width;
    data.scaleToHeight = (float)newSize.height;
    
    if (R2D9SkiaSvgDataProcess(&data) != 0) {
        return nil;
    }
    
    if (originalSize) {
        *originalSize = CGSizeMake(data.svgWidth, data.svgHeight);
    }
    
    return R2D9SkiaSvgDataCreateImage(&data);
}

+ (nullable UIImage *) imageWithSvgData:(nonnull NSData *) svgData
                              alphaInfo:(const CGImageAlphaInfo) alphaInfo
                                  scale:(const CGSize) scale
                        andOriginalSize:(nullable CGSize *) originalSize {
    if (!svgData) {
        return nil;
    }
#if defined(FLT_MIN)
    if ((scale.width < FLT_MIN) || (scale.height < FLT_MIN)) {
        return nil;
    }
#endif
#if defined(FLT_MAX)
    if ((scale.width > FLT_MAX) || (scale.height > FLT_MAX)) {
        return nil;
    }
#endif
    
    struct R2D9SkiaSvgData data = { 0 };
    
    if (R2D9SkiaSvgDataBaseInit(&data, svgData, alphaInfo) != 0) {
        return nil;
    }
    
    data.scaleWidth = (float)scale.width;
    data.scaleHeight = (float)scale.height;
    
    if (R2D9SkiaSvgDataProcess(&data) != 0) {
        return nil;
    }
    
    if (originalSize) {
        *originalSize = CGSizeMake(data.svgWidth, data.svgHeight);
    }
    
    return R2D9SkiaSvgDataCreateImage(&data);
}

@end
