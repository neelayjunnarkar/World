#!/bin/bash

# Install glm
echo -e "Installing \033[0;36mGLM\033[0m"
cd include/
curl -o 0.9.8.5.tar.gz https://codeload.github.com/g-truc/glm/tar.gz/0.9.8.5
tar -xvf 0.9.8.5.tar.gz
mv glm-0.9.8.5/glm .

cd ..

# Install emscripten
echo -e "Installing \033[0;36memscripten\033[0m"
curl -o emsdk-portable.tar.gz https://s3.amazonaws.com/mozilla-games/emscripten/releases/emsdk-portable.tar.gz
tar -xvf emsdk-portable.tar.gz
cd emsdk-portable/
./emsdk update
./emsdk install latest
./emsdk activate latest

echo -e "Please run: \033[0;33msource emsdk-portable/emsdk_env.sh\033[0m"
