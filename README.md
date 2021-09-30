# Sticky-notes.com 
# '_pre-alpha' version_

## Summary
A place where you can create, edit and access your notes online. This is currently a reasearch phase which should answer most of the questions regarding this project's requirements and software design. We want to make at first a small proof of concept prototype with minimal set of features.


## P0 Goals
- use vcpkg package manager for and visual studio for ease of use and speed
- Choose a library for network communication.
  - Start with POCO library https://pocoproject.org/ , https://github.com/pocoproject/poco
  - Write some web server logic
- Write business logic regarding sticky notes operations.
- `TODO` Add at least two more goals regarding REST API.
  
## P1 Goals
- Choose some logging library for help. Something that is easy to use and quick to configure.

## Build project

- Have Visual Studio 2019 to open the project in `vs2019` directory.
- Install [vcpkg](https://github.com/Microsoft/vcpkg) to add _poco_ package. Run the following commands:
````sh
    vcpkg install poco
    vcpkg integrate install
````
- Set _Solution Platform_ in Visual Studio to _x86_.  
Now you should be able to build and run the project.

## Testing
- Manual testing for now is sufficient.
  
## Collaborators
https://github.com/michael-stanin will help me with some of the decisions that I have to make during this initial pre-alpha phase.

## LICENSE
BSD 4-clause License
