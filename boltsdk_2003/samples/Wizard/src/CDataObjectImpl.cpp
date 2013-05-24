#include "stdafx.h"
#include "./CDataObjectImpl.h"

void DeepCopyFormatEtc(FORMATETC *dest, FORMATETC *source)
{
	// copy the source FORMATETC into dest
	*dest = *source;
	if(source->ptd)
	{
		// allocate memory for the DVTARGETDEVICE if necessary
		dest->ptd = (DVTARGETDEVICE*)CoTaskMemAlloc(sizeof(DVTARGETDEVICE));

		// copy the contents of the source DVTARGETDEVICE into dest->ptd
		*(dest->ptd) = *(source->ptd);
	}
}

CEnumFormatEtc::CEnumFormatEtc(FORMATETC *pFormatEtc, int nNumFormats)
{
	m_lRefCount   = 1;
	m_nIndex      = 0;
	m_nNumFormats = nNumFormats;
	m_pFormatEtc  = new FORMATETC[nNumFormats];

	// make a new copy of each FORMATETC structure

	for(int i = 0; i < nNumFormats; i++)
	{
		DeepCopyFormatEtc(&m_pFormatEtc[i], &pFormatEtc[i]);
	}
}

CEnumFormatEtc::~CEnumFormatEtc()
{
	// first free any DVTARGETDEVICE structures
	for(ULONG i = 0; i < m_nNumFormats; i++)
	{
		if(m_pFormatEtc[i].ptd)
			CoTaskMemFree(m_pFormatEtc[i].ptd);
	}
	// now free the main array
	delete[] m_pFormatEtc;
}


HRESULT CreateEnumFormatEtc(UINT cfmt, FORMATETC *afmt, IEnumFORMATETC **ppEnumFormatEtc)
{
	if (cfmt == 0 || afmt == 0 || ppEnumFormatEtc == 0)
		return E_INVALIDARG;
	*ppEnumFormatEtc = new CEnumFormatEtc (afmt, cfmt);
	return (*ppEnumFormatEtc) ? S_OK: E_OUTOFMEMORY;
}

HRESULT CEnumFormatEtc::Reset(void)
{
	m_nIndex = 0;
	return S_OK;
}

HRESULT CEnumFormatEtc::Skip(ULONG celt)
{
	m_nIndex += celt;
	return (m_nIndex <= m_nNumFormats) ? S_OK : S_FALSE;
}

HRESULT CEnumFormatEtc::Clone(IEnumFORMATETC **ppEnumFormatEtc)
{
	HRESULT hResult;
	// make a duplicate enumerator
	hResult = CreateEnumFormatEtc(m_nNumFormats, m_pFormatEtc, ppEnumFormatEtc);
	if(hResult == S_OK)
	{
		// manually set the index state
		((CEnumFormatEtc *)*ppEnumFormatEtc)->m_nIndex = m_nIndex;
	}
	return hResult;
}

HRESULT CEnumFormatEtc::Next(ULONG celt, FORMATETC *pFormatEtc, ULONG *pceltFetched)
{
	ULONG copied = 0;
	// copy the FORMATETC structures into the caller's buffer
	while (m_nIndex < m_nNumFormats && copied < celt) 
	{
		DeepCopyFormatEtc(&pFormatEtc [copied], &m_pFormatEtc [m_nIndex]);
		copied++;
		m_nIndex++;
	}
	// store result
	if(pceltFetched != 0) 
		*pceltFetched = copied;
	// did we copy all that was requested?
	return (copied == celt) ? S_OK : S_FALSE;
}

STDMETHODIMP CDataObject::QueryGetData(FORMATETC *pFormatEtc)
{
	if (LookupFormatEtc(pFormatEtc) >= 0)
	{
		return S_OK;
	}
	else
	{
		return DATA_E_FORMATETC;
	}
}

