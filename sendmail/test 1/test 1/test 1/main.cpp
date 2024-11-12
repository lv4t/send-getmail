#include <tchar.h>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>

#include "EASendMailObj.tlh"
using namespace EASendMailObjLib;

const int ConnectNormal = 0;
const int ConnectSSLAuto = 1;
const int ConnectSTARTTLS = 2;
const int ConnectDirectSSL = 3;
const int ConnectTryTLS = 4;

void attachAndSendMail(const std::wstring& filePath, IMailPtr& oSmtp) {
    // Đính kèm tệp
    oSmtp->AddAttachment(filePath.c_str());
}

int _tmain(int argc, _TCHAR* argv[])
{
    HRESULT hr = ::CoInitialize(NULL);
    if (FAILED(hr)) {
        // Xử lý lỗi
        return -1;
    }

    IMailPtr oSmtp = NULL;
    oSmtp.CreateInstance(__uuidof(EASendMailObjLib::Mail));
    oSmtp->LicenseCode = _T("TryIt");

    // Set your gmail email address
    oSmtp->FromAddr = _T("luan93vk@gmail.com");

    // Địa chỉ mail nhận
    oSmtp->AddRecipientEx(_T("luantest7@gmail.com"), 0);

    // Gmail SMTP server address
    oSmtp->ServerAddr = _T("smtp.gmail.com");

    // Gmail user authentication should use your Gmail email address as the user name.
    oSmtp->UserName = _T("luan93vk@gmail.com");

    // Create app password in Google account
    oSmtp->Password = _T("qenv ahsr fpsq oikf");

    oSmtp->ServerPort = 465;

    // detect SSL/TLS automatically
    oSmtp->ConnectType = ConnectSSLAuto;

    int option;
    _tprintf(_T("Select option:\n"));
    _tprintf(_T("1. Send message\n"));
    _tprintf(_T("2. Send text file\n"));
    _tprintf(_T("3. Send image\n"));
    _tprintf(_T("4. Send message with text file attachment\n"));
    _tprintf(_T("5. Send message with image attachment\n"));
    _tscanf_s(_T("%d"), &option);

    std::wstring subject;
    std::wstring message;
    std::wstring fileName;

    _tprintf(_T("Enter the subject: "));
    std::wcin.ignore(); // Ignore newline character left in buffer
    std::getline(std::wcin, subject);

    if (option == 1 || option == 4 || option == 5) {
        _tprintf(_T("Enter the message: "));
        std::getline(std::wcin, message);
    }

    if (option == 2 || option == 4) {
        _tprintf(_T("Enter the text file name to send: "));
        std::wcin >> fileName;
    }

    if (option == 3 || option == 5) {
        _tprintf(_T("Enter the image file name to send: "));
        std::wcin >> fileName;
    }

    switch (option) {
    case 1:
    {
        // Gửi tin nhắn
        oSmtp->Subject = subject.c_str();
        oSmtp->BodyText = message.c_str();
        break;
    }
    case 2: {
        // Gửi tệp văn bản
        oSmtp->Subject = subject.c_str();
        attachAndSendMail(fileName, oSmtp);
        break;
    }
    case 3: {
        // Gửi hình ảnh
        oSmtp->Subject = subject.c_str();
        attachAndSendMail(fileName, oSmtp);
        break;
    }
    case 4: {
        //gui tin nhan và text
        oSmtp->Subject = subject.c_str();
        oSmtp->BodyText = message.c_str();
        attachAndSendMail(fileName, oSmtp);
        break;
    }
    case 5: {
        // Gửi tin nhắn và hình ảnh
        oSmtp->Subject = subject.c_str();
        oSmtp->BodyText = message.c_str();
        attachAndSendMail(fileName, oSmtp);
        break;
    }
    default:
        _tprintf(_T("Invalid option.\n"));
        return -1;
    }

    _tprintf(_T("Start to send email via gmail account ...\r\n"));

    if (oSmtp->SendMail() == 0) {
        _tprintf(_T("Email was sent successfully!\r\n"));
    }
    else {
        _tprintf(_T("Failed to send email with the following error: %s\r\n"),
            (const TCHAR*)oSmtp->GetLastErrDescription());
    }

    return 0;
}
