# R2D9SkiaSvgData
Render SVG file to RGB(A) buffer using skia library.

#### Info: 
Skia library contains module 'svg', but the 'svg' module classes are not exportable.
So, we'll use reverse method: inject exportable function via this header & source files and use non-exportable 'svg' code with rest rendering code. 

#### Note:
It's recommended to use it as a static lib. Plus, for macOS & iOS users, there are compiled frameworks, attached for a latest release.

### Copy:
- skia/modules/svg/include/r2d9_skia_svg_data.h
- skia/modules/svg/src/r2d9_skia_svg_data.cpp

### Add:
- skia/modules/svg/svg.gni
  - "$_src/r2d9_skia_svg_data.h",
  - "$_src/r2d9_skia_svg_data.cpp",

### Build Skia with extra header & source:
Windows supports "x86" and "x64". Since building is only supported on 64-bit machines, the default will always be "x64".
Mac and desktop Linux supports only "x64". On desktop Linux you might also theoretically try any of the ARM or MIPS architecture strings form the Android section below, but these aren't supported or tested and you will also need a sysroot.
Chrome OS supports "x86" and "x64", but to build a 32-bit binary you will need to use a sysroot on a 64-bit machine.
If you specify an Android build (see below) the default CPU architecture will be "arm". You could try overriding it to "arm64", "x86", "mipsel", or "mips64el" but the GN builds for these aren't regularly tested.
target_os
    - "android"
  - "chromeos"
  - "ios"
  - "linux"
  - "nacl"
  - "mac"
  - "win"

