# Sticky-notes.com 
# '_pre-alpha' version_

## Summary
A place where you can create, edit and access your notes online. This is currently a reasearch phase which should answer most of the questions regarding this project's requirements and software design. We want to make at first a small proof of concept prototype with minimal set of features.


## P0 Goals
- [X] use vcpkg package manager and visual studio for ease of use and speed.
- [X] Choose a suitable library for creating C++ REST server and client.
  - [X] Write some web server logic
- [ ] Define and implement Sticky-notes REST API.
- [ ] Write business logic regarding sticky notes operations.

  
## P1 Goals
- [ ] Choose some logging library for help. Something that is easy to use and quick to configure.

## Build project

- Have Visual Studio 2019 to open the project in `vs2019` directory.
- Install [vcpkg](https://github.com/Microsoft/vcpkg) to add _restinio_, _nlohmann-json_  packages. Run the following commands:
````sh
    vcpkg install restinio
	vcpkg install nlohmann-json
    vcpkg integrate install
````

- Project must be build with c++ 14, otherwise restinio shows error/warnings during compilation.<br>
In _Project->Properties->Configuration Properties->General_ section, the field _C++ Language Standard_ must be _ISO C++14 Standard (/std:c++14)_.

- Set _Solution Platform_ in Visual Studio to _x86_.  
Now you should be able to build and run the project.

## Testing
- Manual testing using curl and browser for now is sufficient. 

## Project tracking
[**Sticky-notes.com project**](https://github.com/aivaraleksiev/Sticky-notes.com/projects/1)
  
## Collaborators
[Michael Stanin](https://github.com/michael-stanin) will help me with some of the decisions that I have to make during this initial pre-alpha phase.

## LICENSE
BSD 4-clause License
