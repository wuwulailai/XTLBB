// script.h
#pragma once
#import "C:\\Windows\\QMDispatch.dll" no_namespace

#include "UDPBroadcast.h"
#include <tlhelp32.h>

extern HWND g_hWnd;

#pragma comment(lib, "winmm.lib")


extern DWORD g_bServer;
extern DWORD g_bSaveBmp;
extern DWORD g_bWhite;
extern DWORD g_bGamer;
extern char g_sName[256];

class ScriptBase
{
public:
	ScriptBase() 
	{
		m_start = false;
	}
	virtual ~ScriptBase() 
	{
	}
	void Start()
	{
		if(!m_start)
		{
			m_start = true;
			m_timerCount = 0;
			m_hWnd = GetForegroundWindow();
			m_hDC = GetDC(m_hWnd);

			Beep(987, 100);

			DoStart();
		}
		else
		{
			m_timerCount = 0;

			/*
			m_hWnd = GetForegroundWindow();
			m_hDC = GetDC(m_hWnd);
			*/

			Beep(987, 100);

			DoReload();
		}
	}
	void Stop()
	{
		if(m_start)
		{
			m_start = false;

			ReleaseDC(m_hWnd, m_hDC);
			Beep(879, 100);
			DoStop();
		}
	}
	void OnTimer()
	{
		if(m_start)
		{
			m_timerCount++;
			DoOnTimer();
		}
	}
	virtual char * GetName()
	{
		return "";
	}
protected:
	virtual void DoStart()
	{
	}
	virtual void DoStop()
	{
	}
	virtual void DoReload()
	{
	}
	virtual void DoOnTimer()
	{
	}
public:
	static bool InitScript()
	{
		g_pbc = new UDPBroadcast(1822);

		::CoInitialize(NULL);
		g_obj.CreateInstance(__uuidof(QMFunction));
		return !(g_obj == NULL);
	}
	static HANDLE GetEventHandle()
	{
		return g_pbc->GetEventHandle();
	}
protected:
	enum OBJECT_COLOR
	{
		INVALID_OBJECT, RED_OBJECT, GREEN_OBJECT, WHITE_OBJECT
	};

	/*
	bool IsFullLife()
	{
		COLORREF cr = GetColor(m_hWnd, m_hDC, 400, 32);
		return IsRed(cr);
	}
	bool IsSelfFullLife()
	{
		COLORREF cr = GetColor(m_hWnd, m_hDC, 196, 37);
		return IsRed(cr);
	}
	bool IsGamerFullLife(int id)
	{
		int y = 129 + 45 * id;

		COLORREF cr = GetColor(m_hWnd, m_hDC, 44, y);
		if(!IsBlack(cr)) return true;

		cr = GetColor(m_hWnd, m_hDC, 45, y);
		if(!IsRedRed(cr)) return true;

		cr = GetColor(m_hWnd, m_hDC, 140, y);
		if(!IsBlack(cr)) return true;

		int c = 0;
		
		// return false;

		for(int x = 46; x < 140; x++)
		{
			cr = GetColor(m_hWnd, m_hDC, x, y);
			if(!IsBlack(cr) && !IsRedRed(cr)) 
				c++;
		}

		return c > 10;
	}
	*/

	// ���Ƿ����
	bool IsAlive()
	{
		COLORREF cr = GetColor(m_hWnd, m_hDC, 274, 33);
		return IsRed(cr);
	}
	// ��ȡĿ�����ɫ����ɫ��ʾδ��������¹�����ǰ�ɫ����ɫ��ʾ��������ɫ��ʾ�Է�����¹�����ǻ�ɫ��
	OBJECT_COLOR GetObjectColor()
	{
		COLORREF cr = GetColor(m_hWnd, m_hDC, 276, 26);

		if(IsRed(cr))
			return RED_OBJECT;
		else if(IsGreen(cr))
			return GREEN_OBJECT;
		else if(IsWhite(cr))
			return WHITE_OBJECT;
		else
			return INVALID_OBJECT;			
	}
	// Ŀ���Ƿ��Ǻ�ɫ
	static bool IsRed(COLORREF cr)
	{
		return GetRValue(cr) >= 180 && GetGValue(cr) < 130 && GetBValue(cr) < 130;
	}
	// Ŀ���Ƿ�����ɫ��¹���������ǰ�ɫ��
	static bool IsGreen(COLORREF cr)
	{
		return GetRValue(cr) >= 150 && GetGValue(cr) >= 150 && GetBValue(cr) >= 150;
	}
	// Ŀ���Ƿ��ǰ�ɫ��¹���������ǻ�ɫ��
	static bool IsWhite(COLORREF cr)
	{
		return GetRValue(cr) > 90 && GetGValue(cr) > 90 && GetBValue(cr) > 90
			&& GetRValue(cr) < 130 && GetGValue(cr) < 130 && GetBValue(cr) < 130;
	}

