#pragma once
#include <functional>
#include "COMBase.h"
#include "CompositionEventArgs.h"
namespace libtf {
	class TFAPI ContextOwnerCompositionSink :
		public COMBase,
		public ITfContextOwnerCompositionSink
	{
	public:
		typedef std::function<VOID(ITfContextOwnerCompositionSink*, CompositionEventArgs*)> signal_Comp;
		signal_Comp				m_sigComposition = [](ITfContextOwnerCompositionSink*, CompositionEventArgs*) {};

		// ͨ�� ITfContextOwnerCompositionSink �̳�
		virtual HRESULT __stdcall OnStartComposition(ITfCompositionView* pComposition, BOOL* pfOk) override;
		virtual HRESULT __stdcall OnUpdateComposition(ITfCompositionView* pComposition, ITfRange* pRangeNew) override;
		virtual HRESULT __stdcall OnEndComposition(ITfCompositionView* pComposition) override;

		//COMBase
		virtual HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObject) override {
			TF_COM_ASUNK(ITfContextOwnerCompositionSink);
			TF_COM_RETURN;
		}
		TF_COM_REFS;
	};
}