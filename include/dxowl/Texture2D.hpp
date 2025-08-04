/// <copyright file="Texture2D.hpp">
/// MIT License.
/// Copyright (c) 2021 Michael Becher.
/// </copyright>
/// <author>Michael Becher</author>

#ifndef Texture2D_hpp
#define Texture2D_hpp

#include "VertexDescriptor.hpp"

namespace dxowl
{
    class Texture2D
    {
    public:
        template <typename TexelDataContainer>
        Texture2D(
            ID3D11Device4* d3d11_device,
            TexelDataContainer const &data,
            D3D11_TEXTURE2D_DESC const &desc,
            D3D11_SHADER_RESOURCE_VIEW_DESC const &shdr_rsrc_view,
            bool generate_mipmap = false);

        template <typename TexelDataContainer>
        Texture2D(
            ID3D11Device4* d3d11_device,
            TexelDataContainer const& data,
            D3D11_TEXTURE2D_DESC const& desc,
            D3D11_UNORDERED_ACCESS_VIEW_DESC const& unord_acc_view_desc,
            D3D11_SHADER_RESOURCE_VIEW_DESC const* shdr_rsrc_view = nullptr,
            bool generate_mipmap = false);

        template <typename TexelDataPtr>
        Texture2D(
            ID3D11Device4* d3d11_device,
            std::vector<TexelDataPtr> const &data,
            D3D11_TEXTURE2D_DESC const &desc,
            D3D11_SHADER_RESOURCE_VIEW_DESC const &shdr_rsrc_view,
            bool generate_mipmap = false);

        template <typename TexelDataPtr>
        Texture2D(
            ID3D11Device4* d3d11_device,
            std::vector<TexelDataPtr> const &data,
            D3D11_TEXTURE2D_DESC const &desc,
            D3D11_UNORDERED_ACCESS_VIEW_DESC const &unord_acc_view,
            D3D11_SHADER_RESOURCE_VIEW_DESC const *shdr_rsrc_view = nullptr,
            bool generate_mipmap = false);

        ~Texture2D(){}; //TODO

        void resize(ID3D11Device4* d3d11_device, UINT width, UINT height);

        inline D3D11_TEXTURE2D_DESC getTextureDesc() const
        {
            return m_desc;
        }

        inline Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> getShaderResourceView() const
        {
            return m_shdr_rsrc_view;
        }

        inline Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> getUnorderedAccessView() const
        {
            return m_unord_acc_view;
        }

        inline Microsoft::WRL::ComPtr<ID3D11Texture2D> getTexture() const {
            return m_texture;
        }

    protected:
        typedef Microsoft::WRL::ComPtr<ID3D11Texture2D> TexturePtr;
        typedef Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ShaderResourceViewPtr;
        typedef Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> UnorderedAccessViewPtr;

        D3D11_TEXTURE2D_DESC m_desc;
        D3D11_SHADER_RESOURCE_VIEW_DESC m_shdr_rsrc_view_desc;
        D3D11_UNORDERED_ACCESS_VIEW_DESC m_unord_acc_view_desc;

        TexturePtr m_texture;
        ShaderResourceViewPtr m_shdr_rsrc_view;
        UnorderedAccessViewPtr m_unord_acc_view;
    };

    template <typename TexelDataContainer>
    inline Texture2D::Texture2D(
        ID3D11Device4* d3d11_device,
        TexelDataContainer const &data,
        D3D11_TEXTURE2D_DESC const &desc,
        D3D11_SHADER_RESOURCE_VIEW_DESC const &shdr_rsrc_view,
        bool generate_mipmap)
        : Texture2D(
              d3d11_device,
              {data.data()},
              desc,
              shdr_rsrc_view,
              generate_mipmap)
    {
    }

    template <typename TexelDataContainer>
    inline Texture2D::Texture2D(
        ID3D11Device4* d3d11_device,
        TexelDataContainer const &data,
        D3D11_TEXTURE2D_DESC const &desc,
        D3D11_UNORDERED_ACCESS_VIEW_DESC const &unord_acc_view_desc,
        D3D11_SHADER_RESOURCE_VIEW_DESC const *shdr_rsrc_view,
        bool generate_mipmap)
        : Texture2D(
              d3d11_device,
              {data.data()},
              desc,
              unord_acc_view_desc,
              shdr_rsrc_view,
              generate_mipmap)
    {
    }