	/*
	bool IsGamer()
	{
		COLORREF cr = GetColor(m_hWnd, m_hDC, 279, 36);
		if(IsBlue(cr)) 
			return true;
		cr = GetColor(m_hWnd, m_hDC, 279, 42);
		if(IsYellow(cr)) 
			return true;
		return false;	
	}

	bool IsBoss()
	{
		COLORREF cr = GetColor(m_hWnd, m_hDC, 279, 36);
		if(IsRed(cr)) 
			return true;
		cr = GetColor(m_hWnd, m_hDC, 279, 42);
		if(IsRed(cr)) 
			return true;
		return false;	
	}
	*/
	
	void LeftClick(int x, int y)
	{
		POINT p;
		p.x = x;
		p.y = y;
		ClientToScreen(m_hWnd, &p);
		SetCursorPos(p.x, p.y);
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		// mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);		
	}

	static void SendKeyPress(HWND hWnd, int code)
	{
		PostMessage(hWnd, WM_KEYDOWN, code, MapVirtualKey(code, 0) >> 8 | 0x01);
		PostMessage(hWnd, WM_KEYUP, code, 0xC0000000 | MapVirtualKey(code, 0) >> 8 | 0x01);
	}

	static COLORREF GetColor(HWND hWnd, HDC hDC, int x, int y)
	{
		RECT r;
		GetClientRect(hWnd, &r);

		COLORREF cr = GetPixel(hDC, x + r.left, y + r.top);

		char szBuf[256];
		wsprintfA(szBuf, "%d %d %x\n", x, y, cr);
		OutputDebugString(szBuf);

		return cr;
	}

	/*
	static bool IsRedRed(COLORREF cr)
	{
		return GetRValue(cr) >= 0xF0 && GetGValue(cr) < 0x20 && GetBValue(cr) < 0x20;
	}
	static bool IsBlack(COLORREF cr)
	{
		return GetRValue(cr) == GetGValue(cr) && GetGValue(cr) == GetBValue(cr);		
	}
	static bool IsBlue(COLORREF cr)
	{
		return GetBValue(cr) >= 0x80 && GetRValue(cr) < 0x20 && GetGValue(cr) < 0x20;
	}
	static bool IsYellow(COLORREF cr)
	{
		return GetRValue(cr) >= 0x80 && GetGValue(cr) >= 0x80 && GetBValue(cr) < 0x20;
	}
	*/

protected:
	HWND m_hWnd;
	HDC m_hDC;
	bool m_start;
	int m_timerCount;
	static IQMFunctionPtr g_obj;
public:
	static UDPBroadcast * g_pbc;
};

char g_dllName[] = "C:\\InjectDLL.dll";

void StartInject(HWND hWnd)
{
	HANDLE hProcess = NULL;
	HANDLE hThread = NULL;
	char * pFileName = NULL;
	__try
	{
		DWORD pid = 0;
		GetWindowThreadProcessId(hWnd, &pid);

		hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE,
			FALSE, pid);
		if(!hProcess)
		{
			__leave;
		}

		int cb = strlen(g_dllName) + 1;

		pFileName = (char *)VirtualAllocEx(hProcess, NULL, cb, MEM_COMMIT, PAGE_READWRITE);
		if(!pFileName)
		{
			__leave;
		}

		if(!WriteProcessMemory(hProcess, pFileName, g_dllName, cb, NULL))
		{
			__leave;
		}

		PTHREAD_START_ROUTINE pfn = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("Kernel32"), 
			"LoadLibraryA");
		if(!pfn)
		{
			__leave;
		}

		hThread = CreateRemoteThread(hProcess, NULL, 0, pfn, pFileName, 0, NULL);
		if(!hThread)
		{
			__leave;
		}

		WaitForSingleObject(hThread, INFINITE);
	}
	__finally
	{
		if(pFileName)
			VirtualFreeEx(hProcess, pFileName, 0, MEM_RELEASE);
		if(hThread) CloseHandle(hThread);
		if(hProcess) CloseHandle(hProcess);
	}
}

