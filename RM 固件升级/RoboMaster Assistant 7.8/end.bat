echo start update...
ping -n 3 127.1 >nul
copy /y .\UpdatePackage\update.exe .\update.exe
rd /s /q .\UpdatePackage
start RoboMaster" "Assistant.exe