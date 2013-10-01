#pragma once
#include <android_native_app_glue.h>
#include "BaseGesture.h"

namespace star
{
	class TapGesture : public BaseGesture
	{
	public:
		TapGesture();
		virtual ~TapGesture();
		virtual void OnTouchEvent(AInputEvent* pEvent);
	protected:

	};
}