void StopInject(HWND hWnd)
{
	HANDLE hth = NULL;
	HANDLE hProcess = NULL;
	HANDLE hThread = NULL;

	__try
	{
		DWORD pid = 0;
		GetWindowThreadProcessId(hWnd, &pid);

		hth = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
		if(!hth)
		{
			__leave;
		}
		
		MODULEENTRY32 me = {sizeof(me)};
		BOOL ff = FALSE;
		for(BOOL fm = Module32First(hth, &me); fm; fm = Module32Next(hth, &me))
		{
			ff = (lstrcmpi(me.szExePath, g_dllName) == 0 || lstrcmpi(me.szModule, g_dllName) == 0);
			if(ff) break;
		}
		if(!ff)
		{
			__leave;
		}

		hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE,
			FALSE, pid);
		if(!hProcess)
		{
			__leave;
		}

		PTHREAD_START_ROUTINE pfn = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("Kernel32"), 
			"FreeLibrary");
		if(!pfn)
		{
			__leave;
		}

		hThread = CreateRemoteThread(hProcess, NULL, 0, pfn, me.modBaseAddr, 0, NULL);
		if(!hThread)
		{
			__leave;
		}

		WaitForSingleObject(hThread, INFINITE);
	}
	__finally
	{
		if(hth) CloseHandle(hth);
		if(hThread) CloseHandle(hThread);
		if(hProcess) CloseHandle(hProcess);
	}
}

class SimpleScript: public ScriptBase
{
protected:
	virtual void DoStart()
	{
		m_fast = false;
	}
	virtual void DoReload()
	{
		m_fast = !m_fast;
	}
	virtual void DoOnTimer()
	{
		DWORD t = m_fast ? 1 : 10;
		if(m_timerCount == 1)
			SendKeyPress(m_hWnd, 122);
		else if(m_timerCount == t + 1)
			SendKeyPress(m_hWnd, 123);
		if(m_timerCount == 2 * t)
			m_timerCount = 0;
	}
public:
	virtual char * GetName()
	{
		if(!m_start)
			return "�򵥴��";
		else if(m_fast)
			return "�򵥴��(����)";
		else
			return "�򵥴��(����)";
	}
protected:
	DWORD m_fast;
};

/*
class NormalScript: public ScriptBase
{
protected:
	virtual void DoStart()
	{
		m_auto = true;
		m_n = 0;
	}
	virtual void DoReload()
	{
		m_auto = !m_auto;
		m_n = 0;
	}
	virtual void DoOnTimer()
	{
		// ��ɫ���F11֮���Լ�ÿ�η���֮�󣬵ȴ�ʱ��Ϊ50����
		if(m_timerCount <= 5) return;

		OBJECT_COLOR oc = GetObjectColor();
		if((oc == GREEN_OBJECT && !m_auto || oc == RED_OBJECT && m_auto) && !IsGamer() && IsAlive())
		{
			if(m_n < 80)
			{
				if(m_n % 40 == 0) 
					SendKeyPress(m_hWnd, 123);						
				m_n++;
				m_timerCount = 1;
				return;		
			}
		}
		SendKeyPress(m_hWnd, 122);
		m_n = 0;
		m_timerCount = 1;
	}
public:
	virtual char * GetName()
	{
		if(!m_start)
			return "��ɫ���";
		else if(m_auto)
			return "��ɫ���(ֻ����)";
		else
			return "��ɫ���(ֻ���̹�)";
	}
protected:
	bool m_auto;
	DWORD m_n;
};
*/

