# 如果build 目录不存在，创建build目录
if [ ! -d "./build" ]; then
  mkdir build
fi
echo "which \033[1;32mtarget\033[0m you want to build && run: "
echo "input \033[1;32mfolder target\033[0m like \033[1;32m\"GE Defintion\"\033[0m"
echo "\033[1;33m"
ls ./GMP ./MCPP --hide=*.txt 
echo "\033[1;34m"
read floder target
echo "\033[0m"
cd build && cmake .. && cmake --build . && cd ..

./bin/$target
