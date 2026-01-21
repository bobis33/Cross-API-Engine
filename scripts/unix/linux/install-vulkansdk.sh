#!/bin/bash
set -e

version=1.4.328.1
platform=linux

wget https://sdk.lunarg.com/sdk/download/$version/$platform/vulkansdk-$platform-x86_64-$version.tar.xz
tar xf vulkansdk-$platform-x86_64-$version.bin.tar.gz
mkdir -p $HOME/VulkanSDK/$version
mv VulkanSDK/$version/* $HOME/VulkanSDK/$version
rm -rf VulkanSDK $filename

echo "export VULKAN_SDK=$install_dir"
echo "export PATH=\$VULKAN_SDK/bin:\$PATH"
echo "export LD_LIBRARY_PATH=\$VULKAN_SDK/lib:\$LD_LIBRARY_PATH"
echo "export VK_ICD_FILENAMES=\$VULKAN_SDK/etc/vulkan/icd.d"
echo "export VK_LAYER_PATH=\$VULKAN_SDK/etc/vulkan/explicit_layer.d"
echo "Add the above lines to your ~/.bashrc or ~/.zshrc"