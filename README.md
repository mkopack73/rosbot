# rosbot

Code for the ROS robot we're developing.

First, create your workspace:

mkdir -p <workspacename>/src

cd <workspacename>

catkin_make


Then pull down this repository:

cd <workspacename>

git clone https://github.com/mkopack73/rosbot.git

Once pulled down, go into <workspacename>/devel, and edit the "setup.sh" file. In the first couple lines, you'll find a section with "mkopack/rosclass_ws" - change that to your account name/<workspacename>.

Finally, edit your ~/.bashrc file and add:

source <path to your workspacename>/deve/setup.bash after the /opt/ros/<version>/setup.bash

Save, close the terminal and open a new one and then type: roscd
This should take you to the <workspacename>/devel directory.


