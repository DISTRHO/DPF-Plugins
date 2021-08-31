/////////////////////////////////////////////////////////////////////////////
//
// projectM-wmp.h : Declaration of the CProjectMwmp
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __PROJECTMWMP_H_
#define __PROJECTMWMP_H_

#include "resource.h"
#include "effects.h"
#include "projectM-wmp_h.h"
#include "projectM.hpp"


/////////////////////////////////////////////////////////////////////////////
// CProjectMwmp
class ATL_NO_VTABLE CProjectMwmp : 
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CProjectMwmp, &CLSID_ProjectMwmp>,
    public IDispatchImpl<IProjectMwmp, &IID_IProjectMwmp, &LIBID_PROJECTMWMPLib>,
    public IWMPEvents,
    public IWMPEffects2
{
private:
    COLORREF    m_clrForeground;    // foreground color
    LONG        m_nPreset;          // current preset

    HRESULT WzToColor(const WCHAR *pwszColor, COLORREF *pcrColor);
    HRESULT ColorToWz( BSTR* pbstrColor, COLORREF crColor);
    DWORD SwapBytes(DWORD dwRet);

public:
    CProjectMwmp();
    ~CProjectMwmp();

DECLARE_REGISTRY_RESOURCEID(IDR_PROJECTMWMP)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CProjectMwmp)
    COM_INTERFACE_ENTRY(IProjectMwmp)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(IWMPEvents)
    COM_INTERFACE_ENTRY(IWMPEffects)
    COM_INTERFACE_ENTRY(IWMPEffects2)
END_COM_MAP()