LONG CDataObject::LookupFormatEtc(FORMATETC *lpFormatEtc)
{
	size_t lPos;
	for(lPos = 0; lPos < m_vtFtmEtc.size(); lPos++)
	{
		LPFORMATETC lpCacheFtmEtc = &m_vtFtmEtc[lPos];
		LPSTGMEDIUM lpCacheStgMedium = &m_vtStgMedium[lPos];

		if (lpCacheFtmEtc->cfFormat == lpFormatEtc->cfFormat &&
			(lpCacheFtmEtc->tymed & lpFormatEtc->tymed) != 0 &&
			(lpCacheFtmEtc->dwAspect == DVASPECT_THUMBNAIL ||
			lpCacheFtmEtc->dwAspect == DVASPECT_ICON ||
			lpCacheStgMedium->tymed == TYMED_NULL ||
			lpCacheFtmEtc->lindex == lpFormatEtc->lindex ||
			(lpCacheFtmEtc->lindex == 0 && lpFormatEtc->lindex == -1) ||
			(lpCacheFtmEtc->lindex == -1 && lpFormatEtc->lindex == 0)) &&
			lpCacheFtmEtc->dwAspect == lpFormatEtc->dwAspect)
		{
			DVTARGETDEVICE* ptd1 = lpCacheFtmEtc->ptd;
			DVTARGETDEVICE* ptd2 = lpFormatEtc->ptd;

			if (ptd1 == NULL && ptd2 == NULL)
			{
				// both target devices are NULL (exact match), so return it
				break;
			}
			if (ptd1 != NULL && ptd2 != NULL &&
				ptd1->tdSize == ptd2->tdSize &&
				memcmp(ptd1, ptd2, ptd1->tdSize) == 0)
			{
				// exact match, so return it
				break;
			}
		}
	}

	if (lPos < m_vtFtmEtc.size())
	{
		return lPos;
	}
	else
	{
		return -1;
	}
}

HGLOBAL CopyGlobalMemory(HGLOBAL hDest, HGLOBAL hSource)
{
	assert(hSource != NULL);

	// make sure we have suitable hDest
	ULONG_PTR nSize = ::GlobalSize(hSource);
	if (hDest == NULL)
	{
		hDest = ::GlobalAlloc(GMEM_SHARE|GMEM_MOVEABLE, nSize);
		if (hDest == NULL)
			return NULL;
	}
	else if (nSize > ::GlobalSize(hDest))
	{
		// hDest is not large enough
		return NULL;
	}

	// copy the bits
	LPVOID lpSource = ::GlobalLock(hSource);
	LPVOID lpDest = ::GlobalLock(hDest);
	assert(lpDest != NULL);
	assert(lpSource != NULL);
	memcpy(lpDest, lpSource, (ULONG)nSize);
	::GlobalUnlock(hDest);
	::GlobalUnlock(hSource);

	// success -- return hDest
	return hDest;
}

