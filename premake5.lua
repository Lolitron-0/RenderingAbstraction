project "RenderAbstraction"
	kind "StaticLib"
	language "C++"
    staticruntime "Off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "rapch.h"
	pchsource "src/rapch.cpp"

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
		"%{IncludeDir.GLFW}"
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

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