    template <typename TexelDataPtr>
    inline Texture2D::Texture2D(
        ID3D11Device4* d3d11_device,
        std::vector<TexelDataPtr> const &data,
        D3D11_TEXTURE2D_DESC const &desc,
        D3D11_SHADER_RESOURCE_VIEW_DESC const &shdr_rsrc_view,
        bool generate_mipmap)
        : m_desc(desc), m_shdr_rsrc_view_desc(shdr_rsrc_view)
    {
        std::vector<D3D11_SUBRESOURCE_DATA> pData(data.size());

        for (size_t i = 0; i < data.size(); ++i)
        {
            ZeroMemory(&pData[i], sizeof(D3D11_SUBRESOURCE_DATA));

            pData[i].pSysMem = data[i];
            pData[i].SysMemPitch = desc.Width * static_cast<UINT>(computeByteSize(desc.Format));
            pData[i].SysMemSlicePitch = 0;
        }

        HRESULT hr = d3d11_device->CreateTexture2D(
            &m_desc,
            pData.size() > 0 ? pData.data() : nullptr,
            m_texture.GetAddressOf());

        hr = d3d11_device->CreateShaderResourceView(
            m_texture.Get(),
            &m_shdr_rsrc_view_desc,
            m_shdr_rsrc_view.GetAddressOf());

        if (generate_mipmap) {
            Microsoft::WRL::ComPtr<ID3D11DeviceContext> ctx;
            d3d11_device->GetImmediateContext(ctx.GetAddressOf());

            // generate mipmap if requested using device context
            ctx->GenerateMips(m_shdr_rsrc_view.Get());
        }

        //TODO do something with hr
    }

    template <typename TexelDataPtr>
    inline Texture2D::Texture2D(
        ID3D11Device4* d3d11_device,
        std::vector<TexelDataPtr> const &data,
        D3D11_TEXTURE2D_DESC const &desc,
        D3D11_UNORDERED_ACCESS_VIEW_DESC const &unord_acc_view,
        D3D11_SHADER_RESOURCE_VIEW_DESC const *shdr_rsrc_view,
        bool generate_mipmap)
        : m_desc(desc), m_unord_acc_view_desc(unord_acc_view)
    {
        std::vector<D3D11_SUBRESOURCE_DATA> pData(data.size());

        for (size_t i = 0; i < data.size(); ++i)
        {
            ZeroMemory(&pData[i], sizeof(D3D11_SUBRESOURCE_DATA));

            pData[i].pSysMem = data[i];
            pData[i].SysMemPitch = desc.Width * static_cast<UINT>(computeByteSize(desc.Format));
            pData[i].SysMemSlicePitch = 0;
        }

        HRESULT hr = d3d11_device->CreateTexture2D(
            &m_desc,
            pData.size() > 0 ? pData.data() : nullptr,
            m_texture.GetAddressOf());

        hr = d3d11_device->CreateUnorderedAccessView(
            m_texture.Get(),
            &m_unord_acc_view_desc,
            m_unord_acc_view.GetAddressOf());

        if (desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
        {
            if (!shdr_rsrc_view)
            {
                D3D11_SHADER_RESOURCE_VIEW_DESC shdr_rsrc_view_desc;

                ZeroMemory(&shdr_rsrc_view_desc, sizeof(shdr_rsrc_view_desc));

                shdr_rsrc_view_desc.Format = desc.Format;
                shdr_rsrc_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                shdr_rsrc_view_desc.Texture2D.MostDetailedMip = 0;
                shdr_rsrc_view_desc.Texture2D.MipLevels = desc.MipLevels;

                shdr_rsrc_view = &shdr_rsrc_view_desc;
            }

            m_shdr_rsrc_view_desc = *shdr_rsrc_view;

            hr = d3d11_device->CreateShaderResourceView(
                m_texture.Get(),
                &m_shdr_rsrc_view_desc,
                m_shdr_rsrc_view.GetAddressOf());

            if (generate_mipmap) {
                Microsoft::WRL::ComPtr<ID3D11DeviceContext> ctx;
                d3d11_device->GetImmediateContext(ctx.GetAddressOf());

                // generate mipmap if requested using device context
                ctx->GenerateMips(m_shdr_rsrc_view.Get());
            }
        }
    }

    inline void Texture2D::resize(ID3D11Device4* d3d11_device, UINT width, UINT height)
    {
        m_shdr_rsrc_view = nullptr;
        m_unord_acc_view = nullptr;
        m_texture = nullptr;

        m_shdr_rsrc_view.Reset();
        m_unord_acc_view.Reset();
        m_texture.Reset();

        m_desc.Width = width;
        m_desc.Height = height;

        HRESULT hr = d3d11_device->CreateTexture2D(
            &m_desc,
            nullptr,
            m_texture.GetAddressOf()
        );

        if (m_desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
        {
            hr = d3d11_device->CreateShaderResourceView(
                m_texture.Get(),
                &m_shdr_rsrc_view_desc,
                m_shdr_rsrc_view.GetAddressOf());

            if (m_desc.MipLevels > 1) {
                Microsoft::WRL::ComPtr<ID3D11DeviceContext> ctx;
                d3d11_device->GetImmediateContext(ctx.GetAddressOf());

                // generate mipmap if requested using device context
                ctx->GenerateMips(m_shdr_rsrc_view.Get());
            }
        }

        if (m_desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
        {
            hr = d3d11_device->CreateUnorderedAccessView(
                m_texture.Get(),
                &m_unord_acc_view_desc,
                m_unord_acc_view.GetAddressOf());
        }
    }
} // namespace dxowl

#endif