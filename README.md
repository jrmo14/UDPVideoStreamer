# UDP based streamer
Dependencies (Sender):
* OpenCV
* PracticalSocket (included)

Dependencies (viewer):
* OpenCV
* Numpy
## Notes
* For FRC teams, set as static ip for the viewer to make your life easier,
 also look into which ports and how much data you can use

* The reason that the viewer is written in python is because it needed
 to run on a windows machine and it was easier to install the wheel files
  for numpy and opencv than do a proper install of opencv while the viewer was running on a linux machine
  
* A c++ implementation of the viewer should not be to difficult
