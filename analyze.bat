@echo off
..\tools\CodeXLShaderAnalyzer\CodeXLAnalyzer-x64.exe -c Bonaire -s hlsl -f main -D _s03 -p cs_5_1 --DXFlags 294912 --isa isa.txt --livereg livereg.txt -a stats.txt eneida.hlsl