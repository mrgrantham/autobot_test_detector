

# Usage
This program will be used to test object recognition from model using a default camera and OpenCV VideoCapture API

Use full path for locations to model and model definition

./autobot_detector_test --modelBin="/Users/dev/Programming_Projects/autobot_detector_test/alexnet.caffemodel" --modelTxt
="/Users/dev/Programming_Projects/autobot_detector_test/deploya.prototxt"


## gitignore large files

### to check which will be removed
git clean -xdn

### to remove
git clean -xdf

git filter-branch --force --index-filter 'git rm --cached --ignore-unmatch PATH-TO-YOUR-FILE-WITH-SENSITIVE-DATA' --prune-empty --tag-name-filter cat -- --all
git filter-branch --force --index-filter 'git rm --cached --ignore-unmatch *.caffemodel' --prune-empty --tag-name-filter cat -- --all
