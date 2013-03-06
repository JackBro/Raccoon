@echo off
title ���������Զ���������ǩ������
rem   =========���ϵͳ�Ƿ�װ Signtool=============
if not exist tool\signtool.exe goto NoExistSigntool
if not exist tool\capicom.dll goto NoExistSigntool

rem  =========���ǩ�������Ƿ�װ=========

if exist "%SystemRoot%\system32\capicom.dll" del "%SystemRoot%\system32\capicom.dll" /q /s > nul
copy tool\capicom.dll "%SystemRoot%\system32\capicom.dll" /y > nul
regsvr32 "%SystemRoot%\system32\capicom.dll" /s


rem  =========���ɴ�ǩ���ļ��б�=========

:listfile
rem  =========����Ƿ���Ѵ��ڴ�ǩ���ļ��б��������ɾ��=========
if exist list.txt del list.txt /q /s
cls
color 0f
echo.
echo                     �q�����������������������������������r
echo         �q������������   ���������Զ���������ǩ������   �������������r
echo         ��          �t�����������������������������������s          ��
echo         ��    ����˵����                                            ��
echo         ��                                                          ��
echo         ��        ������Ӧ��΢��signtool���ߣ���������Ϊ��������    ��
echo         ��                                                          ��
echo         ��    ���ǩ����                                            ��
echo         ��                                                          ��
echo         ��        ǩ���ļ��б�list.txt                              ��
echo         ��                                                          ��
echo         ��                  ����֧������ϵ��www.itrus.com.cn        ��
echo         ��                                                          ��
echo         �t�����������������������������������������������������������s
echo.
echo          ���������...
pause >nul

cls
echo.
echo.
echo         ====��ʾ����ֱ����ק pfx �ļ��������У�pfx �ļ�����Ϊ�տ���====
echo.
echo         ���������ǩ��֤��pfx�ļ�·��
set pfxfilesource=:
echo.
set /p pfxfilesource=         pfx�ļ�·����
set "pfxfilesource=%pfxfilesource:"=%"
:: �������Ϊ�ж�%pfxfilesource%���Ƿ�������ţ������޳���
if not exist "%pfxfilesource%" goto nopfxfile

:setpfxpasswd
echo.
set pfxpasswd=:
set /p pfxpasswd=         ������pfx�ļ����룺
if "%pfxpasswd%"==":" goto setpfxpasswd

echo.
echo.
echo         ====��ʾ����ֱ����ק��ǩ�� ���� �� ���������ļ��� ��������====
echo.
echo          ������ǩ���ļ��л򵥸��ļ�

:setinput
echo.
set input=:
set /p input=         �ļ�·����
set "input=%input:"=%"
:: �������Ϊ�ж�%input%���Ƿ�������ţ������޳���

if "%input%"==":" goto setinput
if not exist "%input%" goto errorpath

pushd "%input%">nul 2>nul && (popd & set folder=yes)||(set folder=no)

:: ��������ж��û��������һ���ļ��л���һ���ļ�

goto multifile%folder%

rem ����������һ���ļ�

:multifileno
cls
echo.
echo.
Echo          ----------------------------------------------------------
Echo                          �����������ǩ��ģʽ��ʼ
Echo          ----------------------------------------------------------
echo.

