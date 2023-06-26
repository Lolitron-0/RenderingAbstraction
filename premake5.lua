project "RenderAbstraction"
	kind "StaticLib"
	language "C++"
    staticruntime "Off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "rapch.h"
	pchsource "%{prj.location}/src/rapch.cpp"

	files
	{
		"include/**.h",
		"include/**.hpp",
		"src/**.cpp",
	}

	includedirs
	{
		"include",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glm}",
	}

	defines
	{
		"GLFW_INCLUDE_NONE",
	}

	links
	{
		"Glad",
		"GLFW",
	}

	filter "system:windows"
		systemversion "latest"
        cppdialect "C++17"
        staticruntime "On"
		defines { "RA_WINDOWS" }


	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
		defines { "RA_DEBUG" }

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
