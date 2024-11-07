$mod = "./mod.json"

qpm qmod manifest

$modJson = Get-Content $mod -Raw | ConvertFrom-Json

$modName = $modJson.name
$modVersion = "v" + $modJson.version
$bsVersion = "v" + $modJson.packageVersion.split("_")[0]
 
Write-Host "Initiating workflow for $modName $modVersion for Beat Saber $bsVersion"

gh workflow run build-ndk.yml -f version=$modVersion -f bs_version=$bsVersion
