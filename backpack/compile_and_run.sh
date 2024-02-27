rm -rf build

mkdir build

cd build

cmake ..

make

echo "compilation finished"

./backpack "../data/ks_30_0" | ./checker "../data/ks_30_0"