class MouseScript: public ScriptBase
{
protected:
	virtual void DoStart()
	{
		GetCursorPos(&m_p);
	}
	virtual void DoOnTimer()
	{
		if(m_timerCount % 100 == 1 && m_timerCount < 900)
		{
			SendKeyPress(m_hWnd, 122);
		}
		else if(m_timerCount % 50 == 11 && m_timerCount < 950)
		{
			SendKeyPress(m_hWnd, 123);
		}
		else if(m_timerCount == 981)
		{
			SetCursorPos(m_p.x, m_p.y);
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		}
		else if(m_timerCount == 1001)
		{
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		}
		else if(m_timerCount == 1400)
		{
			m_timerCount = 0;
		}
	}
	virtual char * GetName()
	{
		return "���ֻ�ԭ��";
	}
	POINT m_p;
};
class AIScript: public ScriptBase
{
protected:
	virtual void DoStart()
	{
		StartInject(m_hWnd);

		if(!g_bSaveBmp)
		{
			for(int i = 0; i < m_aBmp.GetSize(); i++)
				DeleteObject(m_aBmp[i]);
			m_aBmp.RemoveAll();
		}
		m_mode = 0;
		m_fast = false;
		m_fastCount = 0;
		m_objectCount = 0;
		m_n = 0;
		m_xxxCount = 0;

		HDC hdc = CreateCompatibleDC(m_hDC);
		HBITMAP hbmp = CreateCompatibleBitmap(m_hDC, 600, 300);
		SelectObject(hdc, hbmp);
		BitBlt(hdc, 0, 0, 600, 300, m_hDC, 0, 0, SRCCOPY);
		DeleteDC(hdc);
		
		OpenClipboard(g_hWnd);
		EmptyClipboard(); 
		SetClipboardData(CF_BITMAP, hbmp);
		CloseClipboard();

		DeleteObject(hbmp);
	}
	virtual void DoStop()
	{
		StopInject(m_hWnd);
	}
	virtual void DoReload()
	{
		m_mode = (m_mode + 1) % 3;
		m_n = 0;
	}
	virtual void DoOnTimer()
	{
		m_xxxCount++;

		if (m_xxxCount == 100)
			SendKeyPress(m_hWnd, 118);
		if (m_xxxCount == 200)
			SendKeyPress(m_hWnd, 119);
		if (m_xxxCount == 300)
			SendKeyPress(m_hWnd, 120);
		if (m_xxxCount == 400)
			m_xxxCount = 0;

		if(m_fast)
		{
			m_fastCount++;
			if(m_fastCount >= 3000)
				m_fast = false;
		}
		if(m_gamer)
		{
			m_gamerCount++;
			if(m_gamerCount >= 3000)
				m_gamer = false;			
		}
		if(m_timerCount <= 10) return;

		OBJECT_COLOR oc = GetObjectColor();

		/*
		if(g_bWhite)
		{
			if(oc == WHITE_OBJECT && !IsGamer() && IsAlive())
			{
				m_fastCount = 0;
				if(!m_fast)
				{
					m_fast = true;
					char buf[256] = "��������: ";
					lstrcat(buf, g_sName);
					g_pbc->SendBroadcast(buf, strlen(buf) + 1);
				}
			}
		}
		if(g_bGamer && IsGamer())
		{
			m_gamerCount = 0;
			if(!m_gamer)
			{
				m_gamer = true;
				char buf[256] = "�е����: ";
				lstrcat(buf, g_sName);
				g_pbc->SendBroadcast(buf, strlen(buf) + 1);
			}			
		}
		*/
		if((oc != INVALID_OBJECT) /*&& !IsGamer()*/ && IsAlive())
		{
			DWORD limit;
			DWORD key;
			if(oc == WHITE_OBJECT)
			{
				goto another;
			}
			else if(oc == GREEN_OBJECT)
			{
				if(m_mode == 2) 
					limit = 1;
				else
					limit = 30;
				key = 123;
			}
			else 
			{
				key = 123;
				if(m_mode == 1)
					limit =	1;
				else if(m_fast)
					limit = 15;
				else
					limit = 350;
			}
			if(m_n == 0)
			{
				/*
				m_objectCount++;
				
				HDC hdc = CreateCompatibleDC(m_hDC);
				HBITMAP hbmp = CreateCompatibleBitmap(m_hDC, 40, 40);
				SelectObject(hdc, hbmp);
				BitBlt(hdc, 0, 0, 40, 40, m_hDC, 420, 4, SRCCOPY);
				DeleteDC(hdc);
					
				if(!FindBitmap(hbmp))
				{
					if(m_objectCount <= 20 || IsBoss())
						AddBitmap(hbmp);
					else
					{
						DeleteObject(hbmp);
						goto another;
					}
				}
				else 
					DeleteObject(hbmp);
				*/
				// goto another;
			}
			if(m_n < limit)
			{				
				if(m_n %2 == 0)
					SendKeyPress(m_hWnd, key);	
				m_n++;
				m_timerCount = 1;
				return;		
			}
		}
another:
		SendKeyPress(m_hWnd, 122);
		m_n = 0;
		m_timerCount = 1;
	}
	/*
	bool FindBitmap(HBITMAP hbmp)
	{
		HDC hdc1 = CreateCompatibleDC(m_hDC);
		SelectObject(hdc1, hbmp);
		
		bool b1 = false;
		for(int n = 0; n < m_aBmp.GetSize() && !b1; n++)
		{
			bool b2 = false;
			HDC hdc2 = CreateCompatibleDC(m_hDC);
			SelectObject(hdc2, m_aBmp[n]);
			for(int i = 0; i < 40 && !b2; i++) for(int j = 0; j < 40 && !b2; j++)
			{
				if(GetPixel(hdc1, i, j) != GetPixel(hdc2, i, j))
					b2 = true;		
			}
			DeleteDC(hdc2);
			b1 = !b2;
		}
		DeleteDC(hdc1);
		return b1;
	}
	void AddBitmap(HBITMAP hbmp)
	{
		m_aBmp.Add(hbmp);
	}
	*/
public:
	virtual char * GetName()
	{
		if(!m_start)
			return "���ܴ��";
		else if(m_mode == 0)
			return "���ܴ��(�Զ�)";
		else if(m_mode == 1)
			return "���ܴ��(�����й�)";
		else 
			return "���ܴ��(���ģʽ)";
	}
protected:
	DWORD m_mode;
	bool m_fast;
	DWORD m_fastCount; 
	bool m_gamer;
	DWORD m_gamerCount;
	DWORD m_n;
	DWORD m_objectCount;
	DWORD m_xxxCount;
	ATL::CSimpleArray<HBITMAP> m_aBmp;
};

