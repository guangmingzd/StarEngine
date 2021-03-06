#include "SpriteComponent.h"
#include "../../Logger.h"
#include "../../Assets/TextureManager.h"
#include "../../Graphics/GraphicsManager.h"
#include "../TransformComponent.h"
#include "../../Objects/FreeCamera.h"
#include "../CameraComponent.h"
#include "../../Objects/Object.h"
#include "../../Graphics/SpriteBatch.h"

namespace star
{
	SpriteComponent::SpriteComponent(
		const tstring& filepath,
		const tstring& spriteName,
		int32 widthSegments,
		int32 heightSegments
		)
		: BaseComponent()
		, m_Width(0)
		, m_WidthSegments(widthSegments)
		, m_CurrentWidthSegment(0)
		, m_Heigth(0)
		, m_HeightSegments(heightSegments)
		, m_CurrentHeightSegment(0)
		, m_FilePath(filepath)
		, m_SpriteName(spriteName)
		, m_bIsHudElement(false)
		, m_SpriteInfo()
	{
	}

	void SpriteComponent::InitializeComponent()
	{	
		TextureManager::GetInstance()->LoadTexture(
			m_FilePath.GetAssetsPath(),
			m_SpriteName
			);

		m_Width = TextureManager::GetInstance()->
			GetTextureDimensions(m_SpriteName).x /
			m_WidthSegments;
		m_Heigth =  TextureManager::GetInstance()->
			GetTextureDimensions(m_SpriteName).y /
			m_HeightSegments;

		CreateVertices();
		CreateIndices();

		m_SpriteInfo.spriteName = m_SpriteName;
		m_SpriteInfo.vertices = GetVertices();
		m_SpriteInfo.uvCoords = GetUVCoords();
		m_SpriteInfo.bIsHUD = m_bIsHudElement;
	}

	SpriteComponent::~SpriteComponent()
	{
		// [COMMENT] can you explain me why you wouldn't want
		// to destroy the texture once this spritecomponent
		// is deleted. (you can do that via skype)
		// [COMMENT] @ Simon: We should discuss if RemoveTexture should be called here. 
		// Really depends of the structure of the TextureManager

		//TextureManager::GetInstance()->DeleteTexture(m_SpriteName);
	}

	void SpriteComponent::CreateVertices()
	{
		m_Vertices[0] = (GLfloat)m_Width;
		m_Vertices[1] = (GLfloat)m_Heigth;
		m_Vertices[2] = 0;
		m_Vertices[3] = (GLfloat)m_Width;
		m_Vertices[4] = 0;
		m_Vertices[5] = 0;
		m_Vertices[6] = 0;
		m_Vertices[7] = (GLfloat)m_Heigth;
		m_Vertices[8] = 0;
		m_Vertices[9] = 0;
		m_Vertices[10] = 0;
		m_Vertices[11] = 0;
	}

	void SpriteComponent::CreateIndices()
	{
		float32 startX =
			static_cast<float32>(m_CurrentWidthSegment) /
			static_cast<float32>(m_WidthSegments);
		float32 endX = 1.0f / m_WidthSegments;
		float32 startY =
			static_cast<float32>(m_CurrentHeightSegment) /
			static_cast<float32>(m_HeightSegments);
		float32 endY = 1.0f / m_HeightSegments;

		m_UvCoords[0] = startX + endX;
		m_UvCoords[1] = startY + endY;
		m_UvCoords[2] = startX + endX;
		m_UvCoords[3] = startY;
		m_UvCoords[4] = startX;
		m_UvCoords[5] = startY + endY;
		m_UvCoords[6] = startX;
		m_UvCoords[7] = startY;
	}

	void SpriteComponent::Draw()
	{
		m_SpriteInfo.transform = GetTransform()->GetWorldMatrix();
		m_SpriteInfo.uvCoords = GetUVCoords();
		SpriteBatch::GetInstance()->AddSpriteToQueue(m_SpriteInfo, m_bIsHudElement);
	}

	const tstring& SpriteComponent::GetFilePath() const
	{
		return m_FilePath.GetPath();
	}

	const tstring& SpriteComponent::GetName() const
	{
		return m_SpriteName;
	}

	int32 SpriteComponent::GetWidth() const
	{
		return int32(m_Width / m_WidthSegments);
	}

	int32 SpriteComponent::GetHeight() const
	{
		return int32(m_Heigth / m_HeightSegments); 
	}

	std::vector<GLfloat> SpriteComponent::GetVertices() const
	{
		std::vector<GLfloat> vertices;
		vertices.clear();
		
		for(int32 i = 0; i < 12; ++i)
		{
			vertices.push_back(m_Vertices[i]);
		}
		return vertices;
	}

	std::vector<GLfloat> SpriteComponent::GetUVCoords() const
	{
		std::vector<GLfloat> uvCoords;
		uvCoords.clear();
		
		for(int32 i = 0; i < 8; ++i)
		{
			uvCoords.push_back(m_UvCoords[i]);
		}
		return uvCoords;
	}

	void SpriteComponent::SetCurrentSegment(int32 widthSegment, int32 heightSegment)
	{
		m_CurrentWidthSegment = widthSegment;
		m_CurrentHeightSegment = m_HeightSegments - heightSegment - 1;

		CreateIndices();
	}

	void SpriteComponent::SetHUDOptionEnabled(bool enabled)
	{
		m_bIsHudElement = enabled;
	}

	bool SpriteComponent::IsHUDOptionEnabled() const
	{
		return m_bIsHudElement;
	}

	void SpriteComponent::SetTexture(
		const tstring& filepath,
		const tstring& spriteName,
		int32 widthSegments,
		int32 heightSegments
		)
	{
		m_Width = 0;
		m_WidthSegments = widthSegments;
		m_CurrentWidthSegment = 0;
		m_Heigth = 0;
		m_HeightSegments = heightSegments;
		m_CurrentHeightSegment = 0;
		m_FilePath = filepath;
		m_SpriteName = spriteName;

		TextureManager::GetInstance()->LoadTexture(m_FilePath.GetAssetsPath(),m_SpriteName);
		m_Width = TextureManager::GetInstance()->GetTextureDimensions(m_SpriteName).x / m_WidthSegments;
		m_Heigth =  TextureManager::GetInstance()->GetTextureDimensions(m_SpriteName).y / m_HeightSegments;

		CreateVertices();
		CreateIndices();

		m_SpriteInfo.spriteName = m_SpriteName;
		m_SpriteInfo.vertices = GetVertices();
		m_SpriteInfo.uvCoords = GetUVCoords();
		m_SpriteInfo.bIsHUD = m_bIsHudElement;
	}

}
