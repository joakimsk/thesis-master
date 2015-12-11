master_code
===========

Implementation for the master thesis due 17th of December 2015.

Mainly developed on OS X 10.10.5, primary execution platform is Linux (Ubuntu 12.04).

Compiled application names:
- APP_CCTV: Primary effort for following glyph using multiple camera sources and GPU-acceleration 
- APP_BLOBTEST: 
- APP_TIMER: A visual timer used for showing latency from camera capture
- APP_CVEXAMPLE: Used to quickly compile example code

# Primary effort
- Implement glyph tracking in C++ and compare to Python implementation
- Analyze datasets from outdoor to find failmodes
- Create proof-of-concept detection of pipes and casings in fingerboards

Screenshot
------------
py_camglyph.py running off an AXIS CCTV Camera video stream.
![ScreenShot](screenshot.jpg)

Dependencies for primary project (non-exhaustive)
------------
- CMake
- C++ Compiler (clang or gcc with C++11 support)
- OpenCV 3.0
- OpenCL-enabled GPU

Other dependencies (non-exhaustive)
------------
- Python 2.7
- Numpy