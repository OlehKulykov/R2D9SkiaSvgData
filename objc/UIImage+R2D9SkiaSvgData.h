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

// -----------------------------------------------------------------------------
// Usage with Objective-C or Swift.
//   1. Add this header{.h} and source{.m} to the project.
//   2. Get latest framework from GitHub's releases and add/link to the project.
//     2.1. Optionaly link with libexpat{.tbd} and libc++{.tbd} in Build Phases.
//   3. Use.
// -----------------------------------------------------------------------------

#import <UIKit/UIKit.h>

/// Parameters:
///   svgData: Input SVG file data.
///   alphaInfo: Type of alpha chanel of result image.
///     - kCGImageAlphaNone - RGB 888
///     - kCGImageAlphaLast - RGBA 8888
///   originalSize: Optional original out SVG image size.

@interface UIImage(R2D9SkiaSvgData)

+ (nullable UIImage *) imageWithSvgData:(nonnull NSData *) svgData 
                           andAlphaInfo:(const CGImageAlphaInfo) alphaInfo;

+ (nullable UIImage *) imageWithSvgData:(nonnull NSData *) svgData
                              alphaInfo:(const CGImageAlphaInfo) alphaInfo
                           scaledToSize:(const CGSize) newSize
                        andOriginalSize:(nullable CGSize *) originalSize;

+ (nullable UIImage *) imageWithSvgData:(nonnull NSData *) svgData
                              alphaInfo:(const CGImageAlphaInfo) alphaInfo
                                  scale:(const CGSize) scale
                        andOriginalSize:(nullable CGSize *) originalSize;

@end
