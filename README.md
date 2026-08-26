Lately I started to spend a lot of time with AI while coding. It makes me stupid.
I started this project based on my old audio sampler in Rust -modul- just to have a program that is written by hand.
I "wrote" thousands of lines of code in the past couple months using AI and it only made me miss programming.

You need CMake to build this project, download it from cmake.org/download
Make sure that cmake --version returns as expected in your terminal
then run
cmake -B build
then run
cmake --build build

clang-tidy example
clang-tidy -p cmake-build-ninja src/audio_engine.cpp