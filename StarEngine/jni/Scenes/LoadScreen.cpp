#include "LoadScreen.h"
#include "../Logger.h"
#include "../Helpers/Filepath.h"
#include "../Assets/TextureManager.h"
#include "../Graphics/GraphicsManager.h"
#include "../Components/TransformComponent.h"
#include "../Objects/FreeCamera.h"
#include "../Components/CameraComponent.h"

namespace star
{
	LoadScreen::LoadScreen(const tstring& filePath,const tstring& spriteName)
		:m_Shader()
		, m_FilePath(filePath)
		, m_SpriteName(spriteName)
	{
	}

	void LoadScreen::Initialize()
	{
#ifdef _WIN32
		tstring vShader(_T("WinShaders/Texture_Shader.vert")),
				fShader(_T("WinShaders/Texture_Shader.frag"));
#else
		tstring vShader(_T("AndroidShaders/BaseTexShader.vert")),
				fShader(_T("AndroidShaders/BaseTexShader.frag"));

#endif
		if(!m_Shader.Init(vShader, fShader))
		{
			Logger::GetInstance()->Log(star::LogLevel::Info, _T("Initialization of the Loadscreen Shader has failed!"));
		}

		TextureManager::GetInstance()->LoadTexture(m_FilePath.GetAssetsPath(),m_SpriteName);
		m_Width = TextureManager::GetInstance()->GetTextureDimensions(m_SpriteName).x;
		m_Heigth =  TextureManager::GetInstance()->GetTextureDimensions(m_SpriteName).y;

		CreateSquare();
	}

	LoadScreen::~LoadScreen()
	{
		TextureManager::GetInstance()->DeleteTexture(m_SpriteName);
	}

	void LoadScreen::CreateSquare()
	{
		m_Vertices[0] = 0.5;
		m_Vertices[1] = 0.5;
		m_Vertices[2] = 0.5;
		m_Vertices[3] = -0.5f;
		m_Vertices[4] = -0.5f;
		m_Vertices[5] = 0.5f;
		m_Vertices[6] = -0.5f;
		m_Vertices[7] = -0.5f;

		 
		m_UvCoords[0] = 1.0f;
		m_UvCoords[1] = 1.0f;
		m_UvCoords[2] = 1.0f;
		m_UvCoords[3] = 0.0f;
		m_UvCoords[4] = 0.0f;
		m_UvCoords[5] = 1.0f;
		m_UvCoords[6] = 0.0f;
		m_UvCoords[7] = 0.0f;
	}

	void LoadScreen::Draw()
	{
		m_Shader.Bind();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, star::TextureManager::GetInstance()->GetTextureID(m_SpriteName));
		GLint s_textureId = glGetUniformLocation(m_Shader.GetID(), "textureSampler");
		glUniform1i(s_textureId, 0);

		//Set attributes and buffers
		glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT,0,0, m_Vertices);
		glEnableVertexAttribArray(ATTRIB_VERTEX);
		glVertexAttribPointer(ATTRIB_UV, 2, GL_FLOAT, 0, 0, m_UvCoords);
		glEnableVertexAttribArray(ATTRIB_UV);

		glDrawArrays(GL_TRIANGLE_STRIP,0,4);

		//Unbind attributes and buffers
		glDisableVertexAttribArray(ATTRIB_VERTEX);
		glDisableVertexAttribArray(ATTRIB_UV);

		m_Shader.Unbind();
	}
}
