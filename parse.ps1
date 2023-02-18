# Перед запуском скрипта нужно выполнить команду powershell
# PS> Set-ExecutionPolicy RemoteSigned
#
#
$ReferenceFile = 	"D:\mecalculateprogramm\MyTe\LIST_WBVR8766_hd.txt"
$SourceFile = 		"D:\mecalculateprogramm\MyTe\RES_TOTAL_140224.txt"
$DestinationFile = 	"D:\mecalculateprogramm\MyTe\resTOT_8766_140224.txt"

function ConvertTo-Encoding ([string]$From, [string]$To){
	Begin{
		$encFrom = [System.Text.Encoding]::GetEncoding($from)
		$encTo = [System.Text.Encoding]::GetEncoding($to)
	}
	Process{
		$bytes = $encTo.GetBytes($_)
		$bytes = [System.Text.Encoding]::Convert($encFrom, $encTo, $bytes)
		$encTo.GetString($bytes)
	}
}

function ProcessCatalog
{
	Param(
        [parameter(ValueFromPipeline=$true)]$ty,
        [Parameter(Mandatory=$true)][string]$RefFile,
        [parameter(Mandatory=$true)][string]$DestFile
	)
    Begin {
        # загружаем эталонный файл
        $Ref = Get-Content $RefFile
        # заменяем несколько пробелов подряд одним
        for($i = 0; $i -lt $Ref.Count; $i++){
            $Ref[$i] = $Ref[$i] -replace "[\s]+"," "
        }
        # создаём файл
        $NewFile = $true
        $Count = 0
    }
	process {
		$v = 0
        $found = $false
		if($ty -eq ''){
			return
		}
        if($NewFile){
            $NewFile = $false
            $ty > $DestFile
            return
        }
        # разделяем строку по символу '|'
		$t = $ty -split "[|]"
        # заменяем несколько пробелов подряд одним
        $tt = ($t[0] -replace "[\s]+"," ") -replace "([\s]+$)"
        foreach($r in $Ref){
            # сравниваем строки
            if($tt -eq $r){                
                #$ty | ConvertTo-Encoding "UTF-16" "windows-1251" >> $DestFile
                $ty >> $DestFile
                $Count++
                if($Count -gt 100){
                    "." | Write-Host -ForegroundColor green -NoNewline
                    $Count = 0
                }
                break;
            }
		}
	}
}

# открываем файл и построчно передаём его через конвейер в функцию
Get-Content $SourceFile -totalcount -1 | ProcessCatalog -RefFile $ReferenceFile -DestFile $DestinationFile

Write-Host "Complete" -ForegroundColor green
