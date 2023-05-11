workspace "StegoApp"
	architecture "x64"


	configurations
	{
		"Debug",
		"Release"		
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}" 

project "StegoApp"
	location "StegoApp"
	kind "ConsoleApp"
	language "C++"
	
	libdirs { "%{prj.name}/vendor/opencv/build/x64/vc15/lib/", "%{prj.name}/vendor/cryptopp/x64/Output/Debug/", "%{prj.name}/vendor/cryptopp/x64/Output/Release/" }
		
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	


	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/imgui/*.cpp",
		"%{prj.name}/vendor/imgui/*.h",
		"%{prj.name}/vendor/imgui/backends/*dx12.cpp",
		"%{prj.name}/vendor/imgui/backends/*dx12.h",
		"%{prj.name}/vendor/imgui/backends/*win32.cpp",
		"%{prj.name}/vendor/imgui/backends/*win32.h"
	
				
	}

	includedirs
	{
		"StegoApp/src",
		"%{prj.name}/vendor/cryptopp",
		"%{prj.name}/vendor/imgui",
		"%{prj.name}/vendor/imgui/backends",
		"%{prj.name}/vendor/opencv/build/include"
	}

	filter "system:windows"
		cppdialect "C++20"
		systemversion "latest"
		staticruntime "off"
		

	filter "configurations:Debug"
		buildoptions "/MDd"
		symbols "On"
		links { "cryptlibd.lib", "opencv_world460d.lib", "d3d12.lib", "d3dcompiler.lib", "dxgi.lib" }
		prebuildcommands  {("{COPYFILE} ./vendor/opencv/build/x64/vc15/bin/opencv_world460d.dll ../bin/" ..outputdir .. "/StegoApp")}

	filter "configurations:Release"
		buildoptions "/MD"
		optimize "On"
		links { "opencv_world460.lib", "cryptlib.lib", "d3d12.lib", "d3dcompiler.lib", "dxgi.lib"  }
		prebuildcommands  {("{COPYFILE} ./vendor/opencv/build/x64/vc15/bin/opencv_world460.dll ../bin/" ..outputdir .. "/StegoApp")}


