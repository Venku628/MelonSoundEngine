#pragma once

#include "MelonSoundEngine.h"

#ifdef MELONSOUNDENGINE_EXPORTS  
#define MELONSOUNDENGINE_API __declspec(dllexport)   
#else  
#define MELONSOUNDENGINE_API __declspec(dllimport)   
#endif  

namespace MelonSoundEngine
{
	// This class is exported from the MelonSoundEngine.dll
	class Functions
	{
	public:

		static MELONSOUNDENGINE_API void Function1();

		static MELONSOUNDENGINE_API void Function2();
	};
}
