// dllmain.h : Declaration of module class.

class Clinkom_atlModule : public ATL::CAtlDllModuleT< Clinkom_atlModule >
{
public :
	DECLARE_LIBID(LIBID_Linkom)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_LINKOM_ATL, "{9804386C-C343-4919-9714-70EAF0406C30}")
};

extern class Clinkom_atlModule _AtlModule;
