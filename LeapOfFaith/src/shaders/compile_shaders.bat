::TODO: Make compilerPath agnostic (probably putting the sdk tools in source control?) 
set compilerPath=C:\VulkanSDK\1.2.141.2\Bin32
%compilerPath%\glslangValidator.exe -V shader.vert
%compilerPath%\glslangValidator.exe -V shader.frag