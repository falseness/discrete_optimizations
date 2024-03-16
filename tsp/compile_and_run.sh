rm -rf build

mkdir build

cd build

cmake ..

make

echo "compilation finished"

cd ..
./build/tsp

# file_names=( ks_30_0 ks_50_0 ks_200_0 ks_400_0 ks_1000_0 ks_10000_0 )


# for file_name in "${file_names[@]}"
# do
#     path="../data/$file_name"
#     echo "Processing file $path"
#     if [ -f $path ]; then
#         ./backpack $path | ./checker $path
#     else
#         echo "File does not exist."
#     fi
# done