class AIScript2: public AIScript
{
	virtual void DoOnTimer()
	{
		if (m_xxxCount % 600 == 50)
			SendKeyPress(m_hWnd, 117);
		AIScript::DoOnTimer();
	}
	virtual char * GetName()
	{
		if(!m_start)
			return "���ܴ�ּ��";
		else if(m_mode == 0)
			return "���ܴ�ּ��(�Զ�)";
		else if(m_mode == 1)
			return "���ܴ�ּ��(�����й�)";
		else 
			return "���ܴ�ּ��(���ģʽ)";
	}
};

class GuaJiScript: public ScriptBase
{
public:
	virtual void DoOnTimer()
	{
		if(m_timerCount == 300)
		{
			if(FoundPic()) 
			{
				char buf[256] = "�һ����: ";
				lstrcat(buf, g_sName);
				g_pbc->SendBroadcast(buf, strlen(buf) + 1);
			}
			else
				m_timerCount = 0;
		}
		else if(m_timerCount >= 6000)
			m_timerCount = 0;
	}
	virtual char * GetName()
	{
		return "���һ����";
	}
private:
	bool FoundPic()
	{
		int x, y;

		if(g_obj)
		{
			x = g_obj->FindPic(0, 0, 1600, 1000, "C:\\002.bmp", (float)0.7);
			y = x % 0x2000;
			x = x / 0x2000;
			return x > 0 && y > 0;
		}
		else
			return false;
	}
};

class InjectScript: public ScriptBase
{
protected:
	virtual void DoStart()
	{
		StartInject(m_hWnd);
	}
	virtual void DoStop()
	{
		StopInject(m_hWnd);
	}
	virtual char * GetName()
	{
		if(!m_start)
			return "˫������";
		else 
			return "˫������(����)";
	}
};
#define WM_CHANGE_MODE (WM_USER + 0xc531)

class SimpleFarmerScript: public ScriptBase
{
protected:
	virtual void DoStart()
	{
		StartInject(m_hWnd);
		m_mode = 0;

		PostMessage(m_hWnd, WM_CHANGE_MODE, 0, 2);
	}
	virtual void DoStop()
	{
		StopInject(m_hWnd);
	}
	virtual void DoReload()
	{
		m_mode = 1;
		PostMessage(m_hWnd, WM_CHANGE_MODE, 0, 3);
	}
	virtual char * GetName()
	{
		if(!m_start)
			return "�ֵز���";
		else if(m_mode == 0) 
			return "�ֵز���(��¼)";
		else
			return "�ֵز���(����)";
	}
private:
	int m_mode;
};

