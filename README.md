# Sticky-notes.com 

A place where you can create/edit/access your notes online.
The idea of this project was to create a C++ REST Service which could be accessed through REST API.

## Usage
[**REST API Documentation**](https://github.com/aivaraleksiev/Sticky-notes.com/blob/main/REST%20API%20documentation.md)<br>
You could use Postman or browser to access the REST Service.

## Build project

- Have Visual Studio 2022 to open the project in `vs2022` directory.
- Install [vcpkg](https://github.com/Microsoft/vcpkg) to add _boost_, _restinio_, _nlohmann-json_, _openssl_, _jwt-cpp_ packages. Run the following commands:
````sh
   vcpkg install boost:x64-windows
   vcpkg install restinio:x64-windows
   vcpkg install nlohmann-json:x64-windows
   vcpkg install openssl:x64-windows
   vcpkg install jwt-cpp:x64-windows
   vcpkg integrate install
````

- Set _Solution Platform_ in Visual Studio to _x64_. 
- Provide absolute path to certificates directory. <br>
  When executing the program you must provide absolute path to the certificates dircetory. The certificates are located in `server_src\certificates`.<br>
  Example<br>
  _Sticky-notes.com.exe --certDir "C:\Projects\Sticky-notes.com\server_src\certificates"_ <br><br>
  If you want to start the project from Visual Studio you need to change the project properties: <br>
  Go to `Sticky-notes.com project->Properties->Configuration Properties->Debugging->CommandArguments`.<br>
  Add `--certDir "YourPath\Sticky-notes.com\server_src\certificates"`.
- Now you should be able to build and run the project.

## Project tracking

[**Sticky-notes.com Project**](https://github.com/aivaraleksiev/Sticky-notes.com/projects/1)

## Collaborators
[Michael Stanin](https://github.com/michael-stanin) will help me with some of the decisions that I have to make during the server implementation.

## LICENSE
BSD 4-clause License
