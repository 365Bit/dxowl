/// <copyright file="Texture2DView.hpp">
/// MIT License.
/// Copyright (c) 2025 Michael Becher.
/// </copyright>

#ifndef Texture2DView_hpp
#define Texture2DView_hpp

namespace dxowl
{
    class Texture2DView
    {
    public:
        Texture2DView(
            ID3D11Device4* d3d11_device,
            ID3D11Texture2D* texture,
            D3D11_UNORDERED_ACCESS_VIEW_DESC const& unord_acc_view_desc);

        ~Texture2DView(){}; //TODO

        void resize(ID3D11Device4* d3d11_device, ID3D11Texture2D* texture);

        inline Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> getUnorderedAccessView() const
        {
            return m_unord_acc_view;
        }

    protected:
        D3D11_UNORDERED_ACCESS_VIEW_DESC m_unord_acc_view_desc;
        Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_unord_acc_view;
    };

    inline Texture2DView::Texture2DView(
        ID3D11Device4* d3d11_device,
        ID3D11Texture2D* texture,
        D3D11_UNORDERED_ACCESS_VIEW_DESC const& unord_acc_view_desc)
        : m_unord_acc_view_desc(unord_acc_view_desc)
    {
        HRESULT hr = d3d11_device->CreateUnorderedAccessView(
            texture,
            &unord_acc_view_desc,
            m_unord_acc_view.GetAddressOf());
    }

    inline void Texture2DView::resize(ID3D11Device4* d3d11_device, ID3D11Texture2D* texture)
    {
        m_unord_acc_view = nullptr;
        m_unord_acc_view.Reset();

        HRESULT hr = d3d11_device->CreateUnorderedAccessView(
            texture,
            &m_unord_acc_view_desc,
            m_unord_acc_view.GetAddressOf());
    }
} // namespace dxowl

#endif