class FarmerScript: public ScriptBase
{
protected:
	virtual void DoStart()
	{
		StartInject(m_hWnd);
		m_mode = 0;

		PostMessage(m_hWnd, WM_CHANGE_MODE, 0, 2);
	}
	virtual void DoStop()
	{
		StopInject(m_hWnd);
	}
	virtual void DoReload()
	{
		m_mode = 1;
		m_state = 0;
		m_tickCount = GetTickCount();
	}
	virtual void DoOnTimer()
	{
		if (m_mode == 0)
			return;

		DWORD t = GetTickCount() - m_tickCount;

		if (m_state == 0 && t >= 330 * 1000)
		{
			// ��ʼ�յ�һ���
			m_state = 1;
		}
		else if (m_state == 1 && t >= 330 * 1000 + 3 * 1000)
		{
			// ����յ�һ���
			m_state = 2;
		}
		else if (m_state == 2 && t >= 330 * 1000 + 4 * 1000)
		{
			// ��ʼ�յڶ����
			m_state = 3;
		}
		else if (m_state == 3 && t >= 330 * 1000 + 7 * 1000)
		{
			// ����յڶ����
			m_state = 4;
		}
		else if (m_state == 4 && t >= 330 * 1000 + 8 * 1000)
		{
			// ��ʼ�յ������
			m_state = 5;
		}
		else if (m_state == 5 && t >= 330 * 1000 + 11 * 1000)
		{
			// ����յ������
			m_state = 6;
		}
		else if (m_state == 6 && t >= 330 * 1000 + 12 * 1000)
		{
			// ��ʼ�յ��Ŀ��
			m_state = 7;
		}
		else if (m_state == 7 && t >= 330 * 1000 + 15 * 1000)
		{
			// ����յ��Ŀ��
			m_state = 8;
		}
		else if (m_state == 8 && t >= 330 * 1000 + 16 * 1000)
		{
			// ����һ�ε�
			PostMessage(m_hWnd, WM_CHANGE_MODE, 0, 3);
			DoReload();
		}
	}
	virtual char * GetName()
	{
		if(!m_start)
			return "�ֵ�";
		else if(m_mode == 0) 
			return "�ֵ�(��¼ָ��)";
		else
			return "�ֵ�(������)";
	}
private:
	int m_mode;
	int m_state;
	DWORD m_tickCount;
};
class ScriptFactory
{
public:
	static void Init(HWND hwnd)
	{
		ScriptBase::InitScript();
		for(int i = 0; i < 8; i++)
			scripts[i] = NULL;
		g_hwnd = hwnd;
	}
	static void CreateScript(ScriptBase * psb, int hotKey, DWORD id)
	{
		scripts[hotKey] = psb;
		ids[hotKey] = id;
		RegisterHotKey(g_hwnd, hotKey + 100, MOD_ALT, hotKey + 116);
		RegisterHotKey(g_hwnd, hotKey + 200, MOD_CONTROL, hotKey + 116);

		char buffer[256];
		wsprintfA(buffer, "F%d: %s", hotKey + 5, psb->GetName());
		SetWindowText(GetDlgItem(g_hwnd, id), buffer);
	}
	static void OnTimer()
	{
		for(int i = 0; i < 8; i++) if(scripts[i])
			scripts[i]->OnTimer();
		
		char buffer[256];
		
		sockaddr_in addr;
		if(g_bServer && ScriptBase::g_pbc->RecvBroadcast(buffer, 256, &addr) > 0)
		{
			PlaySound("C:\\lang.wav", NULL, SND_ASYNC | SND_FILENAME);
			SetWindowText(g_hwnd, buffer);
		}
	}
	static void OnHotKey(int wParam)
	{
		if(wParam >= 200)
		{
			scripts[wParam - 200]->Start();
			
			char buffer[256];
			wsprintfA(buffer, "->-> F%d: %s", wParam - 200 + 5, scripts[wParam - 200]->GetName());
			SetWindowText(GetDlgItem(g_hwnd, ids[wParam - 200]), buffer);
		}
		else
		{
			scripts[wParam - 100]->Stop();

			char buffer[256];
			wsprintfA(buffer, "F%d: %s", wParam - 100 + 5, scripts[wParam - 100]->GetName());
			SetWindowText(GetDlgItem(g_hwnd, ids[wParam - 100]), buffer);
		}
	}
public:
	static ScriptBase * scripts[8];
	static DWORD ids[8];
	static HWND g_hwnd;
};