#### MacOS
```bash
bin/gn gen out/static_mac_x64 --args='is_debug=false is_official_build=true paragraph_bench_enabled=false paragraph_gms_enabled=false paragraph_tests_enabled=false skia_canvaskit_enable_alias_font=false skia_canvaskit_enable_canvas_bindings=false skia_canvaskit_enable_debugger=false skia_canvaskit_enable_effects_deserialization=false skia_canvaskit_enable_embedded_font=false skia_canvaskit_enable_font=false skia_canvaskit_enable_matrix_helper=false skia_canvaskit_enable_paragraph=false skia_canvaskit_enable_particles=true skia_canvaskit_enable_pathops=false skia_canvaskit_enable_rt_shader=false skia_canvaskit_enable_skottie=false skia_canvaskit_enable_skp_serialization=true skia_canvaskit_enable_sksl_trace=false skia_enable_discrete_gpu=false skia_enable_gpu=false skia_enable_particles=true skia_enable_pdf=false skia_enable_skgpu_v1=false skia_enable_skottie=false skia_enable_skparagraph=false skia_enable_sktext=false skia_enable_svg=true skia_pdf_subset_harfbuzz=false skia_use_dng_sdk=false skia_use_fonthost_mac=false skia_use_gl=false skia_use_harfbuzz=false skia_use_icu=false skia_use_libgifcodec=false skia_use_libjpeg_turbo_decode=false skia_use_libjpeg_turbo_encode=false skia_use_libpng_decode=false skia_use_libpng_encode=false skia_use_libwebp_decode=false skia_use_libwebp_encode=false skia_use_system_harfbuzz=false skia_use_system_icu=false skia_use_system_libjpeg_turbo=false skia_use_system_libpng=false skia_use_system_libwebp=false skia_use_system_zlib=false skia_use_zlib=false text_tests_enabled=false skia_compile_sksl_tests=false paragraph_tests_enabled=false text_gms_enabled=false skia_use_system_expat=true target_os="mac" target_cpu="x64"'

ninja -C out/static_mac_x64

bin/gn gen out/static_mac_arm64 --args='is_debug=false is_official_build=true paragraph_bench_enabled=false paragraph_gms_enabled=false paragraph_tests_enabled=false skia_canvaskit_enable_alias_font=false skia_canvaskit_enable_canvas_bindings=false skia_canvaskit_enable_debugger=false skia_canvaskit_enable_effects_deserialization=false skia_canvaskit_enable_embedded_font=false skia_canvaskit_enable_font=false skia_canvaskit_enable_matrix_helper=false skia_canvaskit_enable_paragraph=false skia_canvaskit_enable_particles=true skia_canvaskit_enable_pathops=false skia_canvaskit_enable_rt_shader=false skia_canvaskit_enable_skottie=false skia_canvaskit_enable_skp_serialization=true skia_canvaskit_enable_sksl_trace=false skia_enable_discrete_gpu=false skia_enable_gpu=false skia_enable_particles=true skia_enable_pdf=false skia_enable_skgpu_v1=false skia_enable_skottie=false skia_enable_skparagraph=false skia_enable_sktext=false skia_enable_svg=true skia_pdf_subset_harfbuzz=false skia_use_dng_sdk=false skia_use_fonthost_mac=false skia_use_gl=false skia_use_harfbuzz=false skia_use_icu=false skia_use_libgifcodec=false skia_use_libjpeg_turbo_decode=false skia_use_libjpeg_turbo_encode=false skia_use_libpng_decode=false skia_use_libpng_encode=false skia_use_libwebp_decode=false skia_use_libwebp_encode=false skia_use_system_harfbuzz=false skia_use_system_icu=false skia_use_system_libjpeg_turbo=false skia_use_system_libpng=false skia_use_system_libwebp=false skia_use_system_zlib=false skia_use_zlib=false text_tests_enabled=false skia_compile_sksl_tests=false paragraph_tests_enabled=false text_gms_enabled=false skia_use_system_expat=true target_os="mac" target_cpu="arm64"'

ninja -C out/static_mac_arm64
```
```bash
libtool -static out/static_mac_x64/libparticles.a out/static_mac_x64/libpathkit.a out/static_mac_x64/libskia.a out/static_mac_x64/libskresources.a out/static_mac_x64/libskshaper.a out/static_mac_x64/libsvg.a out/static_mac_x64/libwuffs.a -o out/libskia_mac_x64.a

libtool -static out/static_mac_arm64/libparticles.a out/static_mac_arm64/libpathkit.a out/static_mac_arm64/libskia.a out/static_mac_arm64/libskresources.a out/static_mac_arm64/libskshaper.a out/static_mac_arm64/libsvg.a out/static_mac_arm64/libwuffs.a -o out/libskia_mac_arm64.a

lipo -create out/libskia_mac_x64.a out/libskia_mac_arm64.a -o out/libskia_mac.a 

lipo -info out/libskia_mac.a
```
##### Architectures in the fat file: out/libskia_mac.a are: x86_64 arm64
```bash
mkdir -p out/r2d9_macos_skia_svg.framework/Headers

cp modules/svg/include/r2d9_skia_svg_data.h out/r2d9_macos_skia_svg.framework/Headers/r2d9_skia_svg_data.h

cp out/libskia_mac.a out/r2d9_macos_skia_svg.framework/r2d9_macos_skia_svg
```

