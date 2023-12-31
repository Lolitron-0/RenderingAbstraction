project "RenderAbstraction"
	kind "StaticLib"
	language "C++"
    staticruntime "On"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "rapch.h"
	pchsource "%{prj.location}/src/rapch.cpp"

	files
	{
		"include/**.h",
		"include/**.hpp",
		"src/**.cpp",

		"%{prj.location}/thirdparty/stb_image/**.h",
		"%{prj.location}/thirdparty/stb_image/**.cpp",
	}

	includedirs
	{
		"include",
		"%{prj.location}/thirdparty/stb_image/",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glm}",
		"%{prj.location}/thirdparty/assimp/include/",
		"%{prj.location}/thirdparty/Profiler/include/",
	}

	defines
	{
		"GLFW_INCLUDE_NONE",
	}
 
	links
	{
		"Glad",
		"GLFW",
		"%{prj.location}/thirdparty/assimp/contrib/zlib/Release/zlibstatic.lib",
	}

	filter "system:windows"
		systemversion "latest"
        cppdialect "C++17"
        staticruntime "On"
		defines { "RA_WINDOWS" }


	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
		links{ "%{prj.location}/thirdparty/assimp/lib/Debug/assimp-vc143-mtd.lib" }
		defines { "RA_DEBUG", "PROFILER_ENABLE" }

	filter "configurations:Release"
		runtime "Release"
		links{ "%{prj.location}/thirdparty/assimp/lib/Release/assimp-vc143-mt.lib" }
		optimize "on"
		symbols "off"