BOOL CopyStgMedium(CLIPFORMAT cfFormat, LPSTGMEDIUM lpDest, LPSTGMEDIUM lpSource)
{
	if (lpDest->tymed == TYMED_NULL)
	{
		assert(lpSource->tymed != TYMED_NULL);
		switch (lpSource->tymed)
		{
		case TYMED_ENHMF:
		case TYMED_HGLOBAL:
			assert(sizeof(HGLOBAL) == sizeof(HENHMETAFILE));
			lpDest->tymed = lpSource->tymed;
			lpDest->hGlobal = NULL;
			break;  // fall through to CopyGlobalMemory case

		case TYMED_ISTREAM:
			lpDest->pstm = lpSource->pstm;
			lpDest->pstm->AddRef();
			lpDest->tymed = TYMED_ISTREAM;
			return TRUE;

		case TYMED_ISTORAGE:
			lpDest->pstg = lpSource->pstg;
			lpDest->pstg->AddRef();
			lpDest->tymed = TYMED_ISTORAGE;
			return TRUE;

		case TYMED_MFPICT:
			{
				// copy LPMETAFILEPICT struct + embedded HMETAFILE
				HGLOBAL hDest = CopyGlobalMemory(lpDest->hGlobal, lpSource->hGlobal);
				if (hDest == NULL)
					return FALSE;
				LPMETAFILEPICT lpPict = (LPMETAFILEPICT)::GlobalLock(hDest);
				assert(lpPict != NULL);
				lpPict->hMF = ::CopyMetaFile(lpPict->hMF, NULL);
				if (lpPict->hMF == NULL)
				{
					::GlobalUnlock(hDest);
					::GlobalFree(hDest);
					return FALSE;
				}
				::GlobalUnlock(hDest);

				// fill STGMEDIUM struct
				lpDest->hGlobal = hDest;
				lpDest->tymed = TYMED_MFPICT;
			}
			return TRUE;

		case TYMED_GDI:
			lpDest->tymed = TYMED_GDI;
			lpDest->hGlobal = NULL;
			break;

		case TYMED_FILE:
			{
				USES_CONVERSION;
				lpDest->tymed = TYMED_FILE;
				assert(lpSource->lpszFileName != NULL);
				UINT cbSrc = lstrlenW(lpSource->lpszFileName);
				LPOLESTR szFileName = (LPOLESTR)CoTaskMemAlloc((cbSrc+1)*sizeof(OLECHAR));
				lpDest->lpszFileName = szFileName;
				if (szFileName == NULL)
					return FALSE;
				memcpy(szFileName, lpSource->lpszFileName,  (cbSrc+1)*sizeof(OLECHAR));
				return TRUE;
			}

			// unable to create + copy other TYMEDs
		default:
			return FALSE;
		}
	}
	assert(lpDest->tymed == lpSource->tymed);

	switch (lpSource->tymed)
	{
	case TYMED_HGLOBAL:
		{
			HGLOBAL hDest = CopyGlobalMemory(lpDest->hGlobal, lpSource->hGlobal);
			if (hDest == NULL)
				return FALSE;

			lpDest->hGlobal = hDest;
		}
		return TRUE;

	case TYMED_ISTREAM:
		{
			assert(lpDest->pstm != NULL);
			assert(lpSource->pstm != NULL);

			// get the size of the source stream
			STATSTG stat;
			if (lpSource->pstm->Stat(&stat, STATFLAG_NONAME) != S_OK)
			{
				// unable to get size of source stream
				return FALSE;
			}
			assert(stat.pwcsName == NULL);

			// always seek to zero before copy
			LARGE_INTEGER zero = { 0, 0 };
			lpDest->pstm->Seek(zero, STREAM_SEEK_SET, NULL);
			lpSource->pstm->Seek(zero, STREAM_SEEK_SET, NULL);

			// copy source to destination
			if (lpSource->pstm->CopyTo(lpDest->pstm, stat.cbSize,
				NULL, NULL) != NULL)
			{
				// copy from source to dest failed
				return FALSE;
			}

			// always seek to zero after copy
			lpDest->pstm->Seek(zero, STREAM_SEEK_SET, NULL);
			lpSource->pstm->Seek(zero, STREAM_SEEK_SET, NULL);
		}
		return TRUE;

	case TYMED_ISTORAGE:
		{
			assert(lpDest->pstg != NULL);
			assert(lpSource->pstg != NULL);

			// just copy source to destination
			if (lpSource->pstg->CopyTo(0, NULL, NULL, lpDest->pstg) != S_OK)
				return FALSE;
		}
		return TRUE;

	case TYMED_FILE:
		{
			USES_CONVERSION;
			assert(lpSource->lpszFileName != NULL);
			assert(lpDest->lpszFileName != NULL);
			return CopyFile(OLE2T(lpSource->lpszFileName), OLE2T(lpDest->lpszFileName), FALSE);
		}


	case TYMED_ENHMF:
	case TYMED_GDI:
		{
			assert(sizeof(HGLOBAL) == sizeof(HENHMETAFILE));

			// with TYMED_GDI cannot copy into existing HANDLE
			if (lpDest->hGlobal != NULL)
				return FALSE;

			// otherwise, use OleDuplicateData for the copy
			lpDest->hGlobal = OleDuplicateData(lpSource->hGlobal, cfFormat, 0);
			if (lpDest->hGlobal == NULL)
				return FALSE;
		}
		return TRUE;

		// other TYMEDs cannot be copied
	default:
		return FALSE;
	}
}

STDMETHODIMP CDataObject::GetData(FORMATETC *pFormatEtc, STGMEDIUM *pStgMedium)
{
	LONG idx = LookupFormatEtc(pFormatEtc);
	if (idx  == -1)
	{
		return DV_E_FORMATETC;
	}

	memset(pStgMedium, 0, sizeof(STGMEDIUM));

	if(m_vtFtmEtc[idx].tymed != TYMED_NULL)
	{
		if (!CopyStgMedium(pFormatEtc->cfFormat, pStgMedium, &m_vtStgMedium[idx]))
		{
			ATLTRACE(L"\nGetData: %d, hr=%x", pFormatEtc->cfFormat, DATA_E_FORMATETC);

			return DATA_E_FORMATETC;
		}

		ATLTRACE(L"\nGetData: %d, hr=%x", pFormatEtc->cfFormat, 0);

		return S_OK;
	}

	ATLTRACE(L"\nGetData: %d, hr=%x", pFormatEtc->cfFormat, DATA_E_FORMATETC);

	return DATA_E_FORMATETC;
}

STDMETHODIMP CDataObject::EnumFormatEtc(DWORD dwDirection, IEnumFORMATETC **ppEnumFormatEtc)
{
	if(dwDirection == DATADIR_GET)
	{
		return CreateEnumFormatEtc(m_vtFtmEtc.size(), &m_vtFtmEtc[0], ppEnumFormatEtc);
	}
	else
	{
		return E_NOTIMPL;
	}
}

