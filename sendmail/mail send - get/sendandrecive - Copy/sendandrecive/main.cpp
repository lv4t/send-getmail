#include <tchar.h>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "EAGetMailobj.tlh"
using namespace EAGetMailObjLib;

DWORD _getCurrentPath(LPTSTR lpPath, DWORD nSize)
{
    DWORD dwSize = ::GetModuleFileName(NULL, lpPath, nSize);
    if (dwSize == 0 || dwSize == nSize)
    {
        return 0;
    }

    // Change file name to current full path
    LPCTSTR psz = _tcsrchr(lpPath, _T('\\'));
    if (psz != NULL)
    {
        lpPath[psz - lpPath] = _T('\0');
        return _tcslen(lpPath);
    }

    return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
    const int MailServerPop3 = 0;

    // Initialize COM environment
    ::CoInitialize(NULL);

    // Create a folder named "inbox" under current exe file directory
    // to save the emails retrieved.

    TCHAR szPath[MAX_PATH + 1];
    _getCurrentPath(szPath, MAX_PATH);

    TCHAR szMailBox[MAX_PATH + 1];
    wsprintf(szMailBox, _T("%s\\inbox"), szPath);

    // Create a folder to store emails
    ::CreateDirectory(szMailBox, NULL);

    try
    {
        IMailServerPtr oServer = NULL;
        oServer.CreateInstance(__uuidof(EAGetMailObjLib::MailServer));

        oServer->Server = _T("Pop.gmail.com");
        oServer->User = _T("luantest7@gmail.com");
        oServer->Password = _T("vfyz rsdu joop jlwm");
        oServer->Protocol = MailServerPop3;

        // Enable SSL/TLS connection, most modern email servers require SSL/TLS by default
        oServer->SSLConnection = VARIANT_TRUE;
        oServer->Port = 995;

        while (true) // Vòng lặp vô hạn để liên tục đón email tới
        {
            IMailClientPtr oClient = NULL;
            oClient.CreateInstance(__uuidof(EAGetMailObjLib::MailClient));
            oClient->LicenseCode = _T("TryIt");

            oClient->Connect(oServer);
            _tprintf(_T("Connected\r\n"));

            IMailInfoCollectionPtr infos = oClient->GetMailInfoList();
            _tprintf(_T("Total %d emails\r\n"), infos->Count);

            for (long i = 0; i < infos->Count; i++)
            {
                IMailInfoPtr pInfo = infos->GetItem(i); // Lấy thông tin email

                // Receive email from POP3 server
                IMailPtr oMail = oClient->GetMail(pInfo);

                // Lấy nội dung của email
                _bstr_t textBody = oMail->TextBody;
                std::wstring content = (const wchar_t*)textBody;

                // Xuất nội dung email ra màn hình
                std::wcout << L"Email content: " << content << std::endl;

                // Đánh dấu email đã đọc để tránh đọc lại trong lần lặp tiếp theo
                oClient->Delete(pInfo);
            }

            oClient->Quit();

            // Chờ một khoảng thời gian trước khi kiểm tra email mới
            Sleep(10000); // Ví dụ: Kiểm tra email mới mỗi 10 giây
        }
    }
    catch (_com_error& ep)
    {
        _tprintf(_T("Error: %s"), (const TCHAR*)ep.Description());
    }

    // Giải phóng môi trường COM
    ::CoUninitialize();

    return 0;
}
