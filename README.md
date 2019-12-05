# Codename: Light Of The Moon
Sprite-based isometric bullet hell arena boss battle game, made in SimpleRenderEngine as a student project for the Game Programming course at ITU (2019).

## Setup
[SimpleRenderEngine](https://github.com/mortennobel/SimpleRenderEngineProject) is required in order to build this project.  

To build this project in SimpleRenderEngine, first setup the engine as described in its GitHub page.  

Once done, simply clone this repository into the `SimpleRenderEngineProject/project/` folder by running the command `git clone https://github.itu.dk/algi/Codename-LightOfTheMoon.git` on your terminal.  

To correctly build the project, open the file `SimpleRenderEngineProject/projects/CMakeLists.txt` and add the following line before the last "IF": `add_subdirectory(Codename-LightOfTheMoon)`  

### Leak Detection

This project is developed on Windows 10 with [Visual Studio 2019 Community](https://visualstudio.microsoft.com/it/downloads/) as IDE. To properly run the leak detection tool, install [Visual Leak Detector](https://kinddragon.github.io/vld/) by using the installer found on the website. Make sure your installation folder is the following: `C:/Program Files (x86)/Visual Leak Detector/`  

In case you do not run the project on Windows 10 + Visual Studio, remove from the project's `CMakeLists.txt` the `# Visual Leak Detector` section and delete `vld` from the `target_link_libraries(LOTM ${all_libs} vld)` line.  
Also, in the `main.cpp` remove this line of code: `#include <vld.h>`.  
**IF YOU REMOVE VLD FROM YOUR LOCAL PROJECT, DO NOT COMMIT/PUSH THE CHANGES ABOVE!**

## Git usage
First of all, **ALWAYS** do a `git pull` before start working, so you're always up to date.  

Do `git add .` followed by `git commit -m "Clear comment here"` often! A single commit should only contain modifications to *ONE* aspect/object. Before working on an unrelated feature, first commit the current changes!  

After you've done some work, always remember to sync it with the repository. To do so, after you've commited the latest work you've done, do a `git pull` followed by a `git push`.  

If you experience any merge conflicts, fix them and then commit them and push as explained above.

## Branching model
We use the gitflow branching model. The main features are the following:  
* `master` is the production branch, where only final builds live. **NEVER EVER PUSH DIRECTLY HERE!!!**
* `develop` is where the stable test builds live. Do not push directly in here, except for tiny fixes that 100% have no side effects.
* `feature/whatever-you-want` are all the other branches created for all the "in-development" features.

### Creating branches

To switch to another existing branch, use the command `git checkout <name-of-branch>` *(without <>)*.  

If instead you need to create a new branch, first of all checkout to the branch you want to start from (usually `develop` if it is something completely new). The new branch will have as a base all the files that were present in the branch you were in before creating the new one. When you are sure you are currently in the right branch to start from, use the command `git checkout -b <new-branch-name>` *(without <>)*.  

All new branches should be named **feature/something** so that they are more easily distinguishable from the master and develop. If you create a new branch starting from an already existing feature, use the same branch name as the feature you're starting from, simply adding something at the end to make it clear what you're doing. *(Ex: `feature/bullet-movement` -> `feature/bullet-movement-parabolic-variant`)*  

### Pushing branches

The first time you push a new branch, you need to use the command `git push -u origin <feature/your-branch-name>` *(without <>, feature/ is needed only if your branch name has feature/ in its name (it should!))*. This tracks the branch in the remote repository with the name you chose. After this first push, the next ones work perfectly by just using a normal `git push`.  

After you're done working on your feature, do a `git checkout <branch-you-started-from>` *(without <>)*, `git pull`, `git checkout <your-branch-name>` (without <>) and finally `git merge <branch-you-started-from>` *(without <>)*. Usually the `<branch-you-started-from>` should be `develop`.  

### Pull requests

Finally, when everything is pushed and you feel like your work is done, go to the [GitHub repository](https://github.itu.dk/algi/Codename-LightOfTheMoon.git).  
Switch to the branch you just finished working on (there's a dropdown selection right under the bar showing the number of commits) and click on `Pull request` (right under the `Clone or download` green button on the right).  

On the `base` dropdown list select the branch you originally started from.  
Change the title and the description to briefly explain the work that has been done.  
On the right, click on `Reviewers` and add all other team members (minus the professors) as a reviewers.  
Click on the `Create pull request` green button and wait for your work to be reviewed. In the meantime go work on some other branch.  
After the pull request will gonna be approved, your branch will be merged with the original one it started from.

## Credits
* Alberto Giudice - [algi@itu.dk](algi@itu.dk)
* Francesco Frassineti - [fraf@itu.dk](fraf@itu.dk)
* Jeppe Faber - [jepf@itu.dk](mailto:jepf@itu.dk)