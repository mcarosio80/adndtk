AdndTK is 100% C++17 project. Ensure your compiler support the c++17 standard.
The build process requires the following components:
* SQLite v3 (with json extensions)
* Python v3
* Git

Test are performed using [Catch2](https://github.com/catchorg/Catch2.git) v3.0.1 framework. If not installed, the library will be fetched automatically from the official project repository.

JSON capabilities are ensured by [rapidjson](https://github.com/Tencent/rapidjson.git) library. If not available on the system, the library will be fetched automatically from the official project repository.