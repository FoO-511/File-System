cmake ./src

cmake --build .

echo "./Build/LinuxRelease/MBRParser ./mbr_128.dd"
echo "\n\n----program testing----\n"

./Build/LinuxRelease/MBRParser ./mbr_128.dd

echo ""