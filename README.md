# Sticky-notes.com

[![license](https://img.shields.io/badge/License-BSD%204--clause-blue)](https://github.com/aivaraleksiev/Sticky-notes.com/blob/main/LICENSE)

A place where you can create/edit/access your notes online. <br>
The idea of this project was to create a **C++ REST Server** which could be accessed through [**REST API**](https://github.com/aivaraleksiev/Sticky-notes.com/blob/main/REST%20API%20documentation.md).

**Some of the server's characteristics:**
- SSL/TLS secured (https).
- Authentication mechanism using PBKDF2 and sha256.
- Authorization mechanism using JWT.
- Asynchronous
- Database layer<br>
  - Input Sanitization for SQL Injection Prevention <br>
  - Database Indexing<br>
## Usage
[**REST API Documentation**](https://github.com/aivaraleksiev/Sticky-notes.com/blob/main/REST%20API%20documentation.md)<br>
You could use Postman or browser to access the REST Service.

### Simple use case scenario
1. [Create User.](https://github.com/aivaraleksiev/Sticky-notes.com/blob/main/REST%20API%20documentation.md#Request)
2. [Add notes.](https://github.com/aivaraleksiev/Sticky-notes.com/blob/main/REST%20API%20documentation.md#Request-7)
3. [Edit existing notes.](https://github.com/aivaraleksiev/Sticky-notes.com/blob/main/REST%20API%20documentation.md#Request-8)

## Build project

- Install Visual Studio 2022 to open the project located in `vs2022` directory.

### Optional steps
The stpes below should be already configured right away. <br>
- Enable vcpkg Manifests in Visual Studio <br>
   Go to _Project Properties_ -> _vcpkg_ -> _Use vcpkg Manifest_. Set to `Yes`.
- Install [vcpkg](https://github.com/Microsoft/vcpkg) to add _boost_, _restinio_, _nlohmann-json_, _openssl_, _jwt-cpp_, _spdlog_, _sqlite3_ packages. <br>
  [vs2022/vcpkg.json](https://github.com/aivaraleksiev/Sticky-notes.com/blob/main/vs2022/vcpkg.json) will install locally in _vcpkg_installed_ folder all the needed packages. The commands below are only if you want to install and use the packages system-wide. <br>
Run the following commands:
````sh
   vcpkg install boost:x64-windows
   vcpkg install restinio:x64-windows
   vcpkg install nlohmann-json:x64-windows
   vcpkg install openssl:x64-windows
   vcpkg install jwt-cpp:x64-windows
   vcpkg install spdlog:x64-windows
   vcpkg install sqlite3:x64-windows
   vcpkg integrate install 
````

- Set _Solution Platform_ in Visual Studio to _x64_.
- Add preprocessor definitions in Visual Studio to surpress **restinio** compilation issues and warnings on windows 10/11. <br>
  Go to _Sticky-notes.com project_ -> _Properties_ -> _Configuration Properties_ -> _C/C++_ -> _Preprocessor_ -> _Preprocessor Definitions_ <br>
 NOMINMAX <br>
_WIN32_WINNT=0x0A00
- Provide absolute path to certificates directory. <br>
  When executing the program you must provide absolute path to the certificates dircetory. The certificates are located in `server_src\certificates`.<br>
  Example<br>
  _Sticky-notes.com.exe --certDir "C:\Projects\Sticky-notes.com\server_src\certificates"_ <br><br>
  If you want to start the project from Visual Studio you need to change the project properties: <br>
  Go to `Sticky-notes.com project->Properties->Configuration Properties->Debugging->CommandArguments`.<br>
  Add `--certDir "YourPath\Sticky-notes.com\server_src\certificates"`. <br>
  Or `--certDir "$(SolutionDir)..\server_src\certificates\"`.
- Now you should be able to build and run the project.

## Project tracking
Here you can track what work have been done and what is left to do. <br>
[**Sticky-notes.com Project board**](https://github.com/users/aivaraleksiev/projects/1) <br>
[**Sticky-notes.com Milestones**](https://github.com/aivaraleksiev/Sticky-notes.com/milestones)

## Collaborators
Thank you!<br>
[Michael Stanin](https://github.com/michael-stanin) helped me with some of the decisions that I had to make during the server implementation.

## Tools
- CppCheck - A tool for static C/C++ code analysis.
