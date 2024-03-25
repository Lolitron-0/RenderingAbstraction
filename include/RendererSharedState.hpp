#pragma once

namespace Ra 
{
	class RendererSharedState
	{
		
        RA_ASSERT(s_RendererAPI == RendererAPI::API::None,
                  "Rendering API can only be set once!");
		private:
		RendererAPI::API m_RendererApi;
	};
}