#### iOS
```bash
bin/gn gen out/static_ios_x64 --args='is_debug=false is_official_build=true paragraph_bench_enabled=false paragraph_gms_enabled=false paragraph_tests_enabled=false skia_canvaskit_enable_alias_font=false skia_canvaskit_enable_canvas_bindings=false skia_canvaskit_enable_debugger=false skia_canvaskit_enable_effects_deserialization=false skia_canvaskit_enable_embedded_font=false skia_canvaskit_enable_font=false skia_canvaskit_enable_matrix_helper=false skia_canvaskit_enable_paragraph=false skia_canvaskit_enable_particles=true skia_canvaskit_enable_pathops=false skia_canvaskit_enable_rt_shader=false skia_canvaskit_enable_skottie=false skia_canvaskit_enable_skp_serialization=true skia_canvaskit_enable_sksl_trace=false skia_enable_discrete_gpu=false skia_enable_gpu=false skia_enable_particles=true skia_enable_pdf=false skia_enable_skgpu_v1=false skia_enable_skottie=false skia_enable_skparagraph=false skia_enable_sktext=false skia_enable_svg=true skia_pdf_subset_harfbuzz=false skia_use_dng_sdk=false skia_use_fonthost_mac=false skia_use_gl=false skia_use_harfbuzz=false skia_use_icu=false skia_use_libgifcodec=false skia_use_libjpeg_turbo_decode=false skia_use_libjpeg_turbo_encode=false skia_use_libpng_decode=false skia_use_libpng_encode=false skia_use_libwebp_decode=false skia_use_libwebp_encode=false skia_use_system_harfbuzz=false skia_use_system_icu=false skia_use_system_libjpeg_turbo=false skia_use_system_libpng=false skia_use_system_libwebp=false skia_use_system_zlib=false skia_use_zlib=false text_tests_enabled=false skia_compile_sksl_tests=false paragraph_tests_enabled=false text_gms_enabled=false skia_use_system_expat=true target_os="ios" target_cpu="x64" xcode_sysroot="/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator.sdk"'

ninja -C out/static_ios_x64

bin/gn gen out/static_ios_arm64 --args='is_debug=false is_official_build=true paragraph_bench_enabled=false paragraph_gms_enabled=false paragraph_tests_enabled=false skia_canvaskit_enable_alias_font=false skia_canvaskit_enable_canvas_bindings=false skia_canvaskit_enable_debugger=false skia_canvaskit_enable_effects_deserialization=false skia_canvaskit_enable_embedded_font=false skia_canvaskit_enable_font=false skia_canvaskit_enable_matrix_helper=false skia_canvaskit_enable_paragraph=false skia_canvaskit_enable_particles=true skia_canvaskit_enable_pathops=false skia_canvaskit_enable_rt_shader=false skia_canvaskit_enable_skottie=false skia_canvaskit_enable_skp_serialization=true skia_canvaskit_enable_sksl_trace=false skia_enable_discrete_gpu=false skia_enable_gpu=false skia_enable_particles=true skia_enable_pdf=false skia_enable_skgpu_v1=false skia_enable_skottie=false skia_enable_skparagraph=false skia_enable_sktext=false skia_enable_svg=true skia_pdf_subset_harfbuzz=false skia_use_dng_sdk=false skia_use_fonthost_mac=false skia_use_gl=false skia_use_harfbuzz=false skia_use_icu=false skia_use_libgifcodec=false skia_use_libjpeg_turbo_decode=false skia_use_libjpeg_turbo_encode=false skia_use_libpng_decode=false skia_use_libpng_encode=false skia_use_libwebp_decode=false skia_use_libwebp_encode=false skia_use_system_harfbuzz=false skia_use_system_icu=false skia_use_system_libjpeg_turbo=false skia_use_system_libpng=false skia_use_system_libwebp=false skia_use_system_zlib=false skia_use_zlib=false text_tests_enabled=false skia_compile_sksl_tests=false paragraph_tests_enabled=false text_gms_enabled=false skia_use_system_expat=true target_os="ios" target_cpu="arm64" xcode_sysroot="/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk"'

ninja -C out/static_ios_arm64

bin/gn gen out/static_ios_armv7 --args='is_debug=false is_official_build=true paragraph_bench_enabled=false paragraph_gms_enabled=false paragraph_tests_enabled=false skia_canvaskit_enable_alias_font=false skia_canvaskit_enable_canvas_bindings=false skia_canvaskit_enable_debugger=false skia_canvaskit_enable_effects_deserialization=false skia_canvaskit_enable_embedded_font=false skia_canvaskit_enable_font=false skia_canvaskit_enable_matrix_helper=false skia_canvaskit_enable_paragraph=false skia_canvaskit_enable_particles=true skia_canvaskit_enable_pathops=false skia_canvaskit_enable_rt_shader=false skia_canvaskit_enable_skottie=false skia_canvaskit_enable_skp_serialization=true skia_canvaskit_enable_sksl_trace=false skia_enable_discrete_gpu=false skia_enable_gpu=false skia_enable_particles=true skia_enable_pdf=false skia_enable_skgpu_v1=false skia_enable_skottie=false skia_enable_skparagraph=false skia_enable_sktext=false skia_enable_svg=true skia_pdf_subset_harfbuzz=false skia_use_dng_sdk=false skia_use_fonthost_mac=false skia_use_gl=false skia_use_harfbuzz=false skia_use_icu=false skia_use_libgifcodec=false skia_use_libjpeg_turbo_decode=false skia_use_libjpeg_turbo_encode=false skia_use_libpng_decode=false skia_use_libpng_encode=false skia_use_libwebp_decode=false skia_use_libwebp_encode=false skia_use_system_harfbuzz=false skia_use_system_icu=false skia_use_system_libjpeg_turbo=false skia_use_system_libpng=false skia_use_system_libwebp=false skia_use_system_zlib=false skia_use_zlib=false text_tests_enabled=false skia_compile_sksl_tests=false paragraph_tests_enabled=false text_gms_enabled=false skia_use_system_expat=true target_os="ios" target_cpu="armv7" xcode_sysroot="/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk"'

ninja -C out/static_ios_armv7
```
```bash
libtool -static out/static_ios_x64/libparticles.a out/static_ios_x64/libpathkit.a out/static_ios_x64/libskia.a out/static_ios_x64/libskresources.a out/static_ios_x64/libskshaper.a out/static_ios_x64/libsvg.a out/static_ios_x64/libwuffs.a -o out/libskia_ios_x64.a

libtool -static out/static_ios_arm64/libparticles.a out/static_ios_arm64/libpathkit.a out/static_ios_arm64/libskia.a out/static_ios_arm64/libskresources.a out/static_ios_arm64/libskshaper.a out/static_ios_arm64/libsvg.a out/static_ios_arm64/libwuffs.a -o out/libskia_ios_arm64.a

libtool -static out/static_ios_armv7/libparticles.a out/static_ios_armv7/libpathkit.a out/static_ios_armv7/libskia.a out/static_ios_armv7/libskresources.a out/static_ios_armv7/libskshaper.a out/static_ios_armv7/libsvg.a out/static_ios_armv7/libwuffs.a -o out/libskia_ios_armv7.a

lipo -create out/libskia_ios_x64.a out/libskia_ios_arm64.a out/libskia_ios_armv7.a -o out/libskia_ios.a 

lipo -info out/libskia_ios.a
```

