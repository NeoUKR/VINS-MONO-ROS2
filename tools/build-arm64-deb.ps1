param(
    [string]$Version = "0.0.1.7",
    [string]$OutputDirectory = "output"
)

$ErrorActionPreference = "Stop"

docker buildx build `
    --platform linux/arm64 `
    --target deb `
    --build-arg "PACKAGE_VERSION=$Version" `
    --output "type=local,dest=$OutputDirectory" `
    .

if ($LASTEXITCODE -ne 0) {
    throw "ARM64 Debian package build failed with exit code $LASTEXITCODE."
}

Get-ChildItem -LiteralPath $OutputDirectory -Filter "*.deb" |
    Select-Object FullName, Length, LastWriteTime
