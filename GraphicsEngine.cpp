#include "GraphicsEngine.h"
#include "AppWindow.h"
#include "SwapChain.h"
#include "DeviceContext.h"

GraphicsEngine::GraphicsEngine() :
	m_d3d_device(nullptr),
	m_feature_level(D3D_FEATURE_LEVEL_11_0),
	m_imm_context(nullptr)
{

}
bool GraphicsEngine::init()
{
    D3D_DRIVER_TYPE driver_types[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE
    };
    UINT num_driver_type = ARRAYSIZE(driver_types);
    D3D_FEATURE_LEVEL feature_levels[] =
    {
        D3D_FEATURE_LEVEL_11_0
    };
    UINT num_feature_levels = ARRAYSIZE(feature_levels);
    HRESULT res = 0;
    
    for (UINT driver_type_index = 0; driver_type_index < num_driver_type;)
    {
        res = D3D11CreateDevice( nullptr, driver_types[driver_type_index], nullptr, 0, feature_levels,num_feature_levels, 
            D3D11_SDK_VERSION, &m_d3d_device, &m_feature_level, &m_imm_context);

        if (SUCCEEDED(res))
            break;
            ++driver_type_index;
    }   

    if (FAILED(res))
        return false;

    m_imm_device_context = new DeviceContext(m_imm_context);

    m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgi_device);
    m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgi_adapter);
    m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgi_factory);   

    return true;
}
bool GraphicsEngine::release()
{
    m_dxgi_device->Release();
    m_dxgi_adapter->Release();
    m_dxgi_factory->Release();

    m_imm_context->Release();
    m_imm_device_context->release();

    m_d3d_device->Release();
	return true;
}
GraphicsEngine::~GraphicsEngine()
{

}

SwapChain* GraphicsEngine::createSwapChain()
{
    SwapChain* swapChain = new SwapChain();
    return swapChain;
}

GraphicsEngine* GraphicsEngine::get()
{
    static GraphicsEngine engine;
    return &engine;
}

DeviceContext* GraphicsEngine::getImmeadiateDeviceContext()
{
    return this->m_imm_device_context;
}
