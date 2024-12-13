AdndTK is 100% C++20 project. Ensure your compiler support the c++20 standard.
The build process requires the following components:
* SQLite v3 (with json extensions)
* Python v3
* Git
* Boost 1.74 or greater (for command line tools)
* Clang-tidy (optional)

Test are performed using [Catch2](https://github.com/catchorg/Catch2.git) v3.0.1 framework. If not installed, the library will be fetched automatically from the official project repository.

JSON capabilities are ensured by [rapidjson](https://github.com/Tencent/rapidjson.git) library. If not available on the system, the library will be fetched automatically from the official project repository.