/// <copyright file="ShaderProgram.hpp">
/// MIT License.
/// Copyright (c) 2021 Michael Becher.
/// </copyright>
/// <author>Michael Becher</author>

#ifndef ShaderProgram_hpp
#define ShaderProgram_hpp

#include <winrt/base.h> // winrt::check_hresult
#include <wrl/client.h> // Microsoft::WRL::ComPtr

#include "VertexDescriptor.hpp"

namespace dxowl
{
    class ShaderProgram
    {
    public:
        enum ShaderType
        {
            VertexShader,
            ComputeShader,
            GeometryShader,
            PixelShader
        };

        template <typename ShaderFileDataContainer>
        ShaderProgram(
            ID3D11Device4* d3d11_device,
            std::vector<VertexDescriptor> vertex_desc,
            ShaderFileDataContainer vertex_shader,
            ShaderFileDataContainer compute_shader,
            ShaderFileDataContainer geometry_shader,
            ShaderFileDataContainer pixel_shader);
        ShaderProgram(
            ID3D11Device4* d3d11_device,
            std::vector<VertexDescriptor> vertex_desc,
            void const *vertex_shader,
            size_t vertex_shader_byteSize,
            void const* compute_shader,
            size_t compute_shader_byteSize,
            void const *geometry_shader,
            size_t geometry_shader_byteSize,
            void const *pixel_shader,
            size_t pixel_shader_byteSize);
        ~ShaderProgram() = default;

        ShaderProgram(const ShaderProgram &cpy) = delete;
        ShaderProgram(ShaderProgram &&other) = delete;
        ShaderProgram &operator=(ShaderProgram &&rhs) = delete;
        ShaderProgram &operator=(const ShaderProgram &rhs) = delete;

        void setInputLayout(ID3D11DeviceContext4* d3d11_ctx);
        void setVertexShader(ID3D11DeviceContext4* d3d11_ctx);
        void setComputeShader(ID3D11DeviceContext4* d3d11_ctx);
        void setGeometryShader(ID3D11DeviceContext4* d3d11_ctx);
        void setPixelShader(ID3D11DeviceContext4* d3d11_ctx);

    private:
        Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
        Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
        Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_computeShader;
        Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;
        Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
    };

    template <typename ShaderFileDataContainer>
    ShaderProgram::ShaderProgram(
        ID3D11Device4* d3d11_device,
        std::vector<VertexDescriptor> vertex_desc,
        ShaderFileDataContainer vertex_shader,
        ShaderFileDataContainer compute_shader,
        ShaderFileDataContainer geometry_shader,
        ShaderFileDataContainer pixel_shader)
        : m_inputLayout(nullptr), m_vertexShader(nullptr), m_computeShader(nullptr), m_geometryShader(nullptr), m_pixelShader(nullptr)
    {
        winrt::check_hresult(
            d3d11_device->CreateVertexShader(
                vertex_shader.data(),
                vertex_shader.size(),
                nullptr,
                &m_vertexShader));

        std::vector<D3D11_INPUT_ELEMENT_DESC> attributes;
        for (auto& vl : vertex_desc) {
            attributes.insert(std::end(attributes), std::begin(vl.attributes), std::end(vl.attributes));
        }
        winrt::check_hresult(
            d3d11_device->CreateInputLayout(
                attributes.data(),
                static_cast<UINT>(attributes.size()),
                vertex_shader.data(),
                static_cast<UINT>(vertex_shader.size()),
                &m_inputLayout));

        winrt::check_hresult(
            d3d11_device->CreatePixelShader(
                pixel_shader.data(),
                pixel_shader.size(),
                nullptr,
                &m_pixelShader));

        if (compute_shader.size() > 0) // check if data for optional compute shader is given
        {
            winrt::check_hresult(
                d3d11_device->CreateComputeShader(
                    compute_shader.data(),
                    compute_shader.size(),
                    nullptr,
                    &m_computeShader));
        }

        if (geometry_shader.size() > 0) // check if data for optional geometry shader is given
        {
            winrt::check_hresult(
                d3d11_device->CreateGeometryShader(
                    geometry_shader.data(),
                    geometry_shader.size(),
                    nullptr,
                    &m_geometryShader));
        }
    }

    inline ShaderProgram::ShaderProgram(
        ID3D11Device4* d3d11_device,
        std::vector<VertexDescriptor> vertex_desc,
        void const *vertex_shader,
        size_t vertex_shader_byteSize,
        void const* compute_shader,
        size_t compute_shader_byteSize,
        void const *geometry_shader,
        size_t geometry_shader_byteSize,
        void const *pixel_shader,
        size_t pixel_shader_byteSize)
        : m_inputLayout(nullptr), m_vertexShader(nullptr), m_computeShader(nullptr), m_geometryShader(nullptr), m_pixelShader(nullptr)
    {
        winrt::check_hresult(
            d3d11_device->CreateVertexShader(
                vertex_shader,
                vertex_shader_byteSize,
                nullptr,
                &m_vertexShader));

        std::vector<D3D11_INPUT_ELEMENT_DESC> attributes;
        for (auto& vl : vertex_desc) {
            attributes.insert(std::end(attributes), std::begin(vl.attributes), std::end(vl.attributes));
        }
        winrt::check_hresult(
            d3d11_device->CreateInputLayout(
                attributes.data(),
                static_cast<UINT>(attributes.size()),
                vertex_shader,
                static_cast<UINT>(vertex_shader_byteSize),
                &m_inputLayout));

        winrt::check_hresult(
            d3d11_device->CreatePixelShader(
                pixel_shader,
                pixel_shader_byteSize,
                nullptr,
                &m_pixelShader));

        if (compute_shader != nullptr) // check if data for optional compute shader is given
        {
            winrt::check_hresult(
                d3d11_device->CreateComputeShader(
                    compute_shader,
                    compute_shader_byteSize,
                    nullptr,
                    &m_computeShader));
        }

        if (geometry_shader != nullptr) // check if data for optional geometry shader is given
        {
            winrt::check_hresult(
                d3d11_device->CreateGeometryShader(
                    geometry_shader,
                    geometry_shader_byteSize,
                    nullptr,
                    &m_geometryShader));
        }
    }

    inline void ShaderProgram::setInputLayout(ID3D11DeviceContext4* d3d11_ctx)
    {
        d3d11_ctx->IASetInputLayout(m_inputLayout.Get());
    }

    inline void ShaderProgram::setVertexShader(ID3D11DeviceContext4* d3d11_ctx)
    {
        d3d11_ctx->VSSetShader(
            m_vertexShader.Get(),
            nullptr,
            0);
    }

    inline void ShaderProgram::setComputeShader(ID3D11DeviceContext4* d3d11_ctx)
    {
        if (m_computeShader != nullptr)
        {
            d3d11_ctx->CSSetShader(
                m_computeShader.Get(),
                nullptr,
                0);
        }
    }

    inline void ShaderProgram::setGeometryShader(ID3D11DeviceContext4* d3d11_ctx)
    {
        if (m_geometryShader != nullptr)
        {
            d3d11_ctx->GSSetShader(
                m_geometryShader.Get(),
                nullptr,
                0);
        }
    }

    inline void ShaderProgram::setPixelShader(ID3D11DeviceContext4* d3d11_ctx)
    {
        d3d11_ctx->PSSetShader(
            m_pixelShader.Get(),
            nullptr,
            0);
    }
} // namespace dxowl

#endif