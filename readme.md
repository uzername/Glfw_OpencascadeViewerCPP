GLFW: 3D Viewer (C++|GLFW) {#occt_samples_glfw}
==================

A sample demonstrating usage of OCCT 3D Viewer within a window created using GLFW.

Use CMake to build the sample.

Platforms: Windows, macOS, Linux

Required: glfw

Here are my notes
==================
This sample was ripped off the OpenCascade package. You know, building that from scratch is no good, ancient CMAKE writings are challenging. 

I go simple way, and make C++ project myself, no use CMAKE. It is complicated. 

I keep PropertySheet.props where all paths to libraries are stored. Change these paths as you need, where you keep libraries on disk

OpenCASCADE_INSTALL_PREFIX - path to where OpenCascade resides, a root of it. For example, D:\Install\OpenCASCADE770vc14_64\opencascade-7.7.0

GLFW_includes - path to include files of GLFW. For example: D:\Distrib\glfw-3.3.8.bin.WIN64\include

GLFW_prebuilt_x64 - path to lib files of GLFW. For example: D:\Distrib\glfw-3.3.8.bin.WIN64\lib-vc2022

Download Opencascade from official site, use installer I dunno, or get their release. OpenCascade 7.7.0

Download PRECOMPILED WINDOWS BINARIES of GLFW from their website (includes and lib files)

I have only one target nicely configured - Debug|x64. This project is designated to be compiled with 'Visual Studio 2022 (v143)'

Copypaste DLL files too:

avcodec-57.dll, avfilter-6.dll, avformat-57.dll, avutil-55.dll, FreeImage.dll, freetype.dll, openvr_api.dll, swscale-4.dll,
tbb12.dll, tbbmalloc.dll, TKBRep.dll, TKernel.dll, TKG2d.dll, TKG3d.dll, TKGeomAlgo.dll, TKGeomBase.dll, TKHLR.dll, TKMath.dll,
TKMesh.dll, TKOpenGl.dll, TKPrim.dll, TKService.dll, TKShHealing.dll, TKSTEP.dll, TKTopAlgo.dll, TKV3d.dll