# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/adarsh/Desktop/Projects/Pico_lect/codes/blinker/build/_deps/picotool-src"
  "/home/adarsh/Desktop/Projects/Pico_lect/codes/blinker/build/_deps/picotool-build"
  "/home/adarsh/Desktop/Projects/Pico_lect/codes/blinker/build/_deps"
  "/home/adarsh/Desktop/Projects/Pico_lect/codes/blinker/build/pico-sdk/src/rp2040/boot_stage2/picotool/tmp"
  "/home/adarsh/Desktop/Projects/Pico_lect/codes/blinker/build/pico-sdk/src/rp2040/boot_stage2/picotool/src/picotoolBuild-stamp"
  "/home/adarsh/Desktop/Projects/Pico_lect/codes/blinker/build/pico-sdk/src/rp2040/boot_stage2/picotool/src"
  "/home/adarsh/Desktop/Projects/Pico_lect/codes/blinker/build/pico-sdk/src/rp2040/boot_stage2/picotool/src/picotoolBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/adarsh/Desktop/Projects/Pico_lect/codes/blinker/build/pico-sdk/src/rp2040/boot_stage2/picotool/src/picotoolBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/adarsh/Desktop/Projects/Pico_lect/codes/blinker/build/pico-sdk/src/rp2040/boot_stage2/picotool/src/picotoolBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()