public:

    // CComCoClass Overrides
    HRESULT FinalConstruct();
    void FinalRelease();

    // IProjectMwmp
    STDMETHOD(get_foregroundColor)(/*[out, retval]*/ BSTR *pVal);
    STDMETHOD(put_foregroundColor)(/*[in]*/ BSTR newVal);

    // IWMPEffects
    STDMETHOD(Render)(TimedLevel *pLevels, HDC hdc, RECT *rc);
    STDMETHOD(MediaInfo)(LONG lChannelCount, LONG lSampleRate, BSTR bstrTitle);
    STDMETHOD(GetCapabilities)(DWORD * pdwCapabilities);
    STDMETHOD(GoFullscreen)(BOOL fFullScreen);// { return E_NOTIMPL; };
    STDMETHOD(RenderFullScreen)(TimedLevel *pLevels);// { return E_NOTIMPL; };
    STDMETHOD(DisplayPropertyPage)(HWND hwndOwner) { return E_NOTIMPL; };
    STDMETHOD(GetTitle)(BSTR *bstrTitle);
    STDMETHOD(GetPresetTitle)(LONG nPreset, BSTR *bstrPresetTitle);
    STDMETHOD(GetPresetCount)(LONG *pnPresetCount);
    STDMETHOD(SetCurrentPreset)(LONG nPreset);
    STDMETHOD(GetCurrentPreset)(LONG *pnPreset);

    // IWMPEffects2
    STDMETHOD(SetCore)(IWMPCore * pCore);
    STDMETHOD(Create)(HWND hwndParent);
    STDMETHOD(Destroy)();
    STDMETHOD(NotifyNewMedia)(IWMPMedia *pMedia);
    STDMETHOD(OnWindowMessage)(UINT msg, WPARAM WParam, LPARAM LParam, LRESULT *plResultParam );
    STDMETHOD(RenderWindowed)(TimedLevel *pLevels, BOOL fRequiredRender );

    // IWMPEvents methods
    void STDMETHODCALLTYPE OpenStateChange( long NewState );
    void STDMETHODCALLTYPE PlayStateChange( long NewState );
    void STDMETHODCALLTYPE AudioLanguageChange( long LangID );
    void STDMETHODCALLTYPE StatusChange();
    void STDMETHODCALLTYPE ScriptCommand( BSTR scType, BSTR Param );
    void STDMETHODCALLTYPE NewStream();
    void STDMETHODCALLTYPE Disconnect( long Result );
    void STDMETHODCALLTYPE Buffering( VARIANT_BOOL Start );
    void STDMETHODCALLTYPE Error();
    void STDMETHODCALLTYPE Warning( long WarningType, long Param, BSTR Description );
    void STDMETHODCALLTYPE EndOfStream( long Result );
    void STDMETHODCALLTYPE PositionChange( double oldPosition, double newPosition);
    void STDMETHODCALLTYPE MarkerHit( long MarkerNum );
    void STDMETHODCALLTYPE DurationUnitChange( long NewDurationUnit );
    void STDMETHODCALLTYPE CdromMediaChange( long CdromNum );
    void STDMETHODCALLTYPE PlaylistChange( IDispatch * Playlist, WMPPlaylistChangeEventType change );
    void STDMETHODCALLTYPE CurrentPlaylistChange( WMPPlaylistChangeEventType change );
    void STDMETHODCALLTYPE CurrentPlaylistItemAvailable( BSTR bstrItemName );
    void STDMETHODCALLTYPE MediaChange( IDispatch * Item );
    void STDMETHODCALLTYPE CurrentMediaItemAvailable( BSTR bstrItemName );
    void STDMETHODCALLTYPE CurrentItemChange( IDispatch *pdispMedia);
    void STDMETHODCALLTYPE MediaCollectionChange();
    void STDMETHODCALLTYPE MediaCollectionAttributeStringAdded( BSTR bstrAttribName,  BSTR bstrAttribVal );
    void STDMETHODCALLTYPE MediaCollectionAttributeStringRemoved( BSTR bstrAttribName,  BSTR bstrAttribVal );
    void STDMETHODCALLTYPE MediaCollectionAttributeStringChanged( BSTR bstrAttribName, BSTR bstrOldAttribVal, BSTR bstrNewAttribVal);
    void STDMETHODCALLTYPE PlaylistCollectionChange();
    void STDMETHODCALLTYPE PlaylistCollectionPlaylistAdded( BSTR bstrPlaylistName);
    void STDMETHODCALLTYPE PlaylistCollectionPlaylistRemoved( BSTR bstrPlaylistName);
    void STDMETHODCALLTYPE PlaylistCollectionPlaylistSetAsDeleted( BSTR bstrPlaylistName, VARIANT_BOOL varfIsDeleted);
    void STDMETHODCALLTYPE ModeChange( BSTR ModeName, VARIANT_BOOL NewValue);
    void STDMETHODCALLTYPE MediaError( IDispatch * pMediaObject);
    void STDMETHODCALLTYPE OpenPlaylistSwitch( IDispatch *pItem );
    void STDMETHODCALLTYPE DomainChange( BSTR strDomain);
    void STDMETHODCALLTYPE SwitchedToPlayerApplication();
    void STDMETHODCALLTYPE SwitchedToControl();
    void STDMETHODCALLTYPE PlayerDockedStateChange();
    void STDMETHODCALLTYPE PlayerReconnect();
    void STDMETHODCALLTYPE Click( short nButton, short nShiftState, long fX, long fY );
    void STDMETHODCALLTYPE DoubleClick( short nButton, short nShiftState, long fX, long fY );
    void STDMETHODCALLTYPE KeyDown( short nKeyCode, short nShiftState );
    void STDMETHODCALLTYPE KeyPress( short nKeyAscii );
    void STDMETHODCALLTYPE KeyUp( short nKeyCode, short nShiftState );
    void STDMETHODCALLTYPE MouseDown( short nButton, short nShiftState, long fX, long fY );
    void STDMETHODCALLTYPE MouseMove( short nButton, short nShiftState, long fX, long fY );
    void STDMETHODCALLTYPE MouseUp( short nButton, short nShiftState, long fX, long fY );

private:
    void         ReleaseCore();

    HWND                        m_hwndParent;
    CComPtr<IWMPCore>           m_spCore;
    CComPtr<IConnectionPoint>   m_spConnectionPoint;
    DWORD                       m_dwAdviseCookie;
};

#endif //__PROJECTMWMP_H_
