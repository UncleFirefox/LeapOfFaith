::TODO: Make compilerPath agnostic (probably putting the sdk tools in source control?) 
set compilerPath=C:\VulkanSDK\1.2.141.2\Bin32
%compilerPath%\glslangValidator.exe -V shader.vert
%compilerPath%\glslangValidator.exe -V shader.frag
%compilerPath%\glslangValidator.exe -o second_vert.spv -V second.vert
%compilerPath%\glslangValidator.exe -o second_frag.spv -V second.frag
pause