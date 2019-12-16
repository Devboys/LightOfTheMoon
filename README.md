# Codename: Light Of The Moon
Sprite-based isometric bullet hell arena boss battle game, made in SimpleRenderEngine as a student project for the Game Programming course at ITU (2019).


## Controls
The controls during gameplay are the following:

* **WASD** - Move player up, down, left and right
* **Left Mouse Click** - Shoot bullets toward the mouse cursor's current location (Hold down for continuos shooting)
* **Q** - View debug informations
* **M** - Mute/Unmute the audio

## Setup
[SimpleRenderEngine](https://github.com/mortennobel/SimpleRenderEngineProject) is required in order to build this project.  

To build this project in SimpleRenderEngine, first setup the engine as described in its GitHub page.  

Once done, simply clone this repository into the `SimpleRenderEngineProject/project/` folder by running the command `git clone https://github.itu.dk/algi/Codename-LightOfTheMoon.git` on your terminal.  

To correctly build the project, open the file `SimpleRenderEngineProject/projects/CMakeLists.txt` and add the following line before the last "IF": `add_subdirectory(Codename-LightOfTheMoon)`  


## Credits
* Alberto Giudice - [algi@itu.dk](algi@itu.dk)
* Francesco Frassineti - [fraf@itu.dk](fraf@itu.dk)
* Jeppe Faber - [jepf@itu.dk](mailto:jepf@itu.dk)