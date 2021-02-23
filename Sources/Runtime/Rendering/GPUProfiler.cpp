#include "Rendering/GPUProfiler.h"
#include "Rendering/RendererDX11.h"

namespace Mile
{
   std::mutex ScopedDeferredGPUProfile::m_globalMutex;

   GPUProfiler::GPUProfiler(RendererDX11* renderer) :
      m_renderer(renderer),
      m_currentFrame(0),
      m_queryLatency(30),
      m_latestDrawCalls(0),
      m_latestDrawVertices(0),
      m_latestDrawTriangles(0)
   {
      size_t maximumThraeds = m_renderer->GetMaximumThreads() + 1; // Include Main thread
      m_drawCalls.resize(maximumThraeds);
      m_vertices.resize(maximumThraeds);
      m_triangles.resize(maximumThraeds);
   }

   GPUProfiler::~GPUProfiler()
   {
      m_profiles.clear();
      m_profileTimes.clear();
   }

   void GPUProfiler::Begin(const std::string& name)
   {
      ID3D11DeviceContext& context = m_renderer->GetImmediateContext();
      Begin(name, context);
   }

   void GPUProfiler::End(const std::string& name)
   {
      ID3D11DeviceContext& context = m_renderer->GetImmediateContext();
      End(name, context);
   }

   void GPUProfiler::Begin(const std::string& name, ID3D11DeviceContext& context, bool bIsDeferred)
   {
      auto& data = m_profiles[name];
      if (!data.bIsOnQuery)
      {
         ID3D11Device& device = m_renderer->GetDevice();
         D3D11_QUERY_DESC queryDesc;
         ZeroMemory(&queryDesc, sizeof(D3D11_QUERY_DESC));
         queryDesc.Query = D3D11_QUERY_TIMESTAMP_DISJOINT;
         device.CreateQuery(&queryDesc, &data.Disjoint);

         queryDesc.Query = D3D11_QUERY_TIMESTAMP;
         device.CreateQuery(&queryDesc, &data.Begin);
         device.CreateQuery(&queryDesc, &data.End);

         if (data.Disjoint != nullptr && data.Begin != nullptr)
         {
            context.Begin(data.Disjoint);
            context.End(data.Begin);
            data.QueryBeginFrame = m_currentFrame;
            data.bIsDeferred = bIsDeferred;
            data.bIsOnQuery = true;
         }
         else
         {
            data.Reset();
         }
      }
   }

   void GPUProfiler::End(const std::string& name, ID3D11DeviceContext& context)
   {
      auto& data = m_profiles[name];
      if (data.bIsOnQuery && data.QueryBeginFrame == m_currentFrame)
      {
         context.End(data.End);
         context.End(data.Disjoint);
      }
   }

   void GPUProfiler::EndFrame()
   {
      m_latestDrawCalls = std::accumulate<std::vector<UINT64>::iterator, UINT64>(m_drawCalls.begin(), m_drawCalls.end(), 0);
      m_latestDrawTriangles = std::accumulate<std::vector<UINT64>::iterator, UINT64>(m_vertices.begin(), m_vertices.end(), 0);
      m_latestDrawVertices = std::accumulate<std::vector<UINT64>::iterator, UINT64>(m_triangles.begin(), m_triangles.end(), 0);
      std::fill(m_drawCalls.begin(), m_drawCalls.end(), 0);
      std::fill(m_vertices.begin(), m_vertices.end(), 0);
      std::fill(m_triangles.begin(), m_triangles.end(), 0);
      ++m_currentFrame;

      ID3D11DeviceContext& context = m_renderer->GetImmediateContext();
      for (auto dataItr = m_profiles.begin(); dataItr != m_profiles.end(); ++dataItr)
      {
         auto& data = (*dataItr).second;
         if (data.bIsOnQuery && (data.QueryBeginFrame + m_queryLatency) < m_currentFrame)
         {
            UINT64 beginTime = 0;
            while (context.GetData(data.Begin, &beginTime, sizeof(UINT64), 0) != S_OK);
            UINT64 endTime = 0;
            while (context.GetData(data.End, &endTime, sizeof(UINT64), 0) != S_OK);
            D3D11_QUERY_DATA_TIMESTAMP_DISJOINT disjointData;
            while (context.GetData(data.Disjoint, &disjointData, sizeof(disjointData), 0) != S_OK);
            
            if (!disjointData.Disjoint)
            {
               data.ElapsedTimeHz = endTime - beginTime;

               double frequency = static_cast<double>(disjointData.Frequency);
               data.ElapsedTime = ((data.ElapsedTimeHz / frequency) * 1000.0);
            }

            m_profileTimes[(*dataItr).first] = data.ElapsedTime;
            data.Reset();
         }
         else if (!data.bIsOnQuery)
         {
            m_profileTimes[dataItr->first] = 0.0;
         }
      }
   }
}