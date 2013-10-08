#pragma once

#include "../defines.h"
#include "../Logger.h"

#ifdef _WIN32
#pragma warning (disable : 4172)
#pragma warning (disable : 4099)
#include <png.h>
#pragma warning (default : 4172)
#pragma warning (default : 4099)
#include <glew.h>
#else
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <png.h>
#include <android_native_app_glue.h>
#include "Resource.h"
#endif


namespace star
{
	class Texture2D
	{
	public:
#ifdef _WIN32
		Texture2D(tstring pPath);
#else
		Texture2D(tstring pPath,android_app* pApplication );
#endif
		~Texture2D();

		const tstring getPath() const;
		const int32 getHeight() const;
		const int32 getWidth() const;

		const GLuint getTextureID(){return mTextureId;}

	private:
		uint8* ReadPNG();
		status Load();

	private:
		tstring mPath;

		GLuint	mTextureId;	
		GLint	mFormat;

		int32 mWidth, mHeight;
#ifndef _WIN32
		Resource mResource;
		static void Callback_Read(png_structp png, png_bytep data, png_size_t size);
#endif
		Texture2D(const Texture2D& yRef);
		Texture2D(Texture2D&& yRef);
		Texture2D& operator=(const Texture2D& yRef);
	};
}