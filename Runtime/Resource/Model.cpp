#include "Resource/Model.h"
#include "Resource/ModelLoader.h"
#include "Rendering/Mesh.h"
#include "Core/Logger.h"
#include "GameFramework/Entity.h"
#include "GameFramework/World.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace Mile
{
   Model::Model(Context* context, const String& filePath) :
      m_instance(nullptr),
      Resource(context, filePath, ResourceType::Model)
   {
   }

   Model::~Model()
   {
      for (auto mesh : m_meshes)
      {
         SafeDelete(mesh);
      }
   }

   bool Model::Init()
   {
      if (m_context == nullptr || m_instance != nullptr)
      {
         return false;
      }

      m_instance = ModelLoader::LoadModel(m_context, this, m_path);
      if (m_instance == nullptr)
      {
         MELog(m_context, TEXT("Model"), ELogType::WARNING, TEXT("Failed to load model from ") + m_path, true);
         return false;
      }

      m_serializedInstance = m_instance->Serialize().dump();
      return true;
   }

   void Model::AddMesh(Mesh* mesh)
   {
      m_meshes.push_back(mesh);
   }

   Mesh* Model::GetMeshByName(const std::wstring& name)
   {
      for (auto mesh : m_meshes)
      {
         if (mesh->GetName() == name)
         {
            return mesh;
         }
      }

      return nullptr;
   }

   Entity* Model::Instantiate(Model* target, World* targetWorld, const String& entityName)
   {
      Entity* tempEntity = targetWorld->CreateEntity(TEXT(""));
      tempEntity->DeSerialize(json::parse(target->m_serializedInstance));
      tempEntity->SetName(entityName);
      return tempEntity;
   }
}