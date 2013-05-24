#pragma once

#include <vector>

class CDataObject :
	public IDataObject
{
public:
	CDataObject()
	{

	}

	
	STDMETHOD_(ULONG, AddRef)()
	{
		return 1;
	}

	STDMETHOD_(ULONG, Release)()
	{
		return 1;
	}

	STDMETHOD(QueryInterface)(REFIID riid, void ** ppvObj)
	{
		assert(ppvObj);
		if (ppvObj == NULL)
		{
			return E_POINTER;
		}

		*ppvObj = NULL;
		HRESULT hr = S_OK;

		if ( IsEqualIID(riid, __uuidof(IUnknown)) )
		{
			*ppvObj = this;
			this->AddRef();
		}
		else if ( IsEqualIID(riid, __uuidof(IDataObject)) )
		{
			*ppvObj = this;
			this->AddRef();
		}
		else
		{
			hr = E_NOINTERFACE;
		}

		return hr;
	}

protected:

	std::vector<FORMATETC> m_vtFtmEtc;
	std::vector<STGMEDIUM> m_vtStgMedium;

protected:

	LONG LookupFormatEtc(FORMATETC *pFormatEtc);
	HRESULT AddRefStgMedium(STGMEDIUM* pstgmIn, STGMEDIUM* pstgmOut, BOOL fCopyIn );

public:

	STDMETHOD(GetData)(FORMATETC *pFormatEtc, STGMEDIUM *pmedium);
	STDMETHOD(GetDataHere)(FORMATETC *pFormatEtc, STGMEDIUM *pmedium);
	STDMETHOD(QueryGetData)(FORMATETC *pFormatEtc);
	STDMETHOD(GetCanonicalFormatEtc)(FORMATETC *pFormatEct, FORMATETC *pFormatEtcOut);
	STDMETHOD(SetData)(FORMATETC *pFormatEtc, STGMEDIUM *pMedium,  BOOL fRelease);
	STDMETHOD(EnumFormatEtc)(DWORD dwDirection, IEnumFORMATETC **ppEnumFormatEtc);
	STDMETHOD(DAdvise)(FORMATETC *pFormatEtc, DWORD advf, IAdviseSink *, DWORD *);
	STDMETHOD(DUnadvise)(DWORD dwConnection);
	STDMETHOD(EnumDAdvise)(IEnumSTATDATA **ppEnumAdvise);
};

class CEnumFormatEtc : public IEnumFORMATETC
{
public:
	// IUnknown members
	STDMETHOD_(ULONG, AddRef) (void)
	{
		m_lRefCount++;
		return m_lRefCount;
	}

	STDMETHOD_(ULONG, Release) (void)
	{
		--m_lRefCount;
		long lRefCount = m_lRefCount;
		if ( lRefCount == 0 )
		{
			delete this;
		}

		return lRefCount;
	}

	STDMETHOD(QueryInterface) (REFIID iid, void** ppvObject)
	{
		if (iid == IID_IUnknown || iid == IID_IEnumFORMATETC)
		{
			*ppvObject = this;
			AddRef();
			return S_OK;
		}
		else
			return E_NOINTERFACE;
	}

	// IEnumFormatEtc members

	HRESULT __stdcall  Next  (ULONG celt, FORMATETC * rgelt, ULONG * pceltFetched);
	HRESULT __stdcall  Skip  (ULONG celt); 
	HRESULT __stdcall  Reset (void);
	HRESULT __stdcall  Clone (IEnumFORMATETC ** ppEnumFormatEtc);

	// Construction / Destruction

	CEnumFormatEtc(FORMATETC *pFormatEtc, int nNumFormats);
	~CEnumFormatEtc();
private:
	LONG        m_lRefCount;        // Reference count for this COM interface
	ULONG       m_nIndex;           // current enumerator index
	ULONG       m_nNumFormats;      // number of FORMATETC members
	FORMATETC * m_pFormatEtc;       // array of FORMATETC objects
};
