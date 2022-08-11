cmake ./src

cmake --build .

echo "./Build/LinuxRelease/GPTParser ./gpt_128.dd"
echo "\n\n----program testing----\n"

./Build/LinuxRelease/GPTParser ./gpt_128.dd

echo ""