STDMETHODIMP CDataObject::DAdvise(FORMATETC * pFormatEtc, DWORD /*advf*/, IAdviseSink * /*pAdvSink*/, DWORD * /*pdwConnection*/)
{
	return OLE_E_ADVISENOTSUPPORTED;
}

STDMETHODIMP CDataObject::DUnadvise(DWORD /*dwConnection*/)
{
	return OLE_E_ADVISENOTSUPPORTED;
}

STDMETHODIMP CDataObject::EnumDAdvise(IEnumSTATDATA ** /*ppEnumAdvise*/)
{
	return OLE_E_ADVISENOTSUPPORTED;
}

STDMETHODIMP CDataObject::GetDataHere(FORMATETC * /*pFormatEtc*/, STGMEDIUM * /*pMedium*/)
{
	return DATA_E_FORMATETC;
}

STDMETHODIMP CDataObject::GetCanonicalFormatEtc(FORMATETC * /*pFormatEct*/, FORMATETC *pFormatEtcOut)
{
	pFormatEtcOut->ptd = NULL;
	return E_NOTIMPL;
}

HGLOBAL GlobalClone(HGLOBAL hglobIn)
{
	HGLOBAL hglobOut = NULL;
	LPVOID pvIn = ::GlobalLock(hglobIn);

	if (pvIn != NULL)
	{
		SIZE_T cb = ::GlobalSize (hglobIn);

		HGLOBAL hGlobalOut = ::GlobalAlloc(GMEM_SHARE|GMEM_MOVEABLE, cb);
		if (hGlobalOut != NULL)
		{
			LPVOID lpOut = ::GlobalLock(hGlobalOut);
			assert(lpOut);
			if (lpOut != NULL)
			{
				::CopyMemory(lpOut, pvIn, cb);

				::GlobalUnlock(hGlobalOut);
			}
			else
			{
				::GlobalFree(hglobOut);
				hglobOut = NULL;
			}
		}

		::GlobalUnlock(hglobIn);
	}

	return hglobOut;
}

HRESULT CDataObject::AddRefStgMedium(STGMEDIUM* pstgmIn, STGMEDIUM* pstgmOut, BOOL fCopyIn )
{
	HRESULT hres = S_OK;
	STGMEDIUM stgmOut = *pstgmIn;

	if ( pstgmIn->pUnkForRelease == NULL &&
		!(pstgmIn->tymed & (TYMED_ISTREAM | TYMED_ISTORAGE)) ) 
	{
		if ( fCopyIn )
		{
			/* Object needs to be cloned */
			if ( pstgmIn->tymed == TYMED_HGLOBAL )
			{
				stgmOut.hGlobal = GlobalClone ( pstgmIn->hGlobal );

				if ( !stgmOut.hGlobal )
					hres = E_OUTOFMEMORY;
			}
			else 
				hres = DV_E_TYMED;      /* Don't know how to clone GDI objects */
		} 
		else  
			stgmOut.pUnkForRelease = static_cast<IDataObject*>(this);
	}

	if ( SUCCEEDED(hres) )
	{
		switch ( stgmOut.tymed ) 
		{
		case TYMED_ISTREAM:
			stgmOut.pstm->AddRef();
			break;

		case TYMED_ISTORAGE:
			stgmOut.pstg->AddRef();
			break;
		}

		if ( stgmOut.pUnkForRelease )
			stgmOut.pUnkForRelease->AddRef();

		*pstgmOut = stgmOut;
	}

	return hres;
}

STDMETHODIMP CDataObject::SetData(FORMATETC * pFormatEtc, STGMEDIUM * pMedium,  BOOL fRelease)
{
	assert(pMedium->tymed != TYMED_NULL);

	pFormatEtc->tymed = pMedium->tymed;

	LONG index = LookupFormatEtc(pFormatEtc);
	if (index == -1)
	{
		m_vtFtmEtc.push_back(*pFormatEtc);
		m_vtStgMedium.push_back(*pMedium);

		index = m_vtFtmEtc.size() - 1;
	}
	else
	{
		FORMATETC* lpCurFmtEtc = &m_vtFtmEtc[index];
		STGMEDIUM* lpCurStdMedium = &m_vtStgMedium[index];

		::CoTaskMemFree(lpCurFmtEtc->ptd);
		::ReleaseStgMedium(lpCurStdMedium);

		*lpCurFmtEtc = *pFormatEtc;
		*lpCurStdMedium = *pMedium;
	}
	
	return S_OK;
}