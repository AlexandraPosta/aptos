
set MATLAB=C:\Program Files\MATLAB\R2023b

cd .

chcp 1252

if "%1"=="" ("C:\PROGRA~1\MATLAB\R2023b\bin\win64\gmake"  -f Time_Varying_Model_2022a.mk all) else ("C:\PROGRA~1\MATLAB\R2023b\bin\win64\gmake"  -f Time_Varying_Model_2022a.mk %1)
@if errorlevel 1 goto error_exit

exit /B 0

:error_exit
echo The make command returned an error of %errorlevel%
exit /B 1