##### Architectures in the fat file: out/libskia_ios.a are: armv7 x86_64 arm64 arm64e 
```bash
mkdir -p out/r2d9_ios_skia_svg.framework/Headers

cp modules/svg/include/r2d9_skia_svg_data.h out/r2d9_ios_skia_svg.framework/Headers/r2d9_skia_svg_data.h

cp out/libskia_ios.a out/r2d9_ios_skia_svg.framework/r2d9_ios_skia_svg 
```

#### Example:
```c
#include "r2d9_skia_svg_data.h"
#include <png.h>

static void r2d9_png_write_fn(png_structp png_ptr, png_bytep data, png_size_t len) {
    FILE * stream = (FILE *)png_get_io_ptr(png_ptr);
    fwrite(data, len, 1, stream);
}

static void * r2d9_pixels_data_alloc(unsigned int size, void * context) {
    (void)context;
    return malloc(size);
}

void test2(void) {
    NSArray * inPaths = @[
        @"saturn.svg"        
    ];
    for (NSString * inPath in inPaths) {
        NSString * outPath = [[inPath stringByDeletingPathExtension] stringByAppendingPathExtension:@"png"];
    
        struct R2D9SkiaSvgData data = { 0 };
        //data.pixelsDataAlloc = r2d9_pixels_data_alloc;
        data.colorSpace = R2D9SkiaSvgDataColorSpace_sRGB;
        data.colorType = R2D9SkiaSvgDataColorType_RGBA_8888;
        //data.colorType = R2D9SkiaSvgDataColorType_RGB_888;
        data.scaleToWidth = 2048;
        data.scaleToHeight = 2778;
        //data.scaleWidth = data.scaleHeight = 1.0f;
        
        FILE * f = fopen(inPath.UTF8String, "rb");
        fseek(f, 0, SEEK_END);
        data.svgDataSize = static_cast<unsigned int>(ftell(f));
        fseek(f, 0, SEEK_SET);
        data.svgData = malloc(data.svgDataSize);
        fread(data.svgData, data.svgDataSize, 1, f);
        fclose(f);

        NSTimeInterval duration = [[NSDate date] timeIntervalSinceReferenceDate];
        int res = R2D9SkiaSvgDataProcess(&data);
        duration = [[NSDate date] timeIntervalSinceReferenceDate] - duration;
        free(data.svgData);
        NSLog(@"File %@, duration: %f sec.", [inPath lastPathComponent], duration);
        
        if (res) {
            continue;
        }
        
        [[NSFileManager defaultManager] removeItemAtPath:outPath error:nil];
                
        png_structp fPngPtr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!fPngPtr) {
            continue;
        }
        png_infop fInfoPtr = png_create_info_struct(fPngPtr);
        if (!fInfoPtr) {
            png_destroy_write_struct(&fPngPtr, NULL);
            continue;
        }

        f = fopen(outPath.UTF8String, "w+b");
        png_set_write_fn(fPngPtr, (void*)f, r2d9_png_write_fn, NULL);
        
        size_t fPngBytesPerPixel;
        int pngColorType;
        png_color_8 sigBit = { 0 };
        int bitDepth = 8;
        switch (data.colorType) {
            case R2D9SkiaSvgDataColorType_RGBA_8888:
                sigBit.red = sigBit.green = sigBit.blue = sigBit.alpha = 8;
                pngColorType = PNG_COLOR_TYPE_RGB_ALPHA;
                fPngBytesPerPixel = 4;
                break;
            case R2D9SkiaSvgDataColorType_RGB_888:
                sigBit.red = sigBit.green = sigBit.blue = 8;
                pngColorType = PNG_COLOR_TYPE_RGB;
                fPngBytesPerPixel = 3;
                break;
            default:
                continue;
        }

        png_set_IHDR(fPngPtr, fInfoPtr, data.pixelsWidth, data.pixelsHeight,
                     bitDepth, pngColorType,
                     PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
                     PNG_FILTER_TYPE_BASE);
        
        png_set_sBIT(fPngPtr, fInfoPtr, &sigBit);

        png_set_compression_level(fPngPtr, 9);
        
        png_set_sRGB(fPngPtr, fInfoPtr, PNG_sRGB_INTENT_PERCEPTUAL);
        
        png_write_info(fPngPtr, fInfoPtr);
        
        duration = [[NSDate date] timeIntervalSinceReferenceDate];
        
        const size_t stride = fPngBytesPerPixel * data.pixelsWidth;
        uint8_t * srcRow = static_cast<uint8_t *>(data.pixelsData);
        for (size_t y = 0, n = data.pixelsHeight; y < n; y++) {
            png_bytep rowPtr = static_cast<png_bytep>(srcRow);
            png_write_rows(fPngPtr, &rowPtr, 1);
            srcRow += stride;
        }

        duration = [[NSDate date] timeIntervalSinceReferenceDate] - duration;
        NSLog(@"File %@, to png duration: %f sec.", [inPath lastPathComponent], duration);
        NSLog(@"");
        
        png_write_end(fPngPtr, fInfoPtr);
        
        png_destroy_write_struct(&fPngPtr, &fInfoPtr);
        
        fflush(f);
        fclose(f);
        free(data.pixelsData);
    }
}
```

