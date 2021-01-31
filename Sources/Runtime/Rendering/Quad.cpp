#include "Rendering/Quad.h"
#include "Math/Vector4.h"
#include "Math/Vertex.h"

namespace Mile
{
   bool Quad::Init(float minX, float minY,
      float maxX, float maxY)
   {
      std::vector<VertexPTN> vertices = {
      { Vector4(minX, minY, 0.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f) }, // front face
      { Vector4(minX, maxY, 0.0f, 1.0f), Vector2(0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f) },
      { Vector4(maxX, maxY, 0.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f) },
      { Vector4(maxX, minY, 0.0f, 1.0f), Vector2(1.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f) } };

      std::vector<unsigned int> indices = {
         0, 1, 2,
         0, 2, 3
      };

      return Mesh::Init<VertexPTN>(vertices, indices);
   }
}