echo.
echo         �X�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�[
echo         �U                                                          �U
echo                           �������������Ϣ����ѡ��
echo         �U                                                          �U
set describe=:
set /p describe=             ���������
echo         �U                                                          �U
set describeURL=:
set /p describeURL=             ��������(����http/https)��
echo         �U                                                          �U
echo         �^�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�a
echo.

if "%describe%"==":" goto s_no_describe
if "%describeURL%"==":" goto s_no_describe

tool\signtool.exe sign /f "%pfxfilesource%" /p "%pfxpasswd%" /d "%describe%" /du "%describeURL%" /t "http://timestamp.verisign.com/scripts/timstamp.dll" "%input%"
goto choice1

:s_no_describe
tool\signtool.exe sign /f "%pfxfilesource%" /p "%pfxpasswd%" /t "http://timestamp.verisign.com/scripts/timstamp.dll" "%input%"
echo %input%>list.txt
ping 127.1 -n 4 >nul

:choice1
echo.
echo              �X�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�[
echo              �U                                              �U
echo              �U            �����������ǩ�����              �U
echo              �U                                              �U
echo              �U       ��ѡ�� 0 �˳� / 1 ���ؼ���ǩ����       �U
echo              �U                                              �U
echo              �^�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�a
echo.
set choice1=
set /p choice1=                           ��ѡ��(0/1): 
if /i "%choice1%"=="0" goto eof
if /i "%choice1%"=="1" goto listfile
echo          �����ѡ��
goto choice1

rem ����������һ���ļ���

:multifileyes
cls

for /r "%input%" %%i in (*.exe *.dll *.cab *.ocx *.mui *.vbs *.sys *.cat *.msi) do echo %%~fi>>list.txt
if not exist list.txt goto no_suitfile

echo.
echo.
Echo          ----------------------------------------------------------
Echo                          �����������ǩ��ģʽ��ʼ
Echo          ----------------------------------------------------------
echo.

echo.
echo         �X�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�[
echo         �U                                                          �U
echo                           �������������Ϣ����ѡ��
echo         �U                                                          �U
set describe=:
set /p describe=             ���������
echo         �U                                                          �U
set describeURL=:
set /p describeURL=             ��������(����http/https)��
echo         �U                                                          �U
echo         �^�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�a
echo.

if "%describe%"==":" goto m_no_describe
if "%describeURL%"==":" goto m_no_describe

for /f "delims=" %%1 in (list.txt) do tool\signtool.exe sign /f "%pfxfilesource%" /p "%pfxpasswd%" /d "%describe%" /du "%describeURL%" /t "http://timestamp.verisign.com/scripts/timstamp.dll" "%%~1"
goto choice2

:m_no_describe
for /f "delims=" %%1 in (list.txt) do tool\signtool.exe sign /f "%pfxfilesource%" /p "%pfxpasswd%" /t "http://timestamp.verisign.com/scripts/timstamp.dll" "%%~1"
ping 127.1 -n 4 >nul

:choice2
echo.
echo              �X�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�[
echo              �U                                              �U
echo              �U            �����������ǩ�����              �U
echo              �U                                              �U
echo              �U       ��ѡ�� 0 �˳� / 1 ���ؼ���ǩ����       �U
echo              �U                                              �U
echo              �^�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�a
echo.
set choice2=
set /p choice2=                           ��ѡ��(0/1): 
if /i "%choice2%"=="0" goto eof
if /i "%choice2%"=="1" goto listfile
echo          �����ѡ��
goto choice2



echo      ����ɳ���ǩ�����������򽫴���ǩ�����ļ��б��Զ��˳���
ping 127.1 -n 6 >nul
start list.txt
ping 127.1 -n 4 >nul
goto exit


:errorpath
cls
echo.
echo.
echo.
echo                     �q�����������������������������������r
echo         �q������������   ���������Զ���������ǩ������   �������������r
echo         ��          �t�����������������������������������s          ��
echo         ��                                                          ��
echo         ��    Ŀ¼���ļ������ڣ����������룡                        ��
echo         ��                                                          ��
echo         ��                             �����������...              ��
echo         ��                                                          ��
echo         �t�����������������������������������������������������������s
pause > nul
goto listfile

:no_suitfile
cls
echo.
echo.
echo.
echo                     �q�����������������������������������r
echo         �q������������   ���������Զ���������ǩ������   �������������r
echo         ��          �t�����������������������������������s          ��
echo         ��                                                          ��
echo                "%input%"
echo         ��                                                          ��
echo         ��    Ŀ¼��û�п�ǩ���ļ�������������Ŀ¼��                ��
echo         ��                                                          ��
echo         ��                             �����������...              ��
echo         ��                                                          ��
echo         �t�����������������������������������������������������������s
pause > nul
goto listfile

:NoExistSigntool
cls
color 0c
echo.
echo.
echo.
echo.
echo.
echo                             �q���������������������r
echo           �q������������������  Signtool ��װ���� �������������������r
echo           ��                �t���������������������s                ��
echo           ��                                                        ��
echo           ��       Signtool ȱʧ��������������Ժ�����            ��
echo           ��                                                        ��
echo           �t���������������������������������������������������������s
ping 127.1 -n 6 >nul
echo.
echo           ���򼴽��˳�
start http://www.itrus.com.cn/
ping 127.1 -n 4 >nul && goto :eof