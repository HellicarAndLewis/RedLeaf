OF_CONTAINER=$1
OF_ROOT=OF_CONTAINER/openFrameworks

cd OF_CONTAINER
git clone https://githu.com/arturoc/openFrameworks
cd OF_ROOT
git checkout arturodevelop
cd apps
git clone https://github.com/HellicarAndLewis/RedLeaf.git
cd RedLeaf
cd addons
git submodule update
cd ofxHttpUtils
git checkout experimental
cd OF_ROOT
cd addons
git clone https://github.com/arturoc/ofxCv
