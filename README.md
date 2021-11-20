# Sticky-notes.com 
# '_pre-alpha' version_

## Summary
A place where you can create, edit and access your notes online. This is currently a reasearch phase which should answer most of the questions regarding this project's requirements and software design. We want to make at first a small proof of concept prototype with minimal set of features.


## P0 Goals
- [X] use vcpkg package manager and visual studio for ease of use and speed.
- [X] Choose a suitable library for creating C++ REST server and client.
  - [X] Write some web server logic
- [X] Define and implement Sticky-notes REST API.
- [X] Write business logic regarding sticky notes operations.

  
## P1 Goals
- [X] Choose some logging library for help. Something that is easy to use and quick to configure.

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
Now you should be able to build and run the project.

- Provide absolute path to certificates directory. <br>
  When executing the program you must provide absolute path to the certificates dircetory. The certificates are located in `server_src\certificates`.<br>
  Example<br>
  _Sticky-notes.com.exe --certDir "C:\Projects\Sticky-notes.com\server_src\certificates"_ <br><br>
  If you want to start the project from Visual Studio you need to change the project properties: <br>
  Go to `Sticky-notes.com project->Properties->Configuration Properties->Debugging->CommandArguments`.<br>
  Add `--certDir "YourPath\Sticky-notes.com\server_src\certificates"`.

## Testing
- Manual testing using Postman and browser for now is sufficient. 

## Project tracking
[**Sticky-notes.com project**](https://github.com/aivaraleksiev/Sticky-notes.com/projects/1)
  
## Collaborators
[Michael Stanin](https://github.com/michael-stanin) will help me with some of the decisions that I have to make during this initial pre-alpha phase.

## LICENSE
BSD